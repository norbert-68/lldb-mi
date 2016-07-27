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
#ifndef LLDBMI_COMMAND_HPP
#define LLDBMI_COMMAND_HPP

#include <string>
#include <vector>

namespace lldbmi {

struct Option
{
    std::string name;
    std::string parameter;
};

struct Command
{
    void parse(const std::string & commandLine);

    std::string token;
    std::string operation;
    std::vector<Option> options;
    std::vector<std::string> parameters;
};

} // namespce lldbmi

std::ostream & operator<<(std::ostream & out, const lldbmi::Option & option);
std::ostream & operator<<(std::ostream & out, const lldbmi::Command & command);

#endif // LLDBMI_COMMAND_HPP
