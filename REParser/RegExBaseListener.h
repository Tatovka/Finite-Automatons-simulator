
// Generated from REParser/RegEx.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "RegExListener.h"


/**
 * This class provides an empty implementation of RegExaListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  RegExBaseListener : public RegExListener {
public:

  virtual void enterRe(RegExParser::ReContext * ctx) override {

    if (ctx->CHAR() != nullptr) {
      std::cout << ctx->CHAR()->toString();
      return;
    }

    std::cout << '(';
    if (ctx->left != nullptr) {
      enterRe(ctx->left);
      if (ctx->or_ != nullptr) {
        std::cout << " or ";
      }
      else {
        std::cout << " concat ";
      }
      enterRe(ctx->right);
    }

    if (ctx->single != nullptr) {
      enterRe(ctx->single);
      if (ctx->OP() != nullptr) {
        std::cout << ctx->OP()->toString();
      }
    }
    std::cout << ")";
  }
  virtual void exitRe(RegExParser::ReContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

