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

#include <lldbmi/MIInterpreter.hpp>
#include <cctype>
#include <sstream>

#include "commands/Enable.hpp"
#include "commands/Environment.hpp"
#include "commands/File.hpp"
#include "commands/Gdb.hpp"
#include "commands/Interpreter.hpp"
#include "commands/List.hpp"
#include "MICommand.hpp"

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

CString::CString(unsigned long val)
{
    std::ostringstream strstr;
    strstr << val;
    value = strstr.str();
}

std::string CString::toString() const
{
    std::ostringstream strstr;
    strstr << '"';
    for (char c : value)
    {
        if (c == '"')
            strstr << '\\';
        strstr << c;
    }
    strstr << '"';
    return strstr.str();
}

std::string Result::toString() const
{
    std::ostringstream strstr;
    strstr << variabe;
    if (!value.empty())
        strstr << "=" << value;
    return strstr.str();
}

MICommand & MICommand::execute()
{
    if (operation.compare(0, 7, "-enable") == 0)
    {
        Enable command(*this);
        *this = command.execute();
    }
    else if (operation.compare(0, 12, "-environment") == 0)
    {
        Environment command(*this);
        *this = command.execute();
    }
    else if (operation.compare(0, 5, "-file") == 0)
    {
        File command(*this);
        *this = command.execute();
    }
    else if (operation.compare(0, 4, "-gdb") == 0)
    {
        Gdb command(*this);
        *this = command.execute();
    }
    else if (operation.compare(0, 5, "-list") == 0)
    {
        List command(*this);
        *this = command.execute();
    }
    else if (operation.compare(0, 12, "-interpreter") == 0)
    {
        Interpreter command(*this);
        *this = command.execute();
    }
    return *this;
}

MICommand & MICommand::operator=(const MICommand & right)
{
    this->resultClass = right.resultClass;
    this->results = right.results;
    return *this;
}

void MICommand::parse(const std::string & commandLine)
{
    std::size_t i = 0;
    bool expectOptions = true;
    bool expectDblDash = true;
    if (i < commandLine.length() && isdigit(commandLine.at(i)))
        while (i < commandLine.length() && isdigit(commandLine.at(i)))
            token.append(1, commandLine.at(i++));
    if (i < commandLine.length() && commandLine.at(i) == '-')
        while (i < commandLine.length() &&  commandLine.at(i) != ' ')
            operation.append(1, commandLine.at(i++));

    Option currentOption;
    std::string currentParameter;
    enum {
        START,
        START_OPTION_NAME,
        START_OPTION_NAME2,
        START_OPTION_PARAMETER,
        START_PARAMETER,
        OPTION_NAME,
        OPTION_PARAMETER,
        OPTION_NAME_CSTRING,
        OPTION_PARAMETER_CSTRING,
        OPTION_NAME_CSTRING_ESC,
        OPTION_PARAMETER_CSTRING_ESC,
        PARAMETER,
        CSTRING,
        CSTRING_ESC,
        END
    } state;
    for (state = i == commandLine.length() ? END : START; state != END; ++i)
    {
        if (i == commandLine.length())
        {
            switch (state)
            {
            case OPTION_NAME:
            case OPTION_NAME_CSTRING:
            case OPTION_NAME_CSTRING_ESC:
            case OPTION_PARAMETER:
            case OPTION_PARAMETER_CSTRING:
            case OPTION_PARAMETER_CSTRING_ESC:
            case START_OPTION_PARAMETER:
                options.push_back(currentOption);
                break;
            case PARAMETER:
            case CSTRING:
            case CSTRING_ESC:
                parameters.push_back(currentParameter);
                break;

            }
            state = END;
        }
        switch (state)
        {
        case START:
            switch (commandLine.at(i))
            {
            case ' ':
                break;
            case '-':
                currentOption.clear();
                currentOption.name.push_back(commandLine.at(i));
                state = START_OPTION_NAME;
                break;
            case '"':
                currentParameter.clear();
                currentParameter.push_back(commandLine.at(i));
                state = CSTRING;
                break;
            default:
                currentParameter.clear();
                currentParameter.push_back(commandLine.at(i));
                state = PARAMETER;
            }
            break;
        case START_OPTION_NAME:
            switch (commandLine.at(i))
            {
            case ' ': // "- "
                state = START;
                break;
            case '-': // "--"
                currentOption.name.push_back(commandLine.at(i));
                state = START_OPTION_NAME2;
                break;
            case '"': // "-""
                currentOption.name.push_back(commandLine.at(i));
                state = OPTION_NAME_CSTRING;
                break;
            default:  // "-c"
                currentOption.name.push_back(commandLine.at(i));
                state = OPTION_NAME;
            }
            break;
        case START_OPTION_NAME2:
            switch (commandLine.at(i))
            {
            case ' ': // "-- "
                state = START_PARAMETER;
                break;
            default:  // "--c"
                currentOption.name.push_back(commandLine.at(i));
                state = OPTION_NAME;
            }
            break;
        case START_OPTION_PARAMETER:
            switch (commandLine.at(i))
            {
            case ' ':
                break;
            case '-':
                options.push_back(currentOption);
                currentOption.clear();
                currentOption.parameter.push_back(commandLine.at(i));
                state = START_OPTION_NAME;
                break;
            case '"':
                currentOption.parameter.push_back(commandLine.at(i));
                state = OPTION_PARAMETER_CSTRING;
                break;
            default:
                currentOption.parameter.push_back(commandLine.at(i));
                state = OPTION_PARAMETER;
            }
            break;
        case START_PARAMETER:
            currentParameter.clear();
            switch (commandLine.at(i))
            {
            case ' ':
                break;
            case '"':
                currentParameter.push_back(commandLine.at(i));
                state = CSTRING;
                break;
            default:
                currentParameter.push_back(commandLine.at(i));
                state = PARAMETER;
            }
            break;
        case PARAMETER:
            switch (commandLine.at(i))
            {
            case ' ':
                parameters.push_back(currentParameter);
                state = START_PARAMETER;
                break;
            default:
                currentParameter.push_back(commandLine.at(i));
            }
            break;
        case CSTRING:
            switch (commandLine.at(i))
            {
            case '\\':
                state = CSTRING_ESC;
                break;
            case '"':
                currentParameter.push_back(commandLine.at(i));
                parameters.push_back(currentParameter);
                state = START_PARAMETER;
                break;
            default:
                currentParameter.push_back(commandLine.at(i));
            }
            break;
        case CSTRING_ESC:
            switch (commandLine.at(i))
            {
            case '"':
                break;
            default:
                currentParameter.push_back('\\');
            }
            currentParameter.push_back(commandLine.at(i));
            state = CSTRING;
            break;
        case OPTION_NAME:
            switch (commandLine.at(i))
            {
            case ' ':
                state = START_OPTION_PARAMETER;
                break;
            default:
                currentOption.name.push_back(commandLine.at(i));
            }
            break;
        case OPTION_NAME_CSTRING:
            switch (commandLine.at(i))
            {
            case '\\':
                state = OPTION_NAME_CSTRING_ESC;
                break;
            case '"':
                currentOption.name.push_back(commandLine.at(i));
                state = START_OPTION_PARAMETER;
                break;
            default:
                currentOption.name.push_back(commandLine.at(i));
            }
            break;
        case OPTION_NAME_CSTRING_ESC:
            switch (commandLine.at(i))
            {
            case '"':
                break;
            default:
                currentParameter.push_back('\\');
            }
            currentParameter.push_back(commandLine.at(i));
            state = OPTION_NAME_CSTRING;
            break;
        case OPTION_PARAMETER:
            switch (commandLine.at(i))
            {
            case ' ':
                options.push_back(currentOption);
                state = START;
                break;
            default:
                currentOption.parameter.push_back(commandLine.at(i));
            }
            break;
        case OPTION_PARAMETER_CSTRING:
            switch (commandLine.at(i))
            {
            case '\\':
                state = OPTION_PARAMETER_CSTRING_ESC;
                break;
            case '"':
                currentOption.parameter.push_back(commandLine.at(i));
                options.push_back(currentOption);
                state = START;
                break;
            default:
                currentOption.parameter.push_back(commandLine.at(i));
            }
            break;
        case OPTION_PARAMETER_CSTRING_ESC:
            switch (commandLine.at(i))
            {
            case '"':
                break;
            default:
                currentOption.parameter.push_back('\\');
            }
            currentOption.parameter.push_back(commandLine.at(i));
            state = OPTION_PARAMETER_CSTRING;
            break;
        }
    }
}

void MICommand::setError(const std::string & msg)
{
    resultClass = ResultClass::error;
    Result error("msg");
    CString cstring(msg);
    error.value = cstring.toString();
    results.push_back(error);
}

std::string MICommand::getOutput() const
{
    std::ostringstream strstr;
    if (!token.empty())
        strstr << token;
    strstr << "^" << resultClass;
    for (const Result & result : results)
        strstr << "," << result;
    return strstr.str();
}

} // namespace lldbmi

std::ostream & operator<<(std::ostream & out, const lldbmi::Result & result)
{
    return out << result.variabe << "=" << result.value;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::Option & option)
{
    out << option.name;
    if (!option.parameter.empty())
        out << " " << option.parameter;
    return out;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::MICommand & command)
{
    if (!command.token.empty())
        out << command.token;
    out << command.operation;
    for (const lldbmi::Option & option : command.options)
        out << " " << option;
    if (!command.parameters.empty())
    {
        out << " --";
        for (const std::string & parameter : command.parameters)
            out << " " << parameter;
    }
    return out;
}
