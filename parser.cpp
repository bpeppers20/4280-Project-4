#include <iostream>
#include <cstdarg>
#include <vector>
#include "parser.h"

using namespace std;

const static int VA_LIST_TERMINATOR = -1;
const static bool NONTERMINAL_NODE = true;
const static bool TERMINAL_NODE = false;

static Token *tk = nullptr;

Parser::Parser(const string &rawData) : rawData(rawData) {
    scanner = new Scanner(rawData);
}

Parser::~Parser() = default;

const string &Parser::getRawData() const {
    return rawData;
}

void Parser::setRawData(const string &rawData) {
    Parser::rawData = rawData;
}

Node* Parser::parser() const { // aux function
    tk = scanner->getNextToken(); //scan the first token
    Node* root = program(ROOT_LEVEL); // Initialize tree with root and start parsing
    if(currentToken() == END_OF_FILE) {
       // cout << "Parsing was succesful!" << endl;
    }else {
        reportError(END_OF_FILE, -1);
    }
    return root;
}

Node* Parser::program( const int level ) const{ // <program>
    Node* node = new Node("program", NONTERMINAL_NODE, level, PROGRAM);
    if (currentToken() == KEYWORD_START) { // if current token is start
        node = consumeTerminal(node); //consume start
        node = consumeNonTerminal(node, vars( level + 1)); // <vars>
        if( currentToken() == KEYWORD_MAIN)      {
            node = consumeTerminal(node); // consume main
            node = consumeNonTerminal(node, block( level + 1));
            if(currentToken() == KEYWORD_STOP) {
                node = consumeTerminal(node);
                return node; // no other terminals or non terminals in the path
            }else {
                reportError(KEYWORD_STOP, VA_LIST_TERMINATOR);
            }
        }else {
            reportError(KEYWORD_MAIN, VA_LIST_TERMINATOR);
        }
    }else {
        reportError(KEYWORD_START, VA_LIST_TERMINATOR);
    }
    return node;
}

Node* Parser::block( const int level ) const{ //block
    Node* node = new Node("block", NONTERMINAL_NODE, level, BLOCK);
    if (currentToken() == DELIMITER_LEFT_CURLY_BRACE) {
        node = consumeTerminal(node);

        node = consumeNonTerminal(node, vars(level + 1)); // <vars>
        node = consumeNonTerminal(node, stats(level + 1)); // <stats>

        if (currentToken() == DELIMITER_RIGHT_CURLY_BRACE) {
            node = consumeTerminal(node);
            return node;
        }else {
            reportError(DELIMITER_RIGHT_CURLY_BRACE, VA_LIST_TERMINATOR);
        }
    } else {
        reportError(DELIMITER_LEFT_CURLY_BRACE, VA_LIST_TERMINATOR);
    }
    return node;
}

Node* Parser::vars( const int level ) const {
    Node* node = new Node("vars", NONTERMINAL_NODE, level, VARS);
    if (currentToken() == KEYWORD_LET) {
        node = consumeTerminal(node); //consume let
        if ( currentToken() == IDENTIFIER) {
            node = consumeTerminal(node);
            if (currentToken() == OPERATOR_COLON) {
                node = consumeTerminal(node);
                if (currentToken() == INTEGER) {
                    node = consumeTerminal(node);
                    node = consumeNonTerminal(node, vars(level + 1)); //<vars>
                    return node;
                } else {
                    reportError(INTEGER, VA_LIST_TERMINATOR);
                }
                
            }else {
                reportError(OPERATOR_COLON, VA_LIST_TERMINATOR);
            }
            
        } else {
            reportError(IDENTIFIER, VA_LIST_TERMINATOR);
        }
    }else
    {
       return node; // Predicitng empty
    }
}

Node* Parser::expr( const int level ) const{ // <expr>
    Node* node = new Node("expr", NONTERMINAL_NODE, level, EXPR);
    node = consumeNonTerminal(node, n(level + 1)); // <N>
    if (currentToken() == OPERATOR_FORWARD_SLASH || currentToken() == OPERATOR_ASTERISK) {
        node = consumeTerminal(node); // "/" or *
        node = consumeNonTerminal(node, expr(level + 1)); //. <expr>
        return node;
    } else {
       return node; // leaving only <N>
    }
}
Node* Parser::n( const int level ) const{
    Node* node =  new Node("N", NONTERMINAL_NODE, level, N); // <N>
    node = consumeNonTerminal(node, a(level + 1)); // <A>
    if (currentToken() == OPERATOR_PLUS || currentToken() == OPERATOR_MINUS) {
        node = consumeTerminal(node); // + or -
        node = consumeNonTerminal(node, n( level + 1));
        return node;
    } else {
       return node; // assume only <A>
    }
}

Node* Parser::a( const int level ) const{
    Node* node = new Node("A", NONTERMINAL_NODE, level, A);// <A>
    if (currentToken() == OPERATOR_PERCENT) {
        node = consumeTerminal(node);
        node = consumeNonTerminal(node, a( level + 1)); // <A>
        return node;
    } else {
        node = consumeNonTerminal(node, r( level + 1 )); // <R>
    }
    return node;
}

Node* Parser::r( const int level ) const{
    Node* node = new Node("R", NONTERMINAL_NODE, level, R);
    switch(currentToken()) {
        case DELIMITER_LEFT_SQUARE_BRACKET:
            node = consumeTerminal(node); // consume [
            node = consumeNonTerminal(node, expr( level + 1)); //<expr>
            if ( currentToken() == DELIMITER_RIGHT_SQUARE_BRACKET) {
                node = consumeTerminal(node); // consume ]
                return node;
            }else {
                reportError(DELIMITER_RIGHT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
            }
            break;
        case IDENTIFIER:
            node = consumeTerminal(node); // Consume Identfier Keyword
            return node;
            break;
        case INTEGER:
            node = consumeTerminal(node); // Consume Integer Keyword
            return node;
            break;
        default:
            reportError(UNKNOWN_ERROR, VA_LIST_TERMINATOR); // Any Terminal than the other 3 prior
    }
}


Node* Parser::stats( const int level ) const{ //<stats>
    Node* node = new Node("stats", NONTERMINAL_NODE, level, STATS); // <stats>
    node = consumeNonTerminal(node, stat( level + 1)); // <stat>
    node = consumeNonTerminal(node, mStat( level + 1)); // <mStat>
    return node;
}

Node* Parser::mStat( const int level ) const{ // <mStat>
    Node* node = new Node("mStat", NONTERMINAL_NODE, level, MSTAT); //<mStat>
    if( (currentToken() == KEYWORD_SCANF || currentToken() == KEYWORD_PRINTF || currentToken() == KEYWORD_IF || currentToken() == KEYWORD_ITER || currentToken() == IDENTIFIER)) {
        node = consumeNonTerminal(node, stat( level + 1)); // <stat>
        node = consumeNonTerminal(node, mStat( level + 1)); // mStat>
        return node;
    }
    return node; // Assume empty
}

Node* Parser::stat( const int level ) const{
    Node* node = new Node("stat", NONTERMINAL_NODE, level, STAT);
    switch (currentToken())
    {
        case KEYWORD_SCANF:
            node = consumeNonTerminal(node, in( level + 1)); // <in>
            if ( currentToken() == DELIMITER_PERIOD ) {
            node = consumeTerminal(node); // consume .
            return node;
            } else {
            reportError(DELIMITER_PERIOD, VA_LIST_TERMINATOR);
            }
            break;
        case KEYWORD_PRINTF:
            node = consumeNonTerminal(node, out( level + 1)); // <out>
            if ( currentToken() == DELIMITER_PERIOD ) {
            node = consumeTerminal(node); // consume .
            return node;
            } else {
            reportError(DELIMITER_PERIOD, VA_LIST_TERMINATOR);
            }
            break;
        case DELIMITER_LEFT_CURLY_BRACE:
            node = consumeNonTerminal(node, block( level + 1)); // <block>
            return node;
            break;
        case KEYWORD_IF:
            node = consumeNonTerminal(node, iF( level + 1)); // <if>
            if ( currentToken() == DELIMITER_PERIOD ) {
            node = consumeTerminal(node); // consume .
            return node;
            } else {
            reportError(DELIMITER_PERIOD, VA_LIST_TERMINATOR);
            }
            break;
        case KEYWORD_ITER:
            node = consumeNonTerminal(node, loop( level + 1)); // <loop>
            if ( currentToken() == DELIMITER_PERIOD ) {
            node = consumeTerminal(node); // consume .
            return node;
            } else {
            reportError(DELIMITER_PERIOD, VA_LIST_TERMINATOR);
            }
            break;
        case IDENTIFIER:
            node = consumeNonTerminal(node, assign( level + 1)); // <assign>
            if ( currentToken() == DELIMITER_PERIOD ) {
            node = consumeTerminal(node); // consume .
            return node;
            } else {
            reportError(DELIMITER_PERIOD, VA_LIST_TERMINATOR);
            }
            break;
        default:
            return node;
    }
}

Node* Parser::in( const int level ) const{ // <in>
    Node* node = new Node("in", NONTERMINAL_NODE, level, IN); // <in>
    if (currentToken() == KEYWORD_SCANF) {
        node = consumeTerminal(node); // consume scanf
        if (currentToken() == DELIMITER_LEFT_SQUARE_BRACKET) {
            node = consumeTerminal(node); //consume [
            if (currentToken() == IDENTIFIER) {
                node = consumeTerminal(node); //consume identifer
                if (currentToken() == DELIMITER_RIGHT_SQUARE_BRACKET) {
                    node = consumeTerminal(node);
                    return node;
                }else {
                    reportError(DELIMITER_RIGHT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
                }
            } else {
                reportError(IDENTIFIER, VA_LIST_TERMINATOR);
            }
        } else {
            reportError(DELIMITER_LEFT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
        }     
    } else {
        reportError(KEYWORD_SCANF, VA_LIST_TERMINATOR);
    }
}

Node* Parser::out( const int level ) const{ // <out>
    Node* node = new Node("out", NONTERMINAL_NODE, level, OUT); // <out>
    if (currentToken() == KEYWORD_PRINTF) {
        node = consumeTerminal(node); // consume printf
        if (currentToken() == DELIMITER_LEFT_SQUARE_BRACKET) {
            node = consumeTerminal(node); //consume [
            node = consumeNonTerminal(node, expr( level + 1)); // <expr>
            if (currentToken() == DELIMITER_RIGHT_SQUARE_BRACKET) {
                    node = consumeTerminal(node);
                    return node;
                }else {
                    reportError(DELIMITER_RIGHT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
                }
        } else {
            reportError(DELIMITER_LEFT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
        }     
    } else {
        reportError(KEYWORD_SCANF, VA_LIST_TERMINATOR);
    }
}

Node* Parser::iF( const int level ) const{ // <if>
    Node* node = new Node("if", NONTERMINAL_NODE, level, IF);
    if (currentToken() == KEYWORD_IF) {
        node = consumeTerminal(node); // consume if
        if (currentToken() == DELIMITER_LEFT_SQUARE_BRACKET) {
            node = consumeTerminal(node); // consume [
            node = consumeNonTerminal(node, expr( level + 1)); // <expr>
            node = consumeNonTerminal(node, Ro( level + 1)); // <expr>
            node = consumeNonTerminal(node, expr( level + 1)); // <expr>
            if (currentToken() == DELIMITER_RIGHT_SQUARE_BRACKET) {
                node = consumeTerminal(node); // consume ]
                if ( currentToken() == KEYWORD_THEN) {
                    node = consumeTerminal(node);
                    node = consumeNonTerminal(node, block( level + 1));
                    return node;
                } else {
                    reportError(KEYWORD_THEN, VA_LIST_TERMINATOR);
                }
            } else {
                    reportError(DELIMITER_RIGHT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
                }
        } else {
            reportError(DELIMITER_LEFT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
        }
    } else {
        reportError(KEYWORD_IF, VA_LIST_TERMINATOR);
    }
}


Node* Parser::loop( const int level ) const{ // <loop>
    Node* node = new Node("loop", NONTERMINAL_NODE, level, LOOP); // <loop>
    if (currentToken() == KEYWORD_ITER) {
        node = consumeTerminal(node); // consume iter
        if (currentToken() == DELIMITER_LEFT_SQUARE_BRACKET) {
            node = consumeTerminal(node); // consume [
            node = consumeNonTerminal(node, expr( level + 1)); // <expr>
            node = consumeNonTerminal(node, Ro( level + 1)); // <expr>
            node = consumeNonTerminal(node, expr( level + 1)); // <expr>
            if (currentToken() == DELIMITER_RIGHT_SQUARE_BRACKET) {
                node = consumeTerminal(node); // consume ]
                node = consumeNonTerminal(node, block( level + 1)); // <block>
                return node;
            } else {
                    reportError(DELIMITER_RIGHT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
                }
        } else {
            reportError(DELIMITER_LEFT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
        }
    } else {
        reportError(KEYWORD_IF, VA_LIST_TERMINATOR);
    }
}


Node* Parser::assign( const int level ) const{ // <assign>
    Node* node = new Node("assign", NONTERMINAL_NODE, level, ASSIGN); // <assign>
    if (currentToken() == IDENTIFIER) {
        node = consumeTerminal(node); // consume identifier
        if (currentToken() == OPERATOR_EQUALS) {
            node = consumeTerminal(node); // consume =
            node = consumeNonTerminal(node, expr( level + 1)); // <expr>
            return node;
        } else {
            reportError(OPERATOR_EQUALS, VA_LIST_TERMINATOR);
        }
    } else {
        reportError(IDENTIFIER, VA_LIST_TERMINATOR);
    }
}


Node* Parser::Ro( const int level) const{ // RO
    Node *node = new Node("RO", NONTERMINAL_NODE, level, RO); // <RO>
    switch (currentToken()) {
        case OPERATOR_LESS_THAN:
        case OPERATOR_GREATER_THAN:
        case OPERATOR_DOUBLE_EQUALS: 
            node = consumeTerminal(node); // consume <,<=,>,=>,==
            return node;
            break;
        case OPERATOR_EQUALS:
            node = consumeTerminal(node); // consume =
            if (currentToken() == OPERATOR_GREATER_THAN || currentToken() == OPERATOR_LESS_THAN) {
                node = consumeTerminal(node); // consume < or >
                return node;
            }
            break;
        case OPERATOR_COLON: 
            node = consumeTerminal(node); // consume :
            if (currentToken() == OPERATOR_COLON) {
                node = consumeTerminal(node); // consume :
                return node;
            }else {
                reportError(OPERATOR_COLON, VA_LIST_TERMINATOR);
            }
            break;
        default:
            reportError(OPERATOR_LESS_THAN, OPERATOR_LESS_THAN_OR_EQUAL_TO, OPERATOR_GREATER_THAN, OPERATOR_GREATER_THAN_OR_EQUAL_TO, OPERATOR_DOUBLE_EQUALS, OPERATOR_COLON, VA_LIST_TERMINATOR);
    }
}

const TOKEN_IDENTIFIER Parser::currentToken() const {
    return tk->getTokenIdentifier();
}


Node *Parser::consumeTerminal(Node *node) const { // if a terminal then consume it
    vector<Node *> children = node->getChildren();
    children.push_back(new Node(tk->getValue(), TERMINAL_NODE, node->getLevel() + 1, TERMINAL));
    node->setChildren(children);
    tk = scanner->getNextToken();

    return node;
}

Node *Parser::consumeNonTerminal(Node *originalNode, Node *nonTerminalNode) const { //If a nonterminal, consume it then move to the next function
    vector<Node *> originalNodesChildren = originalNode->getChildren();
    originalNodesChildren.push_back(nonTerminalNode);
    originalNode->setChildren(originalNodesChildren);

    return originalNode;
}
const void Parser::reportError(TOKEN_IDENTIFIER expectedToken, ...) const {
    string listOfExpectedTokens;

    va_list va_list;
    va_start(va_list, expectedToken);

    while (expectedToken != VA_LIST_TERMINATOR) {
        listOfExpectedTokens += "\"" + TOKEN_IDENTIFIER_TO_TOKEN_NAME_MAP.at(expectedToken) + "\" or ";
        expectedToken = static_cast<TOKEN_IDENTIFIER>(va_arg(va_list, int));
    }

    listOfExpectedTokens = listOfExpectedTokens.substr(0, listOfExpectedTokens.length() - 4);

    cerr << "PARSE ERROR: Got token \"" << TOKEN_IDENTIFIER_TO_TOKEN_NAME_MAP.at(currentToken()) << "\" but " << listOfExpectedTokens << " was expected on line \"" << scanner->getCurrentLineNumber() << "\".\n";
    exit(1);
}

