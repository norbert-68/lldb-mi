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
#include <sstream>
#include "commands/ListFeatures.hpp"
#include "commands/GdbVersion.hpp"

namespace lldbmi {

std::string ResultVector::toString() const
{
    std::ostringstream strstr;
    strstr << open;
    if (!empty())
    {
        std::vector<std::string>::const_iterator itr = begin();
        strstr << *(itr++);
        for (; itr != end(); ++itr)
        {
            strstr << "," << *itr;
        }

    }
    strstr << close;
    return strstr.str();
}

void Command::execute()
{
    if (interpreter.hasLog())
        interpreter.getLog() << __FUNCTION__ << " " << operation << std::endl;
    if (operation.compare("-list-features") == 0)
    {
        ListFeatures command(*this);
        command.execute();
    }
    else if (operation.compare("-gdb-version") == 0)
    {
        GdbVersion command(*this);
        command.execute();
    }
}

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
        if (interpreter.hasLog())
            interpreter.getLog() << __FUNCTION__ << " operation=" << operation << std::endl;
    }
}

void Command::writeOutput()
{
    std::ostringstream strstr;
    if (!token.empty())
        strstr << token;
    strstr << "^" << resultClass;
    for (const Result & result : results)
        strstr << "," << result;
    interpreter.writeOutput(strstr.str());
}

} // namespace lldbmi

std::ostream & operator<<(std::ostream & out, const lldbmi::Result & result)
{
    return out << result.variabe << "=" << result.value ;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::ResultClass & resultClass)
{
    switch (resultClass.value)
    {
    case lldbmi::ResultClass::DONE:
        return out << "done";
    case lldbmi::ResultClass::CONNECTED:
        return out << "connected";
    case lldbmi::ResultClass::ERROR:
        return out << "error";
    case lldbmi::ResultClass::EXIT:
        return out << "exit";
    }
    return out;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::Option & option)
{
    out << option.name;
    if (!option.parameter.empty())
        out << " " << option.parameter;
    return out;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::Command & command)
{
    return out;
}
