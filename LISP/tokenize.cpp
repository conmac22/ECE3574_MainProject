//System includes
#include <cctype>
#include <iostream>

//Module includes
#include "tokenize.hpp"

//Parses the stream into a deque 
TokenSequenceType tokenize(std::istream& seq)
{
  TokenSequenceType tokens; //A deque of the parsed tokens

  char c; //Stores one character in the stream
  std::string token; //Stores a single token
  bool commentFlag = false; //Set when a comment character is encountered, reset
                            //when a newline character is encountered and the
                            //comment flag was set

  while (seq.get(c)) //Process every character in the stream
  {
    //Skip character if comment or carriage character has been encountered
    if (!commentFlag && c != 13)
    {
      if (c == OPEN)
      {
        tokens.push_back("(");
      }
      else if (c == COMMENT)
      {
        commentFlag = true;
      }
      else if (c == CLOSE)
      {
        if (!token.empty())
        {
          tokens.push_back(token);
          token = ""; //Reset token
        }
        tokens.push_back(")");
      }
      else if (c == '\n' || c == ' ')
      {
        if (!token.empty())
        {
          tokens.push_back(token);
          token = ""; //Reset token
        }
      }
      else
      {
        token += c;
      }
    }

    else
    {
      if (c == '\n') //End comment
      {
        token = ""; //Reset token
        commentFlag = false;
      }
    }
  }

  if (!token.empty()) //Handle truncated input
  {
      tokens.push_back(token);
  }
  return tokens;
}
