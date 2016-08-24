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
#ifndef LLDBMI_COMMAND_FILE_HPP
#define LLDBMI_COMMAND_FILE_HPP

#include "../MICommand.hpp"
#include <lldbmi/MIInterpreter.hpp>
#include <lldb/API/SBTarget.h>
#include <mutex>

namespace lldbmi {

/**
 * @brief Implements
 * -file-exec-and-symbols
 */
struct File : public MICommand
{
    File(const MICommand & command) :
        MICommand(command)
    {
    }

    virtual MICommand & execute()
    {
        if (operation.compare("-file-exec-and-symbols") == 0)
        {
            std::string threadGroup;
            lldb::SBTarget target;

            for (const Option & option : options)
            {
                if (option.name.compare("--thread-group") == 0)
                    threadGroup = option.parameter;
            }

            if (parameters.size() > 0)
            {
                if (interpreter.hasLog())
                    interpreter.getLog() << __FUNCTION__ << ": file-exec-and-symbols " << parameters.at(0) << std::endl;

                std::unique_lock<std::recursive_mutex> lock(interpreter);

                lldb::SBDebugger & debugger = interpreter.getDebugger();
                lldb::SBError error;
                target = debugger.CreateTarget(parameters.at(0).c_str(), nullptr, nullptr, interpreter.auto_solib_add, error);

                if (!error.Success())
                {
                    setError(error.GetCString());
                }
                else if (target.IsValid())
                {
                    MITarget * ptarget = new MITarget(debugger, target, threadGroup);
                    interpreter.addTarget(MITargetPtr(ptarget));
                    if (interpreter.hasLog())
                        interpreter.getLog() << __FUNCTION__ << ": target " << ptarget->getThreadGroup() << std::endl;
                }
            }

        }
        return *this;
    }
};

} // namespace lldbmi

#endif // LLDBMI_COMMAND_FILE_HPP
