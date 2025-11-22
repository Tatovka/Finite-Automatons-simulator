
// Generated from REParser/RegEx.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "RegExParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by RegExParser.
 */
class  RegExListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterRe(RegExParser::ReContext *ctx) = 0;
  virtual void exitRe(RegExParser::ReContext *ctx) = 0;


};

