//System includes
#include <cmath>
#include <limits>
#include <cctype>
#include <sstream>
#include <ctype.h>

//Module includes
#include "expression.hpp"

//Boolean Expression
Expression::Expression(bool tf)
{
  head.type = BooleanType;
  head.value.bool_value = tf;
}

//Number Expression
Expression::Expression(double num)
{
  head.type = NumberType;
  head.value.num_value = num;
}

//Symbol Expression
Expression::Expression(const std::string & sym)
{
  head.type = SymbolType;
  head.value.sym_value = sym;
}

//Point Expression
Expression::Expression(std::tuple<double,double> value)
{
  head.type = PointType;
  head.value.point_value.x = std::get<0>(value);
  head.value.point_value.y = std::get<1>(value);
}

//Line Expression
Expression::Expression(std::tuple<double,double> start,
		       std::tuple<double,double> end)
{
  head.type = LineType;
  head.value.line_value.first.x = std::get<0>(start);
  head.value.line_value.first.y = std::get<1>(start);
  head.value.line_value.second.x = std::get<0>(end);
  head.value.line_value.second.y = std::get<1>(end);
}

//Arc Expression
Expression::Expression(std::tuple<double,double> center,
		       std::tuple<double,double> start,
		       double angle)
{
  head.type = ArcType;
  head.value.arc_value.center.x = std::get<0>(center);
  head.value.arc_value.center.y = std::get<1>(center);
  head.value.arc_value.start.x = std::get<0>(start);
  head.value.arc_value.start.y = std::get<1>(start);
  head.value.arc_value.span = angle;
}

//Equality operator
bool Expression::operator==(const Expression & exp) const noexcept
{
  //Empty Expression
  if (head.type == NoneType && exp.head.type == NoneType)
  {
    return true;
  }
  //Boolean Expression
  if (head.type == BooleanType && exp.head.type == BooleanType &&
      head.value.bool_value == exp.head.value.bool_value)
  {
    return true;
  }
  //Number Expression
  if (head.type == NumberType && exp.head.type == NumberType &&
      fabs(head.value.num_value - exp.head.value.num_value) <
           std::numeric_limits<double>::epsilon())
  {
    return true;
  }
  //Symbol Expression
  if (head.type == SymbolType && exp.head.type == SymbolType &&
      head.value.sym_value.compare(exp.head.value.sym_value) == 0)
  {
    return true;
  }
  //Point Expression
  if (head.type == PointType && exp.head.type == PointType &&
      head.value.point_value == exp.head.value.point_value)
  {
    return true;
  }
  //Line Expression
  if (head.type == LineType && exp.head.type == LineType &&
      head.value.line_value.first == exp.head.value.line_value.first &&
      head.value.line_value.second == exp.head.value.line_value.second)
  {
    return true;
  }
  //Arc Expression
  if (head.type == ArcType && exp.head.type == ArcType &&
      head.value.arc_value.center == exp.head.value.arc_value.center &&
      head.value.arc_value.start == exp.head.value.arc_value.start &&
      head.value.arc_value.span == exp.head.value.arc_value.span)
  {
    return true;
  }
  return false;
}

//Inequality operator
bool Expression::operator!=(const Expression & exp) const noexcept
{
  //Empty Expression
  if (head.type == NoneType && exp.head.type == NoneType)
  {
    return false;
  }
  //Boolean Expression
  if (head.type == BooleanType && exp.head.type == BooleanType &&
           head.value.bool_value == exp.head.value.bool_value)
  {
    return false;
  }
  //Number Expression
  if (head.type == NumberType && exp.head.type == NumberType &&
      fabs(head.value.num_value - exp.head.value.num_value) <
           std::numeric_limits<double>::epsilon())
  {
    return false;
  }
  //Symbol Expression
  if (head.type == SymbolType && exp.head.type == SymbolType &&
           head.value.sym_value.compare(exp.head.value.sym_value) == 0)
  {
    return false;
  }
  //Point Expression
  if (head.type == PointType && exp.head.type == PointType &&
      head.value.point_value == exp.head.value.point_value)
  {
    return false;
  }
  //Line Expression
  if (head.type == LineType && exp.head.type == LineType &&
      head.value.line_value.first == exp.head.value.line_value.first &&
      head.value.line_value.second == exp.head.value.line_value.second)
  {
    return false;
  }
  //Arc Expression
  if (head.type == ArcType && exp.head.type == ArcType &&
      head.value.arc_value.center == exp.head.value.arc_value.center &&
      head.value.arc_value.start == exp.head.value.arc_value.start &&
      head.value.arc_value.span == exp.head.value.arc_value.span)
  {
    return false;
  }
  return true;
}

//Maps a given token string to an Atom
bool token_to_atom(const std::string & token, Atom & atom)
{
  if (token.empty())
  {
    atom.type = NoneType;
  }
	//Token cannot contain whitespace
  else if (token.find(' ') != std::string::npos)
  {
    return false;
  }
  else if (token == "True")
  {
    atom.type = BooleanType;
    atom.value.bool_value = true;
  }
  else if (token == "False")
  {
    atom.type = BooleanType;
    atom.value.bool_value = false;
  }
  else
  {
		//Differentiate between number and symbol
    bool numFlag = true;
    try
    {
      std::stod(token);
    }
    //Not a number
    catch (const std::exception& ex)
    {
      numFlag = false;
    }
    if (numFlag)
    {
      for (int i = 0; i < token.size(); i++)
      {
        if ((isdigit(token[i]) == 0) && token[i] != '+' && token[i] != '-' &&
            token[i] != 'e' && token[i] != '.')
        {
          numFlag = false;
          break;
        }
      }
    }

    if (numFlag) //Token is a number
    {
      atom.type = NumberType;
      atom.value.num_value = std::stod(token);
    }
    else //Token is a symbol
    {
			//The first character of a symbol cannot be a number
      if (isdigit(token[0]) != 0)
      {
        return false;
      }
      atom.type = SymbolType;
      atom.value.sym_value = token;
    }
  }
  return true;
}
