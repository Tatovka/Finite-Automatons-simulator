
// Generated from REParser/RegEx.g4 by ANTLR 4.13.2


#include "RegExLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct RegExLexerStaticData final {
  RegExLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  RegExLexerStaticData(const RegExLexerStaticData&) = delete;
  RegExLexerStaticData(RegExLexerStaticData&&) = delete;
  RegExLexerStaticData& operator=(const RegExLexerStaticData&) = delete;
  RegExLexerStaticData& operator=(RegExLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag regexlexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<RegExLexerStaticData> regexlexerLexerStaticData = nullptr;

void regexlexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (regexlexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(regexlexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<RegExLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "OP", "CHAR"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'|'", "'('", "')'"
    },
    std::vector<std::string>{
      "", "", "", "", "OP", "CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,5,21,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,1,0,1,0,1,1,1,1,
  	1,2,1,2,1,3,1,3,1,4,1,4,0,0,5,1,1,3,2,5,3,7,4,9,5,1,0,2,2,0,42,43,63,
  	63,1,0,48,57,20,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,
  	0,0,0,1,11,1,0,0,0,3,13,1,0,0,0,5,15,1,0,0,0,7,17,1,0,0,0,9,19,1,0,0,
  	0,11,12,5,124,0,0,12,2,1,0,0,0,13,14,5,40,0,0,14,4,1,0,0,0,15,16,5,41,
  	0,0,16,6,1,0,0,0,17,18,7,0,0,0,18,8,1,0,0,0,19,20,7,1,0,0,20,10,1,0,0,
  	0,1,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  regexlexerLexerStaticData = std::move(staticData);
}

}

RegExLexer::RegExLexer(CharStream *input) : Lexer(input) {
  RegExLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *regexlexerLexerStaticData->atn, regexlexerLexerStaticData->decisionToDFA, regexlexerLexerStaticData->sharedContextCache);
}

RegExLexer::~RegExLexer() {
  delete _interpreter;
}

std::string RegExLexer::getGrammarFileName() const {
  return "RegEx.g4";
}

const std::vector<std::string>& RegExLexer::getRuleNames() const {
  return regexlexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& RegExLexer::getChannelNames() const {
  return regexlexerLexerStaticData->channelNames;
}

const std::vector<std::string>& RegExLexer::getModeNames() const {
  return regexlexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& RegExLexer::getVocabulary() const {
  return regexlexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView RegExLexer::getSerializedATN() const {
  return regexlexerLexerStaticData->serializedATN;
}

const atn::ATN& RegExLexer::getATN() const {
  return *regexlexerLexerStaticData->atn;
}




void RegExLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  regexlexerLexerInitialize();
#else
  ::antlr4::internal::call_once(regexlexerLexerOnceFlag, regexlexerLexerInitialize);
#endif
}
