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
#ifndef LLDBMI_COMMAND_LIST_HPP
#define LLDBMI_COMMAND_LIST_HPP

#include "../MICommand.hpp"
#include <lldbmi/MIInterpreter.hpp>
#include <lldb/API/SBProcess.h>
#include <set>

namespace lldbmi {

/**
 * @brief Implements
 * -list-features
 * -list-thread-groups [ --available ] [ --recurse 1 ] [ group ... ]
 */
struct List : public MICommand
{
    List(const MICommand & command) :
        MICommand(command)
    {
    }

    virtual MICommand & execute()
    {
        if (operation.compare("-list-features") == 0)
        {
            ResultList featureList;
            Result result;
            result.value = featureList.toString();
            results.push_back(result);
        }
        else if (operation.compare("-list-thread-groups") == 0)
        {
            long recursionDepth = 0;
            bool available = false;

            for (const Option & option :options)
            {
                if (option.name.compare("--available") == 0)
                {
                    available = true;
                }
                else if (option.name.compare("--recurse") == 0)
                {
                    char * endptr = nullptr;
                    recursionDepth = strtol(option.parameter.c_str(), &endptr, 10);
                    recursionDepth = std::max(recursionDepth, 1L);
                }
            }

            if (parameters.empty() == 1)
            {
                ResultList groups;
                for (const MIInterpreter::MITargets::value_type & ptarget : interpreter.getTargets())
                {
                    ResultTuple group;
                    group.push_back(Result("id", CString(ptarget.second->getThreadGroup())));
                    group.push_back(Result("type", CString("process")));
                    group.push_back(Result("executable", CString(ptarget.second->getExecutable())));
                    if (ptarget.second->getTarget().GetProcess().IsValid())
                    {
                        lldb::SBProcess process = ptarget.second->getTarget().GetProcess();
                        group.push_back(Result("num_children", CString(process.GetNumThreads())));
                    }
                    groups.push_back(group);
                }
                results.push_back(Result("groups", groups));
            }
            else if (parameters.size() == 1)
            {
                ResultList threads;
                //todo
                results.push_back(Result("threads", threads));
            }
            else
            {
                ResultList groups;
                for (const std::string & parameter : parameters)
                {
                    MITarget * ptarget = interpreter.findTarget(parameter);
                    if (ptarget)
                    {
                        ResultTuple group;
                        group.push_back(Result("id", CString(ptarget->getThreadGroup())));
                        group.push_back(Result("type", CString("process")));
                        group.push_back(Result("type", CString(ptarget->getTarget().GetExecutable().GetFilename())));
                        if (ptarget->getTarget().GetProcess().IsValid())
                        {
                            lldb::SBProcess process = ptarget->getTarget().GetProcess();
                            group.push_back(Result("num_children", CString(process.GetNumThreads())));
                        }
                        groups.push_back(group);
                    }
                }
                results.push_back(Result("groups", groups));
            }
        }
        return *this;
    }
};

} // namespace lldbmi

#endif // LLDBMI_COMMAND_LIST_HPP
