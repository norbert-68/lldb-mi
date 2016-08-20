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
#ifndef LLDBMI_INTERPRETER_HPP
#define LLDBMI_INTERPRETER_HPP

#include <lldb/API/SBDebugger.h>
#include <memory>
#include <ostream>

namespace lldbmi {

// forward declarations
struct Command;

class Interpreter
{
public:

    bool breakpoint_pending;
    bool detach_on_fork;

    static const char * endl;

	Interpreter() :
	    breakpoint_pending(true),
	    detach_on_fork(true),
        in(0),
	    out(0),
		modifiedArgc(0)
	{
	}

    void addOutOfBandRecord(const std::string & record);

    std::istream & getIn()  { return *in; }
	std::ostream & getLog() { return *log; }
    std::ostream & getOut() { return *out; }

    static std::string getTime();

	bool hasLog() const { return static_cast<bool>(log); }

	static int parseOptions(const char * option, int argc, char * args[], std::string * value = 0);


	void start(int argc, char * args[], std::istream & in, std::ostream & out);

protected:

    void readEvalLoop();
    void writeOutput(std::ostream & stream, const std::string & resultRecord);

private:

    std::istream * in;
	std::ostream * out;
	std::unique_ptr<std::ostream> log;
	std::string logFilename;
	int modifiedArgc;
	std::string interpreter;
    std::vector<std::string> outOfBandRecords;
    std::auto_ptr<lldb::SBDebugger> debugger;

};

} // lldbmi

#endif // LLDBMI_INTERPRETER_HPP
