#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "scanner.h"
#include "node.h"
#include "nonterminal.h"
#include "scope.h"

class Parser {
// Where all the BNF functions are defined
    private:
        std::string rawData;
        Scanner *scanner;
    public:
        explicit Parser(const std::string &rawData);
        ~Parser();
        // Make return type node for node tree later
        const std::string &getRawData() const;

        void setRawData(const std::string &rawData);
        // Period is considered a semicolon in this language
        Node* parser() const; // auxiliary function 
        Node* program( const int level ) const; // <program> -> start <vars> main <block> stop
        Node* block( const int level ) const; // <block> -> { <vars> <stats> }
        Node* vars( const int level ) const; // <vars> -> empty | let Identifier :  Integer    <vars>
        Node* expr( const int level ) const; // <expr> -> <N> / <expr>  | <N> * <expr> | <N>
        Node* n( const int level ) const; // <N> -> <A> + <N> | <A> - <N> | <A>
        Node* a( const int level ) const; // <A> -> % <A> |  <R>
        Node* r( const int level ) const; // <R> -> [ <expr> ] | Identifier | Integer
        Node* stats( const int level ) const; // <stats> -> <stat>  <mStat>
        Node* mStat( const int level ) const; // <mStat> -> empty | <stat> <mStat>
        Node* stat( const int level ) const; // <stat> -> <in>. | <out>. | <block> | <if>. | <loop>. | <assign>.
        Node* in( const int level ) const; // <in> -> scanf [identifier]
        Node* out( const int level) const; // <out> -> printf [<expr>]
        Node* iF( const int level ) const; // <if> -> if [ <expr> <RO> <expr> ] then <block> 
        Node* loop( const int level ) const; // <loop> -> iter [ <expr> <RO> <expr> ] <block>
        Node* assign( const int level ) const; // <assign> -> identifier = <expr>
        Node* Ro( const int level ) const; // <RO> -> =< | => | == | : :
        const void reportError(TOKEN_IDENTIFIER expectedToken, ...) const;
        const TOKEN_IDENTIFIER currentToken() const;
        Node *consumeTerminal(Node *node) const;
        Node *consumeNonTerminal(Node *originalNode, Node *nonTerminalNode) const;
};

#endif //PARSER_H