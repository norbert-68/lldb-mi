//******************************************************************************
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

#include <LldbMi.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

namespace lldbmi
{

std::string LldbMiInterpreter::getTime()
{
    time_t t = time(0);
    char timstr[52];
    ctime_r(&t, timstr);
    for (std::size_t i = strlen(timstr); i > 0 && timstr[i-1] == '\n'; --i)
        timstr[i-1] = 0;
    return std::string(timstr);
}

int LldbMiInterpreter::parseOptions(const char * option, int argc, char * args[], std::string * value)
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

void LldbMiInterpreter::start(int argc, char * args[])
{
    outStream = &std::cout;
    if (logStream)
        logStream.reset();
    if (!logFilename.empty())
        logFilename.clear();

    modifiedArgc = parseOptions("--log", argc, args, &logFilename);

    if (!logFilename.empty())
    {
        logStream.reset(new std::fstream(logFilename.c_str(), std::ios::out|std::ios::app));
        if (!*logStream)
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
            getLog() << __FUNCTION__ << " args[" << i << "]='" << args[i] << "'" << std::endl;
    }

    if (parseOptions("--version", modifiedArgc, args) != modifiedArgc)
    {
        getOut() << "GNU gdb (GDB) 7.6"                     << std::endl
                 << "A MI command line interface for LLDB." << std::endl
                 << "All rights reserved."                  << std::endl;
        return;
    }

    modifiedArgc = parseOptions("--interpreter", modifiedArgc, args, &interpreter);
    if (hasLog())
        getLog() << "Selected interpreter: " << interpreter << std::endl;

    int nxArgc = parseOptions("--nx", modifiedArgc, args);
    if (nxArgc != modifiedArgc)
    {
        modifiedArgc = nxArgc;
        if (hasLog())
            getLog() << "Do not execute commands from any initialization files" << std::endl;
    }
}

} // namespace lldbmi
