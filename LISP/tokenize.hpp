#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <istream>
#include <deque>

typedef std::deque<std::string> TokenSequenceType;

const char OPEN = '(';
const char CLOSE = ')';
const char COMMENT = ';';

//Split string into a list of tokens where a token is one of
//OPEN or CLOSE or a space-delimited string
//Ignores any whitespace and from any ";" to end-of-line
TokenSequenceType tokenize(std::istream & seq);

#endif
