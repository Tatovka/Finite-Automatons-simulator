//
// Created by tatovka on 11/22/25.
//

#ifndef REGEXVISITOR_H
#define REGEXVISITOR_H
#include "ENFA.h"
#include "antlr4-runtime.h"
#include "RegExParser.h"

struct RegExVisitor {
    ENFA visitRE(RegExParser::ReContext * ctx) {
        std::cout << '(';
        if (ctx->left != nullptr) {
            return visitBinaryOp(ctx->or_, visitRE(ctx->left), visitRE(ctx->right));
        }
        if (ctx->single != nullptr) {
            return visitOp(ctx->OP(), visitRE(ctx->single));
        }
        std::cout << ")";
        return visitChar(ctx->CHAR());
    }

    ENFA visitOp(antlr4::tree::TerminalNode* node, ENFA operand) {
        if (node == nullptr) return operand;
        switch (node->getText()[0]) {
            case '*': return ENFA::Astr(operand);
            case '+': return ENFA::Plus(operand);
            case '?': return ENFA::Question(operand);
            default: throw std::invalid_argument("Invalid Operator");
        }
    }

    ENFA visitBinaryOp(antlr4::Token* token, ENFA&& l, ENFA&& r) {
        if (token == nullptr) return ENFA::Concat(l, r);
        if (token->getText()[0] != '|') {
            throw std::invalid_argument("Invalid Binary Operator");
        }
        return ENFA::Alternate(l, r);
    }

    ENFA visitChar(antlr4::tree::TerminalNode* node) {
        if (node == nullptr) throw std::invalid_argument("Invalid Character");
        return ENFA::Char(10, node->getText()[0] - '0');
    }

};



#endif //REGEXVISITOR_H
