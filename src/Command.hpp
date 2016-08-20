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
#ifndef LLDBMI_COMMAND_HPP
#define LLDBMI_COMMAND_HPP

#include <string>
#include <vector>

namespace lldbmi {

// Forward Declaration
class Interpreter;

struct CString : public std::string
{
    CString(const std::string & str) :
        std::string(str) {}

    CString(const char * str) :
        std::string(str) {}

    std::string toString() const;
};

struct Result
{
    Result(const char * variabe = "result") :
        variabe(variabe) {}

    std::string variabe;
    std::string value;
};

struct ResultVector : public std::vector<std::string>
{
public:

    ResultVector(char open, char close) : open(open), close(close) {}

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

class Command
{
public:

    typedef enum { DONE, CONNECTED, ERROR, EXIT } ResultClass;

    std::string token;
    std::string operation;
    std::vector<Option> options;
    std::vector<std::string> parameters;

    Command(Interpreter & interpreter) :
        interpreter(interpreter),
        resultClass(DONE) {}

    virtual ~Command() {}

    /**
     * @brief Returns @c true in case execution should go on. @c false otherwise.
     */
    virtual Command & execute();

    std::string getOutput() const;

    const ResultClass & getResultClass() const
    {
        return resultClass;
    }

    Command & operator=(const Command & right);

    void parse(const std::string & commandLine);

    void setError(const std::string & msg);

protected:

    Interpreter & interpreter;
    ResultClass resultClass;
    std::vector<Result> results;

};

} // namespce lldbmi

std::ostream & operator<<(std::ostream & out, const lldbmi::Result & result);
std::ostream & operator<<(std::ostream & out, const lldbmi::Command::ResultClass & resultClass);
std::ostream & operator<<(std::ostream & out, const lldbmi::Option & option);
std::ostream & operator<<(std::ostream & out, const lldbmi::Command & command);

#endif // LLDBMI_COMMAND_HPP
