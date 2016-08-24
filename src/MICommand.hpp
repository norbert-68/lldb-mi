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
#ifndef LLDBMI_MICOMMAND_HPP
#define LLDBMI_MICOMMAND_HPP

#include <lldbmi/MITypes.hpp>
#include <string>
#include <vector>

namespace lldbmi {

// Forward Declaration
class MIInterpreter;

class CString
{
public:

    CString(const std::string & str) :
        value(str) {}

    CString(const char * str) :
        value(str) {}

    CString(unsigned long val);

    operator std::string () const
    {
        return toString();
    }

    std::string toString() const;

private:

    std::string value;

};

struct Result
{
    Result(const char * variabe = "result") :
        variabe(variabe) {}

    Result(const char * variabe, const std::string & value) :
        variabe(variabe),
        value(value) {}

    operator std::string() const
    {
        return toString();
    }

    std::string toString() const;

    std::string variabe;
    std::string value;
};

struct ResultVector : public std::vector<std::string>
{
public:

    ResultVector(char open, char close) : open(open), close(close) {}

    operator std::string() const
    {
        return toString();
    }

    std::string toString() const;

protected:

    char open;
    char close;
};

struct ResultList : public ResultVector
{
    ResultList() : ResultVector('[', ']') {}
};

struct ResultTuple : public ResultVector
{
    ResultTuple() : ResultVector('{', '}') {}
};

struct Option
{
    std::string name;
    std::string parameter;

    void clear()
    {
        name.clear();
        parameter.clear();
    }

    bool empty() const
    {
        return name.empty() && parameter.empty();
    }
};

class MICommand
{
public:

    std::string token;
    std::string operation;
    std::vector<Option> options;
    std::vector<std::string> parameters;

    MICommand(MIInterpreter & interpreter) :
        interpreter(interpreter),
        resultClass(ResultClass::done) {}

    virtual ~MICommand() {}

    /**
     * @brief Returns @c true in case execution should go on. @c false otherwise.
     */
    virtual MICommand & execute();

    std::string getOutput() const;

    const ResultClass & getResultClass() const
    {
        return resultClass;
    }

    MICommand & operator=(const MICommand & right);

    void parse(const std::string & commandLine);

    void setError(const std::string & msg);

protected:

    MIInterpreter & interpreter;
    ResultClass resultClass;
    std::vector<Result> results;

};

} // namespce lldbmi

std::ostream & operator<<(std::ostream & out, const lldbmi::Result & result);
std::ostream & operator<<(std::ostream & out, const lldbmi::Option & option);
std::ostream & operator<<(std::ostream & out, const lldbmi::MICommand & command);

#endif // LLDBMI_MICOMMAND_HPP
