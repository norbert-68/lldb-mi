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
#ifndef LLDBMI_LIST_FEATURES_HPP
#define LLDBMI_LIST_FEATURES_HPP

#include "../Command.hpp"
#include <lldbmi/Interpreter.hpp>

namespace lldbmi {

struct ListFeatures : public Command
{
    ListFeatures(const Command & command) :
        Command(command) {}

    virtual void execute()
    {
        ResultList featureList;
        Result result;
        result.value = featureList.toString();
        results.push_back(result);
        writeOutput();
    }
};

} // namespace lldbmi

#endif // LLDBMI_LIST_FEATURES_HPP
