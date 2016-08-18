//******************************************************************************
//
// Copyright 2016 by Norbert Klose (norbert.klose@web.de)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//******************************************************************************

#include <lldbmi/Interpreter.hpp>
#include "Command.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include "commands/GdbVersion.hpp"

namespace lldbmi {

const char * Interpreter::endl = "\r\n";

void Interpreter::addOutOfBandRecord(const std::string & record)
{
    outOfBandRecords.push_back(record);
}

std::string Interpreter::getTime()
{
    time_t t = time(0);
    char timstr[52];
    ctime_r(&t, timstr);
    for (std::size_t i = strlen(timstr); i > 0 && timstr[i-1] == '\n'; --i)
        timstr[i-1] = 0;
    return std::string(timstr);
}

int Interpreter::parseOptions(const char * option, int argc, char * args[], std::string * value)
{
    int modifiedArgc = argc;
    int optionLength = strlen(option);
    for (int i = 1; i < modifiedArgc; ++i)
    {
        if (strncmp(args[i], option, optionLength) == 0)
        {
            int shift = 0;
            if (args[i][optionLength] == '=')
            {
                if (value)
                    *value = &(args[i][optionLength+1]);
                shift = 1;
            }
            else if (args[i][optionLength] == 0 && value && (i+1) < modifiedArgc)
            {
                *value = args[i+1];
                shift = 2;
            }
            else if (args[i][optionLength] == 0 && !value)
            {
                shift = 1;
            }
            if (shift && (i+shift) < modifiedArgc)
            {
                char * tmp[2];
                for (int j = 0; j < shift; ++j)
                    tmp[j] = args[i+j];
                for (int j = i+shift; j < modifiedArgc; ++j)
                    args[j-shift] = args[j];
                for (int j = shift; j > 0; --j)
                    args[modifiedArgc-j] = tmp[shift-j];
                --i;
            }
            modifiedArgc -= shift;
        }
    }
    return modifiedArgc;
}

void Interpreter::readEvalLoop()
{
    writeOutput();
    while (true)
    {
        std::string commandLine;
        std::getline(getIn(), commandLine);
        if (hasLog())
            getLog() << __FUNCTION__ << " getline: " << commandLine << std::endl;
        Command command(*this);
        command.parse(commandLine);
        command.execute();
    }
}

void Interpreter::start(int argc, char * args[], std::istream & in, std::ostream & out)
{
    this->in = &in;
    this->out = &out;
    if (log)
        log.reset();
    if (!logFilename.empty())
        logFilename.clear();

    modifiedArgc = parseOptions("--log", argc, args, &logFilename);

    if (!logFilename.empty())
    {
        log.reset(new std::fstream(logFilename.c_str(), std::ios::out|std::ios::app));
        if (!*log)
        {
            std::ostringstream strstr;
            strstr << "can not open logfile '" << logFilename << "'";
            throw std::runtime_error(strstr.str());
        }
    }

    if (hasLog())
    {
        getLog() << getTime() << " LLDB-MI started." << std::endl;
        for (int i = 0; i < argc; ++i)
            getLog() << __FUNCTION__ << " args[" << i << "] " << args[i] << std::endl;
    }

    if (parseOptions("--version", modifiedArgc, args) != modifiedArgc)
    {
        GdbVersion gdbVersion(*this);
        for (const std::string & version : gdbVersion.getVersion())
            getOut() << version << std::endl;
        return;
    }

    modifiedArgc = parseOptions("--interpreter", modifiedArgc, args, &interpreter);
    if (hasLog())
        getLog() << __FUNCTION__ << " selected interpreter: " << interpreter << std::endl;

    int nxArgc = parseOptions("--nx", modifiedArgc, args);
    if (nxArgc != modifiedArgc)
    {
        modifiedArgc = nxArgc;
        if (hasLog())
            getLog() << __FUNCTION__ << " do not read any initialization files" << std::endl;
    }

    readEvalLoop();
}

void Interpreter::writeOutput(const std::string & resultRecord)
{
    std::ostringstream strstr;
    writeOutput(strstr, resultRecord);
    std::string str = strstr.str();
    if (hasLog())
        getLog() << __FUNCTION__ << " " << str;
    getOut() << str;
    getOut().flush();
}

void Interpreter::writeOutput(std::ostream & stream, const std::string & resultRecord)
{
    for (const std::string & outOfBandRecord : outOfBandRecords)
    {
        stream << outOfBandRecord << endl;
    }
    outOfBandRecords.clear();
    if (!resultRecord.empty())
    {
        stream << resultRecord << endl;
    }
    stream << "(gdb)" << endl;
}

} // namespace lldbmi
