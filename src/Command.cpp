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

#include "Command.hpp"
#include <lldbmi/Interpreter.hpp>
#include <cctype>

namespace lldbmi {

void Command::parse(const std::string & commandLine)
{
    std::size_t i = 0;
    if (i < commandLine.length() && isdigit(commandLine.at(i)))
    {
        while (i < commandLine.length() && isdigit(commandLine.at(i)))
            token.append(1, commandLine.at(i++));
    }
    if (i < commandLine.length() && commandLine.at(i) == '-')
    {
        while (i < commandLine.length() &&  commandLine.at(i) != ' ')
            operation.append(1, commandLine.at(i++));
    }
}

} // namespace lldbmi

std::ostream & operator<<(std::ostream & out, const lldbmi::Option & option)
{
    out << option.name;
    if (!option.parameter.empty())
        out << " " << option.parameter;
    return out;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::Command & command)
{
    if (!command.token.empty())
        out << command.token;
    out << command.operation;
    for (const lldbmi::Option & option : command.options)
        out << " " << option;
    if (!command.parameters.empty())
    {
        out << "--";
        for (const std::string & parameter : command.parameters)
            out << " " << parameter;
    }
    return out;
}
