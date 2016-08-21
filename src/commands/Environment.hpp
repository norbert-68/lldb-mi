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
#ifndef LLDBMI_COMMAND_ENVIRONMENT_HPP
#define LLDBMI_COMMAND_ENVIRONMENT_HPP

#include "../Command.hpp"
#include <lldbmi/Interpreter.hpp>
#include <cstring>
#ifdef __unix__
#include <unistd.h>
#endif

namespace lldbmi {

/**
 * @brief Implements
 * -environment-cd
 */
struct Environment : public Command
{
    Environment(const Command & command) :
        Command(command)
    {
    }

    virtual Command & execute()
    {
        if (operation.compare("-environment-cd") == 0)
        {
            if (!parameters.empty())
            {
                int result = chdir(parameters.front().c_str());
                if (result != 0)
                {
                    std::string msg(parameters.front().c_str());
                    msg.append(": ");
                    char buffer[256];
#if ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE)
                    strerror_r(errno, buffer, sizeof(buffer));
                    msg.appnd(buffer);
#else
                    const char * result = strerror_r(errno, buffer, sizeof(buffer));
                    msg.append(result);
#endif
                    msg.append(".");
                    setError(msg);
                }
            }
            else
            {
                std::string msg(operation);
                msg.append(": usage DIRECTORY");
                setError(msg);
            }
        }
        return *this;
    }

};

} // namespace lldbmi

#endif // LLDBMI_COMMAND_ENVIRONMENT_HPP
