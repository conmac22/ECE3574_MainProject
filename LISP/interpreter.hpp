#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

//System includes
#include <string>
#include <istream>
#include <vector>


//Module includes
#include "expression.hpp"
#include "environment.hpp"
#include "tokenize.hpp"

//Interpreter has
//Environment, which starts at a default
//parse method, builds an internal AST
//eval method, updates Environment, returns last result
class Interpreter
{
public:
  bool parse(std::istream& expression) noexcept;
  Expression eval();
  std::vector<Atom> getGraphics();
protected:
  Environment env;
  Expression ast;
  std::vector<Atom> graphics;
  Expression parseUtility(std::string token, TokenSequenceType& tokens);
  Expression evalUtility(Expression& exp);
};

#endif
