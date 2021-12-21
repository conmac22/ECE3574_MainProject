//System includes
#include <cassert>
#include <cmath>

//Module includes
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"

//Non-member functions for procedures
Expression notFunc(const std::vector<Atom>& args);
Expression andFunc(const std::vector<Atom>& args);
Expression orFunc(const std::vector<Atom>& args);
Expression lessThanFunc(const std::vector<Atom>& args);
Expression lessThanEqualToFunc(const std::vector<Atom>& args);
Expression greaterThanFunc(const std::vector<Atom>& args);
Expression greaterThanEqualToFunc(const std::vector<Atom>& args);
Expression equalFunc(const std::vector<Atom>& args);
Expression addFunc(const std::vector<Atom>& args);
Expression negateOrSubtractFunc(const std::vector<Atom>& args);
Expression multiplyFunc(const std::vector<Atom>& args);
Expression divideFunc(const std::vector<Atom>& args);
Expression logFunc(const std::vector<Atom>& args);
Expression powFunc(const std::vector<Atom>& args);
Expression pointFunc(const std::vector<Atom>& args);
Expression lineFunc(const std::vector<Atom>& args);
Expression arcFunc(const std::vector<Atom>& args);
Expression sinFunc(const std::vector<Atom>& args);
Expression cosFunc(const std::vector<Atom>& args);
Expression arctanFunc(const std::vector<Atom>& args);

//Default mapping environment
Environment::Environment()
{
  //Default procedures
  envmap["not"] = {ProcedureType, Expression(), &notFunc};
  envmap["and"] = {ProcedureType, Expression(), &andFunc};
  envmap["or"] = {ProcedureType, Expression(), &orFunc};
  envmap["<"] = {ProcedureType, Expression(), &lessThanFunc};
  envmap["<="] = {ProcedureType, Expression(), &lessThanEqualToFunc};
  envmap[">"] = {ProcedureType, Expression(), &greaterThanFunc};
  envmap[">="] = {ProcedureType, Expression(), &greaterThanEqualToFunc};
  envmap["="] = {ProcedureType, Expression(), &equalFunc};
  envmap["+"] = {ProcedureType, Expression(), &addFunc};
  envmap["-"] = {ProcedureType, Expression(), &negateOrSubtractFunc};
  envmap["*"] = {ProcedureType, Expression(), &multiplyFunc};
  envmap["/"] = {ProcedureType, Expression(), &divideFunc};
  envmap["log10"] = {ProcedureType, Expression(), &logFunc};
  envmap["pow"] = {ProcedureType, Expression(), &powFunc};
  envmap["point"] = {ProcedureType, Expression(), &pointFunc};
  envmap["line"] = {ProcedureType, Expression(), &lineFunc};
  envmap["arc"] = {ProcedureType, Expression(), &arcFunc};
  envmap["sin"] = {ProcedureType, Expression(), &sinFunc};
  envmap["cos"] = {ProcedureType, Expression(), &cosFunc};
  envmap["arctan"] = {ProcedureType, Expression(), &arctanFunc};

  //pi
  Expression piExp(atan2(0, -1));
  envmap["pi"] = {ExpressionType, piExp, Procedure()};
}

//Accessor method for map (Expression only)
Expression Environment::getResultExpression(Symbol s)
{
  std::map<Symbol, EnvResult>::iterator it = envmap.find(s);
  //Symbol is not in map
  if (it == envmap.end())
  {
    throw InterpreterSemanticError("Symbol is not in map");
  }

  EnvResult result = envmap[s];
  return result.exp;
}

//Accessor method for map (Procedure only)
Procedure Environment::getResultProcedure(Symbol s)
{
  std::map<Symbol, EnvResult>::iterator it = envmap.find(s);
  //Symbol is not in map
  if (it == envmap.end())
  {
    throw InterpreterSemanticError("Symbol is not in map");
  }

  EnvResult result = envmap[s];
  return result.proc;
}

//Setter method for "define"
Expression Environment::updateMap(const std::vector<Atom>& args)
{
  //Error checking
  if (args[0].type != SymbolType)
  {
    throw InterpreterSemanticError("Cannot add a nonsymbol to the map");
  }
  if (args[0].value.sym_value.compare("begin") == 0 ||
      args[0].value.sym_value.compare("define") == 0 ||
      args[0].value.sym_value.compare("if") == 0)
  {
    throw InterpreterSemanticError("Cannot redefine special forms");
  }
  std::map<Symbol, EnvResult>::iterator it = envmap.find(args[0].value.sym_value);
  if (it != envmap.end())
  {
    throw InterpreterSemanticError("Cannot redefine a symbol");
  }

  //Add to map
  EnvResult result = {ExpressionType, args[1], Procedure()};
  envmap[args[0].value.sym_value] = result;

  return args[1];
}

//Functions for each procedure

//not
Expression notFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 1)
  {
    throw InterpreterSemanticError("Invalid arguments for 'not'");
  }
  if (args[0].type != BooleanType)
  {
    throw InterpreterSemanticError("Not a boolean type for 'not'");
  }

  //Logical not
  return Expression(!args[0].value.bool_value);
}

//and
Expression andFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() == 1)
  {
    throw InterpreterSemanticError("Only one argument for 'and'");
  }
  for (int i = 0; i < args.size(); i++)
  {
    if (args[i].type != BooleanType)
    {
      throw InterpreterSemanticError("Not a boolean type for 'and'");
    }
  }

  //Logical and
  for (int i = 0; i < args.size(); i++)
  {
    if (!args[i].value.bool_value)
    {
      return Expression(false);
    }
  }
  return Expression(true);
}

//or
Expression orFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() == 1)
  {
    throw InterpreterSemanticError("Only one argument for 'or'");
  }
  for (int i = 0; i < args.size(); i++)
  {
    if (args[i].type != BooleanType)
    {
      throw InterpreterSemanticError("Not a boolean type for 'or'");
    }
  }

  //Logical or
  for (int i = 0; i < args.size(); i++)
  {
    if (args[i].value.bool_value)
    {
      return Expression(true);
    }
  }
  return Expression(false);
}

//<
Expression lessThanFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 2)
  {
    throw InterpreterSemanticError("Invalid number of arguments for '<'");
  }
  if (args[0].type != NumberType || args[1].type != NumberType)
  {
    throw InterpreterSemanticError("Not a number type for '<'");
  }

  //Less than
  if (args[0].value.num_value < args[1].value.num_value)
  {
    return Expression(true);
  }
  return Expression(false);
}

//<=
Expression lessThanEqualToFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 2)
  {
    throw InterpreterSemanticError("Invalid number of arguments for '<='");
  }
  if (args[0].type != NumberType || args[1].type != NumberType)
  {
    throw InterpreterSemanticError("Not a number type for '<='");
  }

  //Less than or equal to
  if (args[0].value.num_value <= args[1].value.num_value)
  {
    return Expression(true);
  }
  return Expression(false);
}

//>
Expression greaterThanFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 2)
  {
    throw InterpreterSemanticError("Invalid number of arguments for '>'");
  }
  if (args[0].type != NumberType || args[1].type != NumberType)
  {
    throw InterpreterSemanticError("Not a number type for '>'");
  }

  //Greater than
  if (args[0].value.num_value > args[1].value.num_value)
  {
    return Expression(true);
  }
  return Expression(false);
}

//>=
Expression greaterThanEqualToFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 2)
  {
    throw InterpreterSemanticError("Invalid number of arguments for '>='");
  }
  if (args[0].type != NumberType || args[1].type != NumberType)
  {
    throw InterpreterSemanticError("Not a number type for '>='");
  }

  //Greater than or equal to
  if (args[0].value.num_value >= args[1].value.num_value)
  {
    return Expression(true);
  }
  return Expression(false);
}


//=
Expression equalFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 2)
  {
    throw InterpreterSemanticError("Invalid number of arguments for '='");
  }
  if (args[0].type != NumberType || args[1].type != NumberType)
  {
    throw InterpreterSemanticError("Not a number type for '='");
  }

  //Equal
  if (args[0].value.num_value == args[1].value.num_value)
  {
    return Expression(true);
  }
  return Expression(false);
}

//add
Expression addFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() < 2)
  {
    throw InterpreterSemanticError("Only one argument for '+'");
  }
  for (int i = 0; i < args.size(); i++)
  {
    if (args[i].type != NumberType)
    {
      throw InterpreterSemanticError("Not a number type for '+'");
    }
  }

  //Addition
  double sum = 0.0;
  for (int i = 0; i < args.size(); i++)
  {
    sum += args[i].value.num_value;
  }
  return Expression(sum);
}

//- (Negate or subtract)
Expression negateOrSubtractFunc(const std::vector<Atom>& args)
{
  //Should be negation
  if (args.size() == 1)
  {
    //Error checking
    if (args[0].type != NumberType)
    {
      throw InterpreterSemanticError("Not a number type for 'negate'");
    }

    //Negation
    return Expression(-args[0].value.num_value);
  }

  //Should be subtraction
  if (args.size() == 2)
  {
    //Error checking
    if (args[0].type != NumberType || args[1].type != NumberType)
    {
      throw InterpreterSemanticError("Not a number type for 'subtract'");
    }

    //Subtraction
    return Expression(args[0].value.num_value - args[1].value.num_value);
  }
  //More error checking
  throw InterpreterSemanticError("Incorrect number of arguments for '-'");
}

//*
Expression multiplyFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() < 2)
  {
    throw InterpreterSemanticError("Only one argument for '*'");
  }
  for (int i = 0; i < args.size(); i++)
  {
    if (args[i].type != NumberType)
    {
      throw InterpreterSemanticError("Not a number type for '*'");
    }
  }

  //Multiplication
  double product = 1.0;
  for (int i = 0; i < args.size(); i++)
  {
     product *= args[i].value.num_value;
  }
  return Expression(product);
}

///
Expression divideFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 2)
  {
    throw InterpreterSemanticError("Incorrect number of arguments for '/'");
  }
  if (args[0].type != NumberType || args[1].type != NumberType)
  {
    throw InterpreterSemanticError("Not a number type for '/'");
  }

  //Division
  return Expression(args[0].value.num_value / args[1].value.num_value);
}

//log10
Expression logFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 1)
  {
    throw InterpreterSemanticError("Incorrect number of arguments for 'log10'");
  }
  if (args[0].type != NumberType)
  {
    throw InterpreterSemanticError("Not a number type for 'log10'");
  }

  //Log (base 10)
  return Expression(log10(args[0].value.num_value));
}

//pow
Expression powFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 2)
  {
    throw InterpreterSemanticError("Incorrect number of arguments for 'pow'");
  }
  if (args[0].type != NumberType || args[1].type != NumberType)
  {
    throw InterpreterSemanticError("Not a number type for 'pow'");
  }

  //Power
  return Expression(pow(args[0].value.num_value, args[1].value.num_value));
}

//point
Expression pointFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 2)
  {
    throw InterpreterSemanticError("Incorrect number of arguments for 'point'");
  }
  if (args[0].type != NumberType || args[1].type != NumberType)
  {
    throw InterpreterSemanticError("Not a number type for 'point'");
  }

  //Create point
  std::tuple<double, double> point = std::make_tuple(args[0].value.num_value,
                                     args[1].value.num_value);
  return Expression(point);
}

//line
Expression lineFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 2)
  {
    throw InterpreterSemanticError("Incorrect number of arguments for 'line'");
  }
  if (args[0].type != PointType || args[1].type != PointType)
  {
    throw InterpreterSemanticError("Not a Point type for 'line'");
  }

  //Create line
  std::tuple<double, double> start = std::make_tuple(
    args[0].value.point_value.x, args[0].value.point_value.y);
  std::tuple<double, double> end = std::make_tuple(
    args[1].value.point_value.x, args[1].value.point_value.y);
  return Expression(start, end);
}

//arc
Expression arcFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 3)
  {
    throw InterpreterSemanticError("Incorrect number of arguments for 'arc'");
  }
  if (args[0].type != PointType || args[1].type != PointType)
  {
    throw InterpreterSemanticError("Not a Point type for 'arc' Points");
  }
  if (args[2].type != NumberType)
  {
    throw InterpreterSemanticError("Not a Number type for 'arc' span angle");
  }

  //Create arc
  std::tuple<double, double> center = std::make_tuple(
    args[0].value.point_value.x, args[0].value.point_value.y);
  std::tuple<double, double> start = std::make_tuple(
    args[1].value.point_value.x, args[1].value.point_value.y);
  return Expression(center, start, args[2].value.num_value);
}

//sin
Expression sinFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 1)
  {
    throw InterpreterSemanticError("Incorrect number of arguments for 'sin'");
  }
  if (args[0].type != NumberType)
  {
    throw InterpreterSemanticError("Not a Number type for 'sin'");
  }

  //Sin
  return Expression(sin(args[0].value.num_value));
}

//cos
Expression cosFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 1)
  {
    throw InterpreterSemanticError("Incorrect number of arguments for 'cos'");
  }
  if (args[0].type != NumberType)
  {
    throw InterpreterSemanticError("Not a Number type for 'cos'");
  }

  //Cos
  return Expression(cos(args[0].value.num_value));
}

//arctan
Expression arctanFunc(const std::vector<Atom>& args)
{
  //Error checking
  if (args.size() != 2)
  {
    throw InterpreterSemanticError("Incorrect number of arguments for 'arctan'");
  }
  if (args[0].type != NumberType || args[1].type != NumberType)
  {
    throw InterpreterSemanticError("Not a Number type for 'arctan'");
  }

  //Arctan
  return Expression(atan2(args[0].value.num_value, args[1].value.num_value));
}
