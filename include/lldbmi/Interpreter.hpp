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
#include <mutex>
#include <ostream>

namespace lldbmi {

// forward declarations
struct Command;

typedef enum { python_print_stack_none, python_print_stack_message, python_print_stack_full } PythonPrintStack;
typedef enum { charset_default, charset_utf8, charset_utf16, charset_utf32 } Charset;
typedef enum { flag_on, flag_off, flag_auto } Flag;

class Interpreter : public std::recursive_mutex
{
public:

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

    static const char * endl;

	Interpreter() :
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
        in(0),
	    out(0),
		modifiedArgc(0)
	{
	}

    void addOutOfBandRecord(const std::string & record);

    lldb::SBDebugger & getDebugger();

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
    std::unique_ptr<lldb::SBDebugger> debugger;

};

} // lldbmi

std::ostream & operator<<(std::ostream & out, const lldbmi::Flag & var);
std::string & operator>>(std::string & str, lldbmi::Flag & var);

std::ostream & operator<<(std::ostream & out, const lldbmi::PythonPrintStack & var);
std::string & operator>>(std::string & str, lldbmi::PythonPrintStack & var);

std::ostream & operator<<(std::ostream & out, const lldbmi::Charset & var);
std::string & operator>>(std::string & str, lldbmi::Charset & var);

#endif // LLDBMI_INTERPRETER_HPP
