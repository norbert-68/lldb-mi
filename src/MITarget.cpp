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

#include <lldb/API/SBDebugger.h>
#include <lldb/API/SBFileSpec.h>
#include <lldbmi/MITarget.hpp>

namespace lldbmi {

MITarget::MITarget(lldb::SBDebugger & debugger, const lldb::SBTarget & target, const std::string & threadGroup) :
    language(language_auto),
    debugger(debugger),
    target(target),
    threadGroup(threadGroup)
{
    uint32_t executableLength = this->target.GetExecutable().GetPath(&executable.front(), 0);
    if (executableLength)
    {
        executable.resize(executableLength);
        this->target.GetExecutable().GetPath(&executable.front(), executableLength+1);
    }
}

MITarget::~MITarget()
{
    debugger.DeleteTarget(target);
}

} // namespace lldbmi
