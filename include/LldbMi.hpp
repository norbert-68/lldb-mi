//******************************************************************************
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
#ifndef LLDB_MI_HPP
#define LLDB_MI_HPP

#include <lldb/API/SBDebugger.h>
#include <memory>
#include <ostream>

namespace lldbmi {

class LldbMiInterpreter
{
public:

	LldbMiInterpreter() :
	    outStream(0),
		modifiedArgc(0)
	{
	}

	std::ostream & getLog() { return *logStream; }
    std::ostream & getOut() { return *outStream; }
    static std::string getTime();

	bool hasLog() const     { return static_cast<bool>(logStream); }

	static int parseOptions(const char * option, int argc, char * args[], std::string * value = 0);

	void start(int argc, char * args[]);

private:

	std::ostream * outStream;
	std::unique_ptr<std::ostream> logStream;
	std::string logFilename;
	int modifiedArgc;
	std::string interpreter;

};

} // lldbmi

#endif // LLDB_MI_HPP
