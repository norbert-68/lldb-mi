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
#ifndef LLDBMI_COMMAND_GDB_HPP
#define LLDBMI_COMMAND_GDB_HPP

#include "../MICommand.hpp"
#include <lldbmi/MIInterpreter.hpp>

namespace lldbmi {

/**
 * @brief Implements
 * -gdb-exit
 * -gdb-version
 * -gdb-set
 */
struct Gdb : public MICommand
{
    Gdb(MIInterpreter & interpreter) :
        MICommand(interpreter)
    {
        init();
    }

    Gdb(const MICommand & command) :
        MICommand(command)
    {
        init();
    }

    virtual MICommand & execute()
    {
        if (operation.compare("-gdb-exit") == 0)
        {
            resultClass = ResultClass::exit;
        } // -gdb-exit
        else if (operation.compare("-gdb-show") == 0)
        {
            std::string threadGroup;
            for (const Option & option : options)
                if (option.name.compare("--thread-group") == 0)
                    threadGroup = option.parameter;
            MITarget * ptarget = nullptr;
            if (!threadGroup.empty())
                ptarget = interpreter.findTarget(threadGroup);
            if (parameters.size() > 0)
            {
                if (parameters.at(0).compare("language") == 0)
                {
                    Language language = ptarget == nullptr ? interpreter.language : ptarget->language;
                    Result result("value", CString(toString<Language>(language)));
                    results.push_back(result);
                }
            }
        } // -gdb-show
        else if (operation.compare("-gdb-version") == 0)
        {
            for (const std::string & version : getVersion())
            {
                std::string consoleStreamOutput("~\"");
                consoleStreamOutput.append(version).append("\\n\"");
                interpreter.addOutOfBandRecord(consoleStreamOutput);
            }
        } // -gdb-version
        else if (operation.compare("-gdb-set") == 0)
        {
            if (parameters.size() > 0)
            {
                if (parameters.at(0).compare("breakpoint") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        if (parameters.at(1).compare("pending") == 0)
                        {
                            if (parameters.size() > 2)
                            {
                                parameters.at(2) >> interpreter.breakpoint_pending;
                                if (interpreter.hasLog())
                                    interpreter.getLog() << __FUNCTION__ << ": breakpoint pending " << interpreter.breakpoint_pending << std::endl;
                            }
                        }
                    }
                } // breakpoint
                else if (parameters.at(0).compare("detach-on-fork") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        parameters.at(1) >> interpreter.detach_on_fork;
                        if (interpreter.hasLog())
                            interpreter.getLog() << __FUNCTION__ << ": detach-on-fork " << interpreter.detach_on_fork << std::endl;
                    }
                } // detach-on-fork
                else if (parameters.at(0).compare("python") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        if (parameters.at(1).compare("print-stack") == 0)
                        {
                            if (parameters.size() > 2)
                            {
                                parameters.at(2) >> interpreter.python_print_stack;
                                if (interpreter.hasLog())
                                    interpreter.getLog() << __FUNCTION__ << ": python print-stack " << interpreter.python_print_stack << std::endl;
                            }
                        }
                    }
                } // python
                else if (parameters.at(0).compare("print") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        if (parameters.at(1).compare("object") == 0)
                        {
                            if (parameters.size() > 2)
                            {
                                parameters.at(2) >> interpreter.print_object;
                                if (interpreter.hasLog())
                                    interpreter.getLog() << __FUNCTION__ << ": print object " << interpreter.print_object << std::endl;
                            }
                        }
                        else if (parameters.at(1).compare("sevenbit-strings") == 0)
                        {
                            if (parameters.size() > 2)
                            {
                                parameters.at(2) >> interpreter.print_sevenbit_strings;
                                if (interpreter.hasLog())
                                    interpreter.getLog() << __FUNCTION__ << ": print sevenbit-strings " << interpreter.print_sevenbit_strings << std::endl;
                            }
                        }
                    }
                }
                else if (parameters.at(0).compare("host-charset") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        parameters.at(1) >> interpreter.host_charset;
                        if (interpreter.hasLog())
                            interpreter.getLog() << __FUNCTION__ << ": host-charset " << interpreter.host_charset << std::endl;
                    }
                } // host-charset
                else if (parameters.at(0).compare("target-charset") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        parameters.at(1) >> interpreter.target_charset;
                        if (interpreter.hasLog())
                            interpreter.getLog() << __FUNCTION__ << ": target-charset " << interpreter.target_charset << std::endl;
                    }
                } // target-charset
                else if (parameters.at(0).compare("target-wide-charset") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        parameters.at(1) >> interpreter.target_wide_charset;
                        if (interpreter.hasLog())
                            interpreter.getLog() << __FUNCTION__ << ": target-wide-charset " << interpreter.target_wide_charset << std::endl;
                    }
                } // target-wide-charset
                else if (parameters.at(0).compare("target-async") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        parameters.at(1) >> interpreter.target_async;
                        if (interpreter.hasLog())
                            interpreter.getLog() << __FUNCTION__ << ": target-async " << interpreter.target_async << std::endl;
                    }
                } // target-async
                else if (parameters.at(0).compare("pagination") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        parameters.at(1) >> interpreter.pagination;
                        if (interpreter.hasLog())
                            interpreter.getLog() << __FUNCTION__ << ": pagination " << interpreter.pagination << std::endl;
                    }
                } // pagination
                else if (parameters.at(0).compare("non-stop") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        parameters.at(1) >> interpreter.non_stop;
                        if (interpreter.hasLog())
                            interpreter.getLog() << __FUNCTION__ << ": non-stop " << interpreter.non_stop << std::endl;
                    }
                } // non-stop
                else if (parameters.at(0).compare("auto-solib-add") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        parameters.at(1) >> interpreter.auto_solib_add;
                        if (interpreter.hasLog())
                            interpreter.getLog() << __FUNCTION__ << ": auto-solib-add " << interpreter.auto_solib_add << std::endl;
                    }
                } // auto-solib-add
                else if (parameters.at(0).compare("language") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        std::string threadGroup;
                        for (const Option & option : options)
                            if (option.name.compare("--thread-group") == 0)
                                threadGroup = option.parameter;
                        MITarget * ptarget = nullptr;
                        if (!threadGroup.empty())
                            ptarget = interpreter.findTarget(threadGroup);
                        Language & language = ptarget == nullptr ? interpreter.language : ptarget->language;
                        parameters.at(1) >> language;
                    }
                } // language
            }
        } // -gdb-set
        return *this;
    }

    const std::vector<std::string> & getVersion() const
    {
        return version;
    }

protected:

    void init()
    {
        version.push_back("GNU gdb (GDB) 7.6");
        version.push_back("A MI command line interface for LLDB.");
        version.push_back("All rights reserved.");
    }

    std::vector<std::string> version;

};

} // namespace lldbmi

#endif // LLDBMI_COMMAND_GDB_HPP
