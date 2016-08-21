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
#ifndef LLDBMI_COMMAND_TEMPLATE_HPP
#define LLDBMI_COMMAND_TEMPLATE_HPP

#include "../Command.hpp"
#include <lldbmi/Interpreter.hpp>

namespace lldbmi {

/**
 * @brief Implements
 */
struct Template : public Command
{
    Template(const Command & command) :
        Command(command)
    {
    }

    virtual Command & execute()
    {
        //if (operation.compare("-exec-and-symbols") == 0)
        //{
        //}
        return *this;
    }
};

} // namespace lldbmi

#endif // LLDBMI_COMMAND_TEMPLATE_HPP
