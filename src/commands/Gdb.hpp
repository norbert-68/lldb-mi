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

#include "../Command.hpp"
#include <lldbmi/Interpreter.hpp>

namespace lldbmi {

/**
 * @brief Implements
 * -gdb-version
 */
struct Gdb : public Command
{
    Gdb(Interpreter & interpreter) :
        Command(interpreter)
    {
        init();
    }

    Gdb(const Command & command) :
        Command(command)
    {
        init();
    }

    virtual Command & execute()
    {
        if (operation.compare(4, 5, "-exit") == 0)
        {
            resultClass = ResultClass::EXIT;
        } // -gdb-exit
        else if (operation.compare(4, 8, "-version") == 0)
        {
            for (const std::string & version : getVersion())
            {
                std::string consoleStreamOutput("~\"");
                consoleStreamOutput.append(version).append("\\n\"");
                interpreter.addOutOfBandRecord(consoleStreamOutput);
            }
        } // -gdb-version
        else if (operation.compare(4, 4, "-set") == 0)
        {
            if (parameters.size() > 0)
            {
                if (parameters.at(0).compare("breakpoint") == 0)
                {
                    if (parameters.size() > 1)
                    {
                        if (parameters.at(1).compare("pending") == 0)
                        {
                            if ( parameters.size() > 2
                              && ( parameters.at(2).compare("on") == 0
                                || parameters.at(2).compare("auto") == 0 ) )
                            {
                                // default behavior in lldb
                                interpreter.breakpoint_pending = true;
                            }
                            else if (parameters.size() > 2 && parameters.at(2).compare("off") == 0)
                            {
                                interpreter.breakpoint_pending = false;
                            }
                            else
                            {
                                std::string msg("on, off or auto expected.");
                                setError(msg);
                            }
                        }
                        else
                        {
                            std::string msg("undefined set breakpoint command: ");
                            msg.append(parameters.at(1)).append(".");
                            setError(msg);
                        }
                    }
                } // breakpoint
                else if (parameters.at(0).compare("detach-on-fork") == 0)
                {
                    if (parameters.size() > 1 && parameters.at(1).compare("on") == 0)
                    {
                        // default behavior in lldb
                        interpreter.detach_on_fork = true;
                    }
                    else if (parameters.size() > 1 && parameters.at(2).compare("off") == 0)
                    {
                        interpreter.detach_on_fork = false;
                    }
                    else
                    {
                        std::string msg("on or off expected.");
                        setError(msg);
                    }
                } // detach-on-fork
                else
                {
                    std::string msg("undefined set command: ");
                    msg.append(parameters.at(0)).append(".");
                    setError(msg);
                }
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
