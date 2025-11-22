
// Generated from REParser/RegEx.g4 by ANTLR 4.13.2


#include "RegExListener.h"

#include "RegExParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct RegExParserStaticData final {
  RegExParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  RegExParserStaticData(const RegExParserStaticData&) = delete;
  RegExParserStaticData(RegExParserStaticData&&) = delete;
  RegExParserStaticData& operator=(const RegExParserStaticData&) = delete;
  RegExParserStaticData& operator=(RegExParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag regexParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<RegExParserStaticData> regexParserStaticData = nullptr;

void regexParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (regexParserStaticData != nullptr) {
    return;
  }
#else
  assert(regexParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<RegExParserStaticData>(
    std::vector<std::string>{
      "re"
    },
    std::vector<std::string>{
      "", "'|'", "'('", "')'"
    },
    std::vector<std::string>{
      "", "", "", "", "OP", "CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,5,23,2,0,7,0,1,0,1,0,1,0,1,0,1,0,1,0,3,0,9,8,0,1,0,1,0,1,0,1,0,1,
  	0,1,0,1,0,5,0,18,8,0,10,0,12,0,21,9,0,1,0,0,1,0,1,0,0,0,25,0,8,1,0,0,
  	0,2,3,6,0,-1,0,3,4,5,2,0,0,4,5,3,0,0,0,5,6,5,3,0,0,6,9,1,0,0,0,7,9,5,
  	5,0,0,8,2,1,0,0,0,8,7,1,0,0,0,9,19,1,0,0,0,10,11,10,4,0,0,11,18,3,0,0,
  	5,12,13,10,3,0,0,13,14,5,1,0,0,14,18,3,0,0,4,15,16,10,5,0,0,16,18,5,4,
  	0,0,17,10,1,0,0,0,17,12,1,0,0,0,17,15,1,0,0,0,18,21,1,0,0,0,19,17,1,0,
  	0,0,19,20,1,0,0,0,20,1,1,0,0,0,21,19,1,0,0,0,3,8,17,19
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  regexParserStaticData = std::move(staticData);
}

}

RegExParser::RegExParser(TokenStream *input) : RegExParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

RegExParser::RegExParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  RegExParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *regexParserStaticData->atn, regexParserStaticData->decisionToDFA, regexParserStaticData->sharedContextCache, options);
}

RegExParser::~RegExParser() {
  delete _interpreter;
}

const atn::ATN& RegExParser::getATN() const {
  return *regexParserStaticData->atn;
}

std::string RegExParser::getGrammarFileName() const {
  return "RegEx.g4";
}

const std::vector<std::string>& RegExParser::getRuleNames() const {
  return regexParserStaticData->ruleNames;
}

const dfa::Vocabulary& RegExParser::getVocabulary() const {
  return regexParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView RegExParser::getSerializedATN() const {
  return regexParserStaticData->serializedATN;
}


//----------------- ReContext ------------------------------------------------------------------

RegExParser::ReContext::ReContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<RegExParser::ReContext *> RegExParser::ReContext::re() {
  return getRuleContexts<RegExParser::ReContext>();
}

RegExParser::ReContext* RegExParser::ReContext::re(size_t i) {
  return getRuleContext<RegExParser::ReContext>(i);
}

tree::TerminalNode* RegExParser::ReContext::CHAR() {
  return getToken(RegExParser::CHAR, 0);
}

tree::TerminalNode* RegExParser::ReContext::OP() {
  return getToken(RegExParser::OP, 0);
}


size_t RegExParser::ReContext::getRuleIndex() const {
  return RegExParser::RuleRe;
}

void RegExParser::ReContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RegExListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRe(this);
}

void RegExParser::ReContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RegExListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRe(this);
}


RegExParser::ReContext* RegExParser::re() {
   return re(0);
}

RegExParser::ReContext* RegExParser::re(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  RegExParser::ReContext *_localctx = _tracker.createInstance<ReContext>(_ctx, parentState);
  RegExParser::ReContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 0;
  enterRecursionRule(_localctx, 0, RegExParser::RuleRe, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(8);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RegExParser::T__1: {
        setState(3);
        match(RegExParser::T__1);
        setState(4);
        antlrcpp::downCast<ReContext *>(_localctx)->single = re(0);
        setState(5);
        match(RegExParser::T__2);
        break;
      }

      case RegExParser::CHAR: {
        setState(7);
        match(RegExParser::CHAR);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(19);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(17);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<ReContext>(parentContext, parentState);
          _localctx->left = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleRe);
          setState(10);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(11);
          antlrcpp::downCast<ReContext *>(_localctx)->right = re(5);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<ReContext>(parentContext, parentState);
          _localctx->left = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleRe);
          setState(12);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(13);
          antlrcpp::downCast<ReContext *>(_localctx)->or_ = match(RegExParser::T__0);
          setState(14);
          antlrcpp::downCast<ReContext *>(_localctx)->right = re(4);
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<ReContext>(parentContext, parentState);
          _localctx->single = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleRe);
          setState(15);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(16);
          match(RegExParser::OP);
          break;
        }

        default:
          break;
        } 
      }
      setState(21);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool RegExParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 0: return reSempred(antlrcpp::downCast<ReContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool RegExParser::reSempred(ReContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 4);
    case 1: return precpred(_ctx, 3);
    case 2: return precpred(_ctx, 5);

  default:
    break;
  }
  return true;
}

void RegExParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  regexParserInitialize();
#else
  ::antlr4::internal::call_once(regexParserOnceFlag, regexParserInitialize);
#endif
}
