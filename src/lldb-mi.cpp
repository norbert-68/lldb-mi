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

#include <lldbmi/Interpreter.hpp>
#include <iostream>
#include <cstdlib>

int main(int argc, char * args[])
{
	try
	{
		lldbmi::Interpreter miInterpreter;
		miInterpreter.start(argc, args, std::cin, std::cout);
	}
	catch (const std::exception & exception)
	{
		std::cerr << "error: " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const char * exception)
	{
		std::cerr << "error: " << exception << std::endl;
		return EXIT_FAILURE;
	}
	catch (int exception)
	{
		std::cerr << "error: " << exception << std::endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "unknown error" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
