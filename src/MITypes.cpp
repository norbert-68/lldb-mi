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

#include <lldbmi/MITypes.hpp>

namespace lldbmi {

} // namespace lldbmi

std::ostream & operator<<(std::ostream & out, const lldbmi::Flag & var)
{
    switch (var)
    {
    case lldbmi::flag_on:  return out << "on";
    case lldbmi::flag_off: return out << "off";
    }
    return out << "auto";
}

std::string & operator>>(std::string & str, lldbmi::Flag & var)
{
    if (str.compare("on") == 0)
    {
        var = lldbmi::flag_on;
    }
    else if (str.compare("off") == 0)
    {
        var = lldbmi::flag_off;
    }
    else
    {
        var = lldbmi::flag_auto;
    }
    return str;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::PythonPrintStack & var)
{
    switch (var)
    {
    case lldbmi::python_print_stack_message: return out << "message";
    case lldbmi::python_print_stack_full:    return out << "full";
    }
    return out << "none";
}

std::string & operator>>(std::string & str, lldbmi::PythonPrintStack & var)
{
    if (str.compare("full") == 0)
    {
        var = lldbmi::python_print_stack_full;
    }
    else if (str.compare("message") == 0)
    {
        var = lldbmi::python_print_stack_message;
    }
    else
    {
        var = lldbmi::python_print_stack_none;
    }
    return str;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::Charset & var)
{
    switch (var)
    {
    case lldbmi::charset_utf8:  return out << "UTF-8";
    case lldbmi::charset_utf16: return out << "UTF-16";
    case lldbmi::charset_utf32: return out << "UTF-32";
    }
    return out << "default";
}

std::string & operator>>(std::string & str, lldbmi::Charset & var)
{
    if (str.compare("UTF-8") == 0)
    {
        var = lldbmi::charset_utf8;
    }
    else if (str.compare("UTF-16") == 0)
    {
        var = lldbmi::charset_utf16;
    }
    else if (str.compare("UTF-32") == 0)
    {
        var = lldbmi::charset_utf32;
    }
    else
    {
        var = lldbmi::charset_default;
    }
    return str;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::Language & var)
{
    switch (var)
    {
    case lldbmi::language_c: return out << "c";
    }
    return out << "auto";
}

std::string & operator>>(std::string & str, lldbmi::Language & var)
{
    if (str.compare("c") == 0)
    {
        var = lldbmi::language_c;
    }
    else
    {
        var = lldbmi::language_auto;
    }
    return str;
}

std::ostream & operator<<(std::ostream & out, const lldbmi::ResultClass & resultClass)
{
    switch (resultClass)
    {
    case lldbmi::connected:
        return out << "connected";
    case lldbmi::error:
        return out << "error";
    case lldbmi::exit:
        return out << "exit";
    }
    return out << "done";
}
