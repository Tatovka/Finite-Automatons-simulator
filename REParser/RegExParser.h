
// Generated from REParser/RegEx.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  RegExParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, OP = 4, CHAR = 5
  };

  enum {
    RuleRe = 0
  };

  explicit RegExParser(antlr4::TokenStream *input);

  RegExParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~RegExParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ReContext; 

  class  ReContext : public antlr4::ParserRuleContext {
  public:
    RegExParser::ReContext *single = nullptr;
    RegExParser::ReContext *left = nullptr;
    RegExParser::ReContext *right = nullptr;
    antlr4::Token *or_ = nullptr;
    ReContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ReContext *> re();
    ReContext* re(size_t i);
    antlr4::tree::TerminalNode *CHAR();
    antlr4::tree::TerminalNode *OP();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ReContext* re();
  ReContext* re(int precedence);

  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool reSempred(ReContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

