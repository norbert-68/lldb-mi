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
#ifndef LLDBMI_MIINTERPRETER_HPP
#define LLDBMI_MIINTERPRETER_HPP

#include <lldbmi/MITypes.hpp>
#include <lldbmi/MITarget.hpp>
#include <lldb/API/SBDebugger.h>
#include <map>
#include <memory>
#include <mutex>

namespace lldbmi {

class MIInterpreter : public std::recursive_mutex
{
public:

    typedef std::map<std::string, MITargetPtr> MITargets;

    Flag sourc_init_files;
    Flag breakpoint_pending;
    Flag detach_on_fork;
    Flag enable_pretty_printing;
    PythonPrintStack python_print_stack;
    Flag print_object;
    Flag print_sevenbit_strings;
    Charset host_charset;
    Charset target_charset;
    Charset target_wide_charset;
    Flag target_async;
    Flag pagination;
    Flag non_stop;
    Flag auto_solib_add;
    Language language;

    static const char * endl;

	MIInterpreter() :
	    sourc_init_files(flag_auto),
	    breakpoint_pending(flag_auto),
	    detach_on_fork(flag_auto),
	    enable_pretty_printing(flag_auto),
	    python_print_stack(python_print_stack_none),
	    print_object(flag_auto),
	    print_sevenbit_strings(flag_auto),
	    host_charset(charset_default),
	    target_charset(charset_default),
	    target_wide_charset(charset_default),
	    target_async(flag_auto),
	    pagination(flag_auto),
	    non_stop(flag_auto),
	    auto_solib_add(flag_auto),
	    language(language_auto),
        in(nullptr),
	    out(nullptr),
		modifiedArgc(0)
	{
	}

	virtual ~MIInterpreter();

	void addConsoleStreamOutput(const std::string & record);

    void addOutOfBandRecord(const std::string & record);

    void addTarget(MITargetPtr && target);

    MITarget * findTarget(const std::string & threadGroup);

    lldb::SBDebugger & getDebugger();

    std::istream & getIn()  { return *in; }
	std::ostream & getLog() { return *log; }
    std::ostream & getOut() { return *out; }

    const MITargets & getTargets() const
    {
        return targets;
    }

    static std::string getTime();

	bool hasLog() const { return static_cast<bool>(log); }

	static int parseOptions(const char * option, int argc, char * args[], std::string * value = nullptr);

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
    std::unique_ptr<lldb::SBDebugger> debugger;
    MITargets targets;

};

} // lldbmi

#endif // LLDBMI_MIINTERPRETER_HPP
