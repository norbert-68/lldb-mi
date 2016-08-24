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
#ifndef LLDBMI_MITARGET_HPP
#define LLDBMI_MITARGET_HPP

#include <lldbmi/MITypes.hpp>
#include <lldb/API/SBTarget.h>
#include <string>

namespace lldbmi {

class MITarget
{
public:

    Language language;

    MITarget(lldb::SBDebugger & debugger, const lldb::SBTarget & target, const std::string & threadGroup);

    MITarget(const MITarget&) = delete;

    virtual ~MITarget();

    const std::string & getExecutable() const
    {
        return executable;
    }

    const std::string & getThreadGroup() const
    {
        return threadGroup;
    }

    lldb::SBTarget & getTarget()
    {
        return target;
    }

    MITarget & operator=(const MITarget&) = delete;

private:

    lldb::SBDebugger & debugger;
    lldb::SBTarget target;
    std::string threadGroup;
    std::string executable;

};

typedef std::unique_ptr<MITarget> MITargetPtr;

} // namespace lldbmi

#endif // LLDBMI_MITARGET_HPP
