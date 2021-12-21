//Test includes
#include "catch.hpp"

//System includes
#include <string>

//Module includes
#include "expression.hpp"

//---INSTRUCTOR TESTS---

TEST_CASE( "Test Type Inference", "[types]" ) {

  Atom a;

  std::string token = "True";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == BooleanType);
  REQUIRE(a.value.bool_value == true);

  token = "False";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == BooleanType);
  REQUIRE(a.value.bool_value == false);

  token = "1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 1);

  token = "-1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == -1);

  token = "var";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == "var");

  token = "1abc";
  REQUIRE(!token_to_atom(token, a));

  token = "var1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == token);

}

TEST_CASE( "Test Expression Constructors", "[types]" ) {

  Expression exp1;

  REQUIRE(exp1 == Expression());
}

//---STUDENT TESTS---

TEST_CASE( "Test Expression Constructors (Non-empty)", "[types]" )
{
  //Boolean
  Expression expBoolTrue1(true);
  Expression expBoolTrue2(true);
  Expression expBoolFalse(false);
  REQUIRE(expBoolTrue1 == expBoolTrue2);
  REQUIRE(expBoolTrue1 != expBoolFalse);

  //Number
  Expression expNumber1(1.5);
  Expression expNumber2(1.5);
  Expression expNumber3(2.5);
  REQUIRE(expNumber1 == expNumber2);
  REQUIRE(expNumber1 != expNumber3);

  //Symbol
  std::string test1 = "Test1";
  std::string test2 = "Test1";
  std::string test3 = "Test3";
  Expression expSymbol1(test1);
  Expression expSymbol2(test2);
  Expression expSymbol3(test3);
  REQUIRE(expSymbol1 == expSymbol2);
  REQUIRE(expSymbol1 != expSymbol3);

  //Point
  std::tuple<double, double> point1 = std::make_tuple(10, 20);
  std::tuple<double, double> point2 = std::make_tuple(10, 20);
  std::tuple<double, double> point3 = std::make_tuple(10, 30);
  std::tuple<double, double> point4 = std::make_tuple(30, 20);
  std::tuple<double, double> point5 = std::make_tuple(30, 30);
  Expression expPoint1(point1);
  Expression expPoint2(point2);
  Expression expPoint3(point3);
  Expression expPoint4(point4);
  Expression expPoint5(point5);
  //Same x and y
  REQUIRE(expPoint1 == expPoint2);
  //Same x but different y
  REQUIRE(expPoint1 != expPoint3);
  //Same y but different x
  REQUIRE(expPoint1 != expPoint4);
  //Different x and y
  REQUIRE(expPoint1 != expPoint5);

  //Line
  std::tuple<double, double> start1 = std::make_tuple(10, 20);
  std::tuple<double, double> end1 = std::make_tuple(10, 30);
  std::tuple<double, double> start2 = std::make_tuple(10, 20);
  std::tuple<double, double> end2 = std::make_tuple(10, 30);
  std::tuple<double, double> start3 = std::make_tuple(10, 40);
  std::tuple<double, double> end3 = std::make_tuple(10, 40);
  std::tuple<double, double> start4 = std::make_tuple(10, 50);
  std::tuple<double, double> end4 = std::make_tuple(10, 50);
  Expression expLine1(start1, end1);
  Expression expLine2(start2, end2);
  Expression expLine3(start2, end3);
  Expression expLine4(start3, end2);
  Expression expLine5(start4, end4);
  //Same start and end
  REQUIRE(expLine1 == expLine2);
  //Same start but different end
  REQUIRE(expLine1 != expLine3);
  //Same end but different start
  REQUIRE(expLine1 != expLine4);
  //Different start and end
  REQUIRE(expLine1 != expLine5);

  //Arc
  std::tuple<double, double> center = std::make_tuple(10, 20);
  std::tuple<double, double> start = std::make_tuple(10, 30);
  Expression expArc1(center, start, 1.0);
  Expression expArc2(center, start, 1.0);
  Expression expArc3(center, start, 2.0);
  //Same center, start, and span
  REQUIRE(expArc1 == expArc2);
  //Same start and end, but different span
  REQUIRE(expArc1 != expArc3);
}
