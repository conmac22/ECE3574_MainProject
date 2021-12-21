//System includes
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>

//Module includes
#include "interpreter.hpp"
#include "interpreter_semantic_error.hpp"

//Main slisp program that takes command-line or file program input and displays
//the result. Can also be configured to run a REPL interactive version.
//
//Invocation: Simple program > slisp -e "[program]"
//              File program > slisp [filename]
//                      REPL > slisp
int main(int argc, char *argv[])
{
  //REPL
  if (argc == 1)
  {
    Interpreter interp;

    //One line of user input
    std::string input;

    //First prompt
    std::cout << "slisp> ";

    //Keep asking for user input until EOF is entered
    while (std::getline(std::cin, input))
    {
      std::istringstream program(input);

      if (!interp.parse(program))
      {
        std::cout << "Error: Could not parse program" << std::endl;
      }

      try
      {
        Expression result = interp.eval();
        //Display result based on Expression type
        if (result.head.type == BooleanType)
        {
          //True
          if (result.head.value.bool_value)
          {
            std::cout << "(" << "True" << ")" << std::endl;
          }
          //False
          else
          {
            std::cout << "(" << "False" << ")" << std::endl;
          }
        }
        else if (result.head.type == NumberType)
        {
          std::cout << "(" << result.head.value.num_value << ")" << std::endl;
        }
        //Debugging
        else
        {
          std::cout << "Error: Result is of incorrect type" << std::endl;
        }
      }
      catch (InterpreterSemanticError& err)
      {
        std::cout << "Error: " << err.what() << std::endl;
      }

      //Display prompt
      std::cout << "slisp> ";
    }
  }

  //File input
  else if (argc == 2)
  {
    std::ifstream program(argv[1]);

    //Throw error if file cannot be read
    if (!program.is_open())
    {
      std::cout << "Error: Cannot read program file" << std::endl;
      return EXIT_FAILURE;
    }

    Interpreter interp;

    if (!interp.parse(program))
    {
      std::cout << "Error: Could not parse program" << std::endl;
      return EXIT_FAILURE;
    }

    try
    {
      Expression result = interp.eval();
      //Display result based on Expression type
      if (result.head.type == BooleanType)
      {
        //True
        if (result.head.value.bool_value)
        {
          std::cout << "(" << "True" << ")" << std::endl;
        }
        //False
        else
        {
          std::cout << "(" << "False" << ")" << std::endl;
        }
      }
      else if (result.head.type == NumberType)
      {
        std::cout << "(" << result.head.value.num_value << ")" << std::endl;
      }
      //Debugging
      else
      {
        std::cout << "Error: Result is of incorrect type" << std::endl;
        return EXIT_FAILURE;
      }
    }
    catch (InterpreterSemanticError& err)
    {
      std::cout << "Error: " << err.what() << std::endl;
      return EXIT_FAILURE;
    }
  }

  //Simple program from command line
  else if (argc == 3)
  {
    //First argument should be "-e"
    if (strcmp(argv[1], "-e") != 0)
    {
      std::cout << "Error: Invocation: slisp -e \"[program]\"\n\t\t   slisp [filename]\n\t\t   slisp" << std::endl;
      return EXIT_FAILURE;
    }

    std::string programStr = argv[2];
    std::istringstream program(programStr);

    Interpreter interp;

    if (!interp.parse(program))
    {
      std::cout << "Error: Could not parse program" << std::endl;
      return EXIT_FAILURE;
    }

    try
    {
      Expression result = interp.eval();
      //Display result based on Expression type
      if (result.head.type == BooleanType)
      {
        //True
        if (result.head.value.bool_value)
        {
          std::cout << "(" << "True" << ")" << std::endl;
        }
        //False
        else
        {
          std::cout << "(" << "False" << ")" << std::endl;
        }
      }
      else if (result.head.type == NumberType)
      {
        std::cout << "(" << result.head.value.num_value << ")" << std::endl;
      }
      //Debugging
      else
      {
        std::cout << "Error: Result is of incorrect type" << std::endl;
        return EXIT_FAILURE;
      }
    }
    catch (InterpreterSemanticError& err)
    {
      std::cout << "Error: " << err.what() << std::endl;
      return EXIT_FAILURE;
    }
  }

  //Incorrect invocation
  else
  {
    std::cout << "Error: Invocation: slisp -e \"[program]\"\n\t\t   slisp [filename]\n\t\t   slisp" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
