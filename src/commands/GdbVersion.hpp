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
#ifndef LLDBMI_GDB_VERSION_HPP
#define LLDBMI_GDB_VERSION_HPP

#include "../Command.hpp"
#include <lldbmi/Interpreter.hpp>

namespace lldbmi {

struct GdbVersion : public Command
{
    GdbVersion(Interpreter & interpreter) :
        Command(interpreter)
    {
        init();
    }

    GdbVersion(const Command & command) :
        Command(command)
    {
        init();
    }

    virtual void execute()
    {
        for (const std::string & version : getVersion())
        {
            std::string consoleStreamOutput("~");
            consoleStreamOutput.append(version);
            interpreter.addOutOfBandRecord(consoleStreamOutput);
        }
        writeOutput();
    }

    const std::vector<std::string> & getVersion() const
    {
        return version;
    }

protected:

    void init()
    {
        version.push_back("GNU gdb (GDB) 7.6");
        version.push_back("A MI command line interface for LLDB.");
        version.push_back("All rights reserved.");
    }

    std::vector<std::string> version;

};

} // namespace lldbmi

#endif // LLDBMI_GDB_VERSION_HPP
