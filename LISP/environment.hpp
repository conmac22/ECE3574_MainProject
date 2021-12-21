#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

//System includes
#include <map>

//Module includes
#include "expression.hpp"

class Environment
{
public:
  Environment();
  Expression getResultExpression(Symbol s);
  Procedure getResultProcedure(Symbol s);
  Expression updateMap(const std::vector<Atom>& args);

private:
  enum EnvResultType {ExpressionType, ProcedureType};
  
  struct EnvResult
  {
    EnvResultType type;
    Expression exp;
    Procedure proc;
  };

  std::map<Symbol,EnvResult> envmap;
};

#endif
