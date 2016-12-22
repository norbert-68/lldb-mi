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

#include <lldbmi/MIInterpreter.hpp>
#include <lldbmi/MITarget.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

#include "commands/Gdb.hpp"
#include "MICommand.hpp"

namespace lldbmi {

const char * MIInterpreter::endl = "\r\n";

MIInterpreter::~MIInterpreter()
{
}

void MIInterpreter::addConsoleStreamOutput(const std::string & record)
{
    std::string str("~\"");
    str.append(record).append("\\n\"");
    addConsoleStreamOutput(record);
}

void MIInterpreter::addOutOfBandRecord(const std::string & record)
{
    std::lock_guard<std::recursive_mutex> lock(*this);
    outOfBandRecords.push_back(record);
}

void MIInterpreter::addTarget(MITargetPtr && target)
{
    targets[target->getThreadGroup()] = std::move(target);
}

MITarget * MIInterpreter::findTarget(const std::string & threadGroup)
{
    MITargets::const_iterator itarget = targets.find(threadGroup);
    if (itarget == targets.end())
        return nullptr;
    return itarget->second.get();
}

lldb::SBDebugger & MIInterpreter::getDebugger()
{
    if (!debugger)
        debugger.reset(new lldb::SBDebugger(lldb::SBDebugger::Create(sourc_init_files != flag_off)));
    return *debugger;
}

std::string MIInterpreter::getTime()
{
    time_t t = time(nullptr);
    char timstr[52];
    ctime_r(&t, timstr);
    for (std::size_t i = strlen(timstr); i > 0 && timstr[i-1] == '\n'; --i)
        timstr[i-1] = 0;
    return std::string(timstr);
}

int MIInterpreter::parseOptions(const char * option, int argc, char * args[], std::string * value)
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

void MIInterpreter::readEvalLoop()
{
    while (true)
    {
        getOut() << "(gdb)" << endl;
        std::string commandLine;
        std::getline(getIn(), commandLine);
        if (hasLog())
            getLog() << __FUNCTION__ << " getline: " << commandLine << std::endl;

        MICommand command(*this);
        command.parse(commandLine);

        if (hasLog())
            getLog() << command << std::endl;
        command.execute();

        std::ostringstream strstr;
        writeOutput(strstr, command.getOutput());
        std::string output = strstr.str();
        if (hasLog())
        {
            getLog() << output;
            getLog().flush();
        }
        getOut() << output;
        getOut().flush();

        if (command.getResultClass() == ResultClass::exit)
            break;
    }
}

void MIInterpreter::start(int argc, char * args[], std::istream & in, std::ostream & out)
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
        Gdb gdb(*this);
        for (const std::string & version : gdb.getVersion())
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
        sourc_init_files = flag_off;
    }

    lldb::SBDebugger::Initialize();
    readEvalLoop();
}

void MIInterpreter::writeOutput(std::ostream & stream, const std::string & resultRecord)
{
    for (const std::string & outOfBandRecord : outOfBandRecords)
        stream << outOfBandRecord << endl;
    outOfBandRecords.clear();
    stream << resultRecord << endl;
}

} // namespace lldbmi

