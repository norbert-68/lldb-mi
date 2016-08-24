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
#ifndef LLDBMI_MITYPES_HPP
#define LLDBMI_MITYPES_HPP

#include <ostream>
#include <sstream>
#include <string>

namespace lldbmi {

typedef enum { python_print_stack_none, python_print_stack_message, python_print_stack_full } PythonPrintStack;
typedef enum { charset_default, charset_utf8, charset_utf16, charset_utf32 } Charset;
typedef enum { flag_on, flag_off, flag_auto } Flag;
typedef enum { language_auto, language_c } Language;
typedef enum { done, connected, error, exit } ResultClass;

} // namespace lldbmi

std::ostream & operator<<(std::ostream & out, const lldbmi::Flag & var);
std::string & operator>>(std::string & str, lldbmi::Flag & var);

std::ostream & operator<<(std::ostream & out, const lldbmi::PythonPrintStack & var);
std::string & operator>>(std::string & str, lldbmi::PythonPrintStack & var);

std::ostream & operator<<(std::ostream & out, const lldbmi::Charset & var);
std::string & operator>>(std::string & str, lldbmi::Charset & var);

std::ostream & operator<<(std::ostream & out, const lldbmi::Language & var);
std::string & operator>>(std::string & str, lldbmi::Language & var);

std::ostream & operator<<(std::ostream & out, const lldbmi::ResultClass & resultClass);

template<typename Type> std::string toString(Type type)
{
    std::ostringstream strstr;
    strstr << type;
    return strstr.str();
}

#endif // LLDBMI_MITARGET_HPP
