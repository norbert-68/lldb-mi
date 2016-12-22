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
#ifndef LLDBMI_COMMAND_INTERPRETER_HPP
#define LLDBMI_COMMAND_INTERPRETER_HPP

#include "../MICommand.hpp"
#include <lldbmi/MIInterpreter.hpp>
#include <lldb/API/SBCommandInterpreter.h>
#include <lldb/API/SBCommandReturnObject.h>

namespace lldbmi {

/**
 * @brief Implements
 * -interpreter-exec
 */
struct Interpreter : public MICommand
{
    Interpreter(const MICommand & command) :
        MICommand(command)
    {
    }

    virtual MICommand & execute()
    {
        if (operation.compare("-interpreter-exec") == 0)
        {
            if (parameters.size() > 1)
            {
                std::string console = parameters.at(0);
                std::string command = parameters.at(1);
                if (command.at(0) == '"' && command.length() > 1)
                {
                    command.erase(0, 1);
                    command.erase(command.length() -1, 1);
                }
                if (interpreter.hasLog())
                    interpreter.getLog() << __FUNCTION__ << ": " << console << " '" << command << "'" << std::endl;

                if (console.compare("console") == 0)
                {
                    lldb::SBCommandInterpreter consoleInterpreter = interpreter.getDebugger().GetCommandInterpreter();
                    lldb::SBCommandReturnObject result;
                    lldb::ReturnStatus status = consoleInterpreter.HandleCommand(command.c_str(), result);
                    if (interpreter.hasLog())
                        interpreter.getLog() << __FUNCTION__ << ": " << (int)status << std::endl;
                    if (status == lldb::eReturnStatusSuccessFinishResult)
                        interpreter.addConsoleStreamOutput(result.GetOutput());
                    else if (status == lldb::eReturnStatusInvalid || status == lldb::eReturnStatusFailed)
                        interpreter.addConsoleStreamOutput(result.GetError());

                }
            }
        } // -interpreter-exec
        return *this;
    }
};

} // namespace lldbmi

#endif // LLDBMI_COMMAND_INTERPRETER_HPP
