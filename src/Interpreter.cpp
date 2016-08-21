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

#include "commands/Gdb.hpp"

namespace lldbmi {

const char * Interpreter::endl = "\r\n";

void Interpreter::addOutOfBandRecord(const std::string & record)
{
    outOfBandRecords.push_back(record);
}

lldb::SBDebugger & Interpreter::getDebugger()
{
    if (!debugger)
        debugger.reset(new lldb::SBDebugger(lldb::SBDebugger::Create(sourc_init_files != flag_off)));
    return *debugger;
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
    while (true)
    {
        getOut() << "(gdb)" << endl;
        std::string commandLine;
        std::getline(getIn(), commandLine);
        if (hasLog())
            getLog() << __FUNCTION__ << " getline: " << commandLine << std::endl;

        Command command(*this);
        command.parse(commandLine);

        if (hasLog())
            getLog() << command << std::endl;
        command.execute();

        std::ostringstream strstr;
        writeOutput(strstr, command.getOutput());
        std::string output = strstr.str();
        if (hasLog())
            getLog() << output;
        getOut() << output;

        if (command.getResultClass() == Command::ResultClass::EXIT)
            break;
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

void Interpreter::writeOutput(std::ostream & stream, const std::string & resultRecord)
{
    for (const std::string & outOfBandRecord : outOfBandRecords)
        stream << outOfBandRecord << endl;
    outOfBandRecords.clear();
    stream << resultRecord << endl;
}

} // namespace lldbmi

std::ostream & operator<<(std::ostream & out, const lldbmi::Flag & var)
{
    switch (var)
    {
    case lldbmi::flag_on:   return out << "on";
    case lldbmi::flag_off:  return out << "off";
    case lldbmi::flag_auto: return out << "auto";
    }
    return out;
}

std::string & operator>>(std::string & str, lldbmi::Flag & var)
{
    if (str.compare("on") == 0)
    {
        var = lldbmi::flag_on;
    }
    else if (str.compare("off") == 0)
    {
        var = lldbmi::flag_off;
    }
    else
    {
        var = lldbmi::flag_auto;
    }
    return str;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::PythonPrintStack & var)
{
    switch (var)
    {
    case lldbmi::python_print_stack_none:    return out << "none";
    case lldbmi::python_print_stack_message: return out << "message";
    case lldbmi::python_print_stack_full:    return out << "full";
    }
    return out;
}

std::string & operator>>(std::string & str, lldbmi::PythonPrintStack & var)
{
    if (str.compare("full") == 0)
    {
        var = lldbmi::python_print_stack_full;
    }
    else if (str.compare("message") == 0)
    {
        var = lldbmi::python_print_stack_message;
    }
    else
    {
        var = lldbmi::python_print_stack_none;
    }
    return str;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::Charset & var)
{
    switch (var)
    {
    case lldbmi::charset_default: return out << "default";
    case lldbmi::charset_utf8:    return out << "UTF-8";
    case lldbmi::charset_utf16:   return out << "UTF-16";
    case lldbmi::charset_utf32:   return out << "UTF-32";
    }
    return out;
}

std::string & operator>>(std::string & str, lldbmi::Charset & var)
{
    if (str.compare("UTF-8") == 0)
    {
        var = lldbmi::charset_utf8;
    }
    else if (str.compare("UTF-16") == 0)
    {
        var = lldbmi::charset_utf16;
    }
    else if (str.compare("UTF-32") == 0)
    {
        var = lldbmi::charset_utf32;
    }
    else
    {
        var = lldbmi::charset_default;
    }
    return str;
}
