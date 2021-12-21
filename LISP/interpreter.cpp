//System includes
#include <stack>
#include <stdexcept>
#include <iostream>

//Module includes
#include "interpreter.hpp"
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"

//Parses the isteam into a valid AST
//
//Pre: expression is a valid istream object
//Post: env contains a valid AST
//Return: true if the the program could be parsed and false otherwise.
bool Interpreter::parse(std::istream& expression) noexcept
{
  TokenSequenceType tokens = tokenize(expression);

  //Check to make sure there are tokens in the stream
  if (tokens.empty())
  {
    return false;
  }

  std::string firstToken = tokens.front();
  tokens.pop_front();

  //Single token in the stream
  if (firstToken.compare("(") != 0)
  {
    //Should only be one token in the stream if first token is not "("
    if (!tokens.empty())
    {
      return false;
    }

    Atom head;
    //Valid token
    if (token_to_atom(firstToken, head))
    {
      ast.head = head;
      return true;
    }
    //Invalid token
    return false;
  }

  //Build the AST
  ast = parseUtility(firstToken, tokens);

  //If an empty Expression is returned or all the tokens have not been processed
  //then the AST could not be built
  return !(ast == Expression() || !tokens.empty());
};

//Utility function for parse. Recursively builds the AST
//
//Pre: token is a valid string and tokens is a valid TokenSequenceType
//Post: Returns a single Expression containing the AST at the end of recursion
//Return: A single Expression containing the AST at the end of recursion
Expression Interpreter::parseUtility(std::string token, TokenSequenceType& tokens)
{
  //Base Expression
  Expression e;

  //Build the AST recursively
  while (true)
  {
    //Should not be empty until the AST is built
    if (tokens.empty())
    {
      return Expression();
    }

    std::string currentToken = tokens.front();
    tokens.pop_front();

    if (currentToken.compare("(") == 0)
    {
      //There should not be two "(" in a row
      if (token.compare("(") == 0)
      {
        return Expression();
      }

      Expression newExpression = parseUtility(currentToken, tokens);

      if (e != Expression())
      {
        e.tail.push_back(newExpression);
        token = ")";
      }
    }

    //End of current Expression
    else if (currentToken.compare(")") == 0)
    {
      return e;
    }

    else
    {
      if (token.compare("(") == 0)
      {
        Atom head;

        if (token_to_atom(currentToken, head))
        {
          e.head = head;
          token = currentToken;
        }
        //Invalid token
        else
        {
          return Expression();
        }
      }

      else
      {
        Atom a;
        Expression exp;

        if (token_to_atom(currentToken, a))
        {
          token = currentToken;
          exp.head = a;
          e.tail.push_back(exp);
        }
        else
        {
          return Expression();
        }
      }
    }
  }
}

//Evaluates the program and returns the result in an Expression
//
//Pre: parse was called
//Post: The program is evaluated and the result is returned
//Return: The result of the program in the form of an Expression
Expression Interpreter::eval()
{
  return evalUtility(ast);
}

//Utility function for eval. Recursively evalues the program
//
//Pre: exp is a valid Expression
//Post: Returns a single Expression containing the result of the program at the
//      end of recursion
//Return: A single Expression containing the result of the program at the end of
//        recursion
Expression Interpreter::evalUtility(Expression& exp)
{
  //Could be special form, symbol, or procedure
  if (exp.head.type == SymbolType)
  {
    //Start special forms

    //Begin
    if (exp.head.value.sym_value.compare("begin") == 0)
    {
      Expression finalExp;
      //Iterate through the Expressions and return the last Expression
      for (int i = 0; i < exp.tail.size(); i++)
      {
        finalExp = evalUtility(exp.tail[i]);
      }
      return finalExp;
    }

    //Define
    if (exp.head.value.sym_value.compare("define") == 0)
    {
      if (exp.tail.empty())
      {
        throw InterpreterSemanticError("No arguments for define");
      }
      std::vector<Atom> args;
      //First argument is the symbol after "define"
      args.push_back(exp.tail[0].head);
      Expression arg2 = evalUtility(exp.tail[1]);
      //Second argument should be evaluated to a symbol
      if (!arg2.tail.empty() || arg2.head.type == SymbolType)
      {
        throw InterpreterSemanticError("Second argument for define is empty or is a symbol");
      }
      args.push_back(arg2.head);
      return env.updateMap(args);
    }

    //If
    if (exp.head.value.sym_value.compare("if") == 0)
    {
      if (exp.tail.empty())
      {
        throw InterpreterSemanticError("No arguments for if");
      }
      Expression arg1 = evalUtility(exp.tail[0]);
      //First argument should be a boolean atom
      if (!arg1.tail.empty() || arg1.head.type != BooleanType)
      {
        throw InterpreterSemanticError("First argument for if should be a boolean atom");
      }
      //If Expression 1 evaluates to true, return Expression 2
      if (arg1.head.value.bool_value)
      {
        return evalUtility(exp.tail[1]);
      }
      //If Expression 1 evaluates to false, return Expression 3
      return evalUtility(exp.tail[2]);
    }

    //Draw
    if (exp.head.value.sym_value.compare("draw") == 0)
    {
      //Add draw arguments to vector of graphics
      for (int i = 0; i < exp.tail.size(); i++)
      {
        Expression graphic = evalUtility(exp.tail[i]);
        graphics.push_back(graphic.head);
      }
      return Expression();
    }

    //End special forms

    //Start Procedures/Symbols

    //Procedure
    if (exp.head.value.sym_value.compare("not") == 0 ||
        exp.head.value.sym_value.compare("and") == 0 ||
        exp.head.value.sym_value.compare("or") == 0 ||
        exp.head.value.sym_value.compare("<") == 0 ||
        exp.head.value.sym_value.compare("<=") == 0 ||
        exp.head.value.sym_value.compare(">") == 0 ||
        exp.head.value.sym_value.compare(">=") == 0 ||
        exp.head.value.sym_value.compare("=") == 0 ||
        exp.head.value.sym_value.compare("+") == 0 ||
        exp.head.value.sym_value.compare("-") == 0 ||
        exp.head.value.sym_value.compare("-") == 0 ||
        exp.head.value.sym_value.compare("*") == 0 ||
        exp.head.value.sym_value.compare("/") == 0 ||
        exp.head.value.sym_value.compare("log10") == 0 ||
        exp.head.value.sym_value.compare("point") == 0 ||
        exp.head.value.sym_value.compare("line") == 0 ||
        exp.head.value.sym_value.compare("arc") == 0 ||
        exp.head.value.sym_value.compare("sin") == 0 ||
        exp.head.value.sym_value.compare("cos") == 0 ||
        exp.head.value.sym_value.compare("arctan") == 0)
    {
      Procedure proc = env.getResultProcedure(exp.head.value.sym_value);
      std::vector<Atom> args;
      for (int i = 0; i < exp.tail.size(); i++)
      {
        //Recursively evaluate the procedure arguments until they are atoms
        if (!exp.tail[i].tail.empty())
        {
          exp.tail[i] = evalUtility(exp.tail[i]);
        }

        //Argument is a symbol
        if (exp.tail[i].head.type == SymbolType)
        {
          Expression resultExp = env.getResultExpression(exp.tail[i].head.value.sym_value);
          args.push_back(resultExp.head);
        }
        //Argument is not a symbol
        else
        {
          args.push_back(exp.tail[i].head);
        }
      }
      return proc(args);
    }
    //Symbol
    return env.getResultExpression(exp.head.value.sym_value);
  }

  //Number type or Boolean Type (atoms)
  if (exp.head.type == NumberType || exp.head.type == BooleanType)
  {
    return exp;
  }
}

//Returns the vector of graphic Atoms (points, lines, and arcs)
std::vector<Atom> Interpreter::getGraphics()
{
  return graphics;
}
