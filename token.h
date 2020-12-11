#ifndef P1_TOKEN_H
#define P1_TOKEN_H
#include <map>
#include <string>
#include "util.h"

// 8 points of interest

enum TOKEN_IDENTIFIER {
    END_OF_FILE, //Indicates the end of the file has been reached.
    IDENTIFIER, //Conforms to the regex (JavaScript notation) "/^[a-zA-Z]{1}[a-zA-Z0-9]{0,7}$/".
    INTEGER, //Conforms to the regex (JavaScript notation) "/^[0-9]{1,8}$/".
    KEYWORD_START, //"start"
    KEYWORD_STOP, // "stop"
    KEYWORD_ITER, //"iter"
    KEYWORD_VOID, //"void"
    KEYWORD_LET, //"let"
    KEYWORD_RETURN, //"return"
    KEYWORD_FUNC, //"func"
    KEYWORD_INT, // int
    KEYWORD_PRINTF, // printf
    KEYWORD_SCANF, //scanf
    KEYWORD_MAIN, //main
    KEYWORD_IF, //if
    KEYWORD_THEN, //then
    KEYWORD_DATA, //data
    KEYWORD_EXIT, // exit
    OPERATOR_EQUALS, //"="
    OPERATOR_LESS_THAN, //"<"
    OPERATOR_LESS_THAN_OR_EQUAL_TO, //"<="
    OPERATOR_GREATER_THAN, //">"
    OPERATOR_GREATER_THAN_OR_EQUAL_TO, //">="
    ILLEGAL_EXCLAMATION_POINT, //"!"
    OPERATOR_EXCLAMATION_POINT_EQUALS, //"!="
    OPERATOR_DOUBLE_EQUALS, //"=="
    OPERATOR_COLON, //":"
    OPERATOR_PLUS, //"+"
    OPERATOR_MINUS, //"-"
    OPERATOR_ASTERISK, //"*"
    OPERATOR_FORWARD_SLASH, //"/"
    OPERATOR_AMPERSAND, //"&"
    OPERATOR_PERCENT, //"%"
    DELIMITER_PERIOD, //"."
    DELIMITER_LEFT_PARENTHESIS, //"("
    DELIMITER_RIGHT_PARENTHESIS, //")"
    DELIMITER_LEFT_CURLY_BRACE, //"{"
    DELIMITER_RIGHT_CURLY_BRACE, //"}"
    DELIMITER_LEFT_SQUARE_BRACKET, //"["
    DELIMITER_RIGHT_SQUARE_BRACKET, //"]"
    DELIMITER_COMMA, //","
    DELIMITER_SEMICOLON, //";"
    UNKNOWN_ERROR // for hard to identify errors
};

const std::string END_OF_FILE_TOKEN_NAME = "END_OF_FILE";
const std::string IDENTIFIER_TOKEN_NAME = "IDENTIFIER";
const std::string INTEGER_TOKEN_NAME = "INTEGER";
const std::string KEYWORD_START_TOKEN_NAME = "KEYWORD_START";
const std::string KEYWORD_STOP_TOKEN_NAME = "KEYWORD_STOP";
const std::string KEYWORD_CHECK_TOKEN_NAME = "KEYWORD_CHECK";
const std::string KEYWORD_ITER_TOKEN_NAME = "KEYWORD_ITER";
const std::string KEYWORD_VOID_TOKEN_NAME = "KEYWORD_VOID";
const std::string KEYWORD_LET_TOKEN_NAME = "KEYWORD_LET";
const std::string KEYWORD_RETURN_TOKEN_NAME = "KEYWORD_RETURN";
const std::string KEYWORD_INPUT_TOKEN_NAME = "KEYWORD_INPUT";
const std::string KEYWORD_OUTPUT_TOKEN_NAME = "KEYWORD_OUTPUT";
const std::string KEYWORD_FUNC_TOKEN_NAME = "KEYWORD_FUNC";
const std::string KEYWORD_INT_TOKEN_NAME = "KEYWORD_INT"; // int
const std::string KEYWORD_PRINTF_TOKEN_NAME = "KEYWORD_PRINTF"; // printf
const std::string KEYWORD_SCANF_TOKEN_NAME = "KEYWORD_SCANF"; //scanf
const std::string KEYWORD_MAIN_TOKEN_NAME = "KEYWORD_MAIN"; //main
const std::string KEYWORD_IF_TOKEN_NAME = "KEYWORD_IF"; //if
const std::string KEYWORD_THEN_TOKEN_NAME = "KEYWORD_THEN"; //then
const std::string KEYWORD_DATA_TOKEN_NAME = "KEYWORD_DATA"; //data
const std::string KEYWORD_EXIT_TOKEN_NAME = "KEYWORD_EXIT"; //exit
const std::string OPERATOR_EQUALS_TOKEN_NAME = "OPERATOR_EQUALS";
const std::string OPERATOR_LESS_THAN_TOKEN_NAME = "OPERATOR_LESS_THAN";
const std::string OPERATOR_LESS_THAN_OR_EQUAL_TO_TOKEN_NAME = "OPERATOR_LESS_THAN_OR_EQUAL_TO";
const std::string OPERATOR_GREATER_THAN_TOKEN_NAME = "OPERATOR_GREATER_THAN";
const std::string OPERATOR_GREATER_THAN_OR_EQUAL_TO_TOKEN_NAME = "OPERATOR_GREATER_THAN_OR_EQUAL_TO";
const std::string ILLEGAL_EXCLAMATION_POINT_TOKEN_NAME = "ILLEGAL_EXCLAMATION_POINT";
const std::string OPERATOR_EXCLAMATION_POINT_EQUALS_TOKEN_NAME = "OPERATOR_EXCLAMATION_POINT_EQUALS";
const std::string OPERATOR_DOUBLE_EQUALS_TOKEN_NAME = "OPERATOR_DOUBLE_EQUALS";
const std::string OPERATOR_COLON_TOKEN_NAME = "OPERATOR_COLON";
const std::string OPERATOR_PLUS_TOKEN_NAME = "OPERATOR_PLUS";
const std::string OPERATOR_MINUS_TOKEN_NAME = "OPERATOR_MINUS";
const std::string OPERATOR_ASTERISK_TOKEN_NAME = "OPERATOR_ASTERISK";
const std::string OPERATOR_FORWARD_SLASH_TOKEN_NAME = "OPERATOR_FORWARD_SLASH";
const std::string OPERATOR_AMPERSAND_TOKEN_NAME = "OPERATOR_AMPERSAND";
const std::string OPERATOR_PERCENT_TOKEN_NAME = "OPERATOR_PERCENT";
const std::string DELIMITER_PERIOD_TOKEN_NAME = "DELIMITER_PERIOD";
const std::string DELIMITER_LEFT_PARENTHESIS_TOKEN_NAME = "DELIMITER_LEFT_PARENTHESIS";
const std::string DELIMITER_RIGHT_PARENTHESIS_TOKEN_NAME = "DELIMITER_RIGHT_PARENTHESIS";
const std::string DELIMITER_LEFT_CURLY_BRACE_TOKEN_NAME = "DELIMITER_LEFT_CURLY_BRACE";
const std::string DELIMITER_RIGHT_CURLY_BRACE_TOKEN_NAME = "DELIMITER_RIGHT_CURLY_BRACE";
const std::string DELIMITER_LEFT_SQUARE_BRACKET_TOKEN_NAME = "DELIMITER_LEFT_SQUARE_BRACKET";
const std::string DELIMITER_RIGHT_SQUARE_BRACKET_TOKEN_NAME = "DELIMITER_RIGHT_SQUARE_BRACKET";
const std::string DELIMITER_COMMA_TOKEN_NAME = "DELIMITER_COMMA";
const std::string DELIMITER_SEMICOLON_TOKEN_NAME = "DELIMITER_SEMICOLON";
const std::string UNKNOWN_ERROR_NAME = "UNKNOWN_ERROR";

const std::map<const TOKEN_IDENTIFIER, const std::string> TOKEN_IDENTIFIER_TO_TOKEN_NAME_MAP = {
        {END_OF_FILE,                       END_OF_FILE_TOKEN_NAME},
        {IDENTIFIER,                        IDENTIFIER_TOKEN_NAME},
        {INTEGER,                           INTEGER_TOKEN_NAME},
        {KEYWORD_START,                     KEYWORD_START_TOKEN_NAME},
        {KEYWORD_STOP,                      KEYWORD_STOP_TOKEN_NAME},
        {KEYWORD_ITER,                      KEYWORD_ITER_TOKEN_NAME},
        {KEYWORD_VOID,                      KEYWORD_VOID_TOKEN_NAME},
        {KEYWORD_LET,                       KEYWORD_LET_TOKEN_NAME},
        {KEYWORD_RETURN,                    KEYWORD_RETURN_TOKEN_NAME},
        {KEYWORD_FUNC,                      KEYWORD_FUNC_TOKEN_NAME},
        {KEYWORD_INT,                       KEYWORD_INT_TOKEN_NAME}, // int
        {KEYWORD_PRINTF,                    KEYWORD_PRINTF_TOKEN_NAME}, // printf
        {KEYWORD_SCANF,                     KEYWORD_SCANF_TOKEN_NAME}, //scanf
        {KEYWORD_MAIN,                      KEYWORD_MAIN_TOKEN_NAME}, //main
        {KEYWORD_IF,                        KEYWORD_IF_TOKEN_NAME}, //if
        {KEYWORD_THEN,                      KEYWORD_THEN_TOKEN_NAME}, //then
        {KEYWORD_DATA,                      KEYWORD_DATA_TOKEN_NAME},//data
        {KEYWORD_EXIT,                      KEYWORD_EXIT_TOKEN_NAME},
        {OPERATOR_EQUALS,                   OPERATOR_EQUALS_TOKEN_NAME},
        {OPERATOR_LESS_THAN,                OPERATOR_LESS_THAN_TOKEN_NAME},
        {OPERATOR_LESS_THAN_OR_EQUAL_TO,    OPERATOR_LESS_THAN_OR_EQUAL_TO_TOKEN_NAME},
        {OPERATOR_GREATER_THAN,             OPERATOR_GREATER_THAN_TOKEN_NAME},
        {OPERATOR_GREATER_THAN_OR_EQUAL_TO, OPERATOR_GREATER_THAN_OR_EQUAL_TO_TOKEN_NAME},
        {ILLEGAL_EXCLAMATION_POINT,         ILLEGAL_EXCLAMATION_POINT_TOKEN_NAME},
        {OPERATOR_EXCLAMATION_POINT_EQUALS, OPERATOR_EXCLAMATION_POINT_EQUALS_TOKEN_NAME},
        {OPERATOR_DOUBLE_EQUALS,            OPERATOR_DOUBLE_EQUALS_TOKEN_NAME},
        {OPERATOR_COLON,                    OPERATOR_COLON_TOKEN_NAME},
        {OPERATOR_PLUS,                     OPERATOR_PLUS_TOKEN_NAME},
        {OPERATOR_MINUS,                    OPERATOR_MINUS_TOKEN_NAME},
        {OPERATOR_ASTERISK,                 OPERATOR_ASTERISK_TOKEN_NAME},
        {OPERATOR_FORWARD_SLASH,            OPERATOR_FORWARD_SLASH_TOKEN_NAME},
        {OPERATOR_AMPERSAND,                OPERATOR_AMPERSAND_TOKEN_NAME},
        {OPERATOR_PERCENT,                  OPERATOR_PERCENT_TOKEN_NAME},
        {DELIMITER_PERIOD,                  DELIMITER_PERIOD_TOKEN_NAME},
        {DELIMITER_LEFT_PARENTHESIS,        DELIMITER_LEFT_PARENTHESIS_TOKEN_NAME},
        {DELIMITER_RIGHT_PARENTHESIS,       DELIMITER_RIGHT_PARENTHESIS_TOKEN_NAME},
        {DELIMITER_LEFT_CURLY_BRACE,        DELIMITER_LEFT_CURLY_BRACE_TOKEN_NAME},
        {DELIMITER_RIGHT_CURLY_BRACE,       DELIMITER_RIGHT_CURLY_BRACE_TOKEN_NAME},
        {DELIMITER_LEFT_SQUARE_BRACKET,     DELIMITER_LEFT_SQUARE_BRACKET_TOKEN_NAME},
        {DELIMITER_RIGHT_SQUARE_BRACKET,    DELIMITER_RIGHT_SQUARE_BRACKET_TOKEN_NAME},
        {DELIMITER_COMMA,                   DELIMITER_COMMA_TOKEN_NAME},
        {DELIMITER_SEMICOLON,               DELIMITER_SEMICOLON_TOKEN_NAME},
        {UNKNOWN_ERROR,                     UNKNOWN_ERROR_NAME}
};



// keywords
const std::string KEYWORD_LITERAL_START = "start";
const std::string KEYWORD_LITERAL_STOP = "stop";
const std::string KEYWORD_LITERAL_ITER = "iter";
const std::string KEYWORD_LITERAL_VOID = "void";
const std::string KEYWORD_LITERAL_LET = "let";
const std::string KEYWORD_LITERAL_RETURN = "return";
const std::string KEYWORD_LITERAL_FUNC = "func";
const std::string KEYWORD_LITERAL_INT = "int"; // int
const std::string KEYWORD_LITERAL_PRINTF = "printf"; // printf
const std::string KEYWORD_LITERAL_SCANF = "scanf"; //scanf
const std::string KEYWORD_LITERAL_MAIN = "main"; //main
const std::string KEYWORD_LITERAL_IF = "if"; //if
const std::string KEYWORD_LITERAL_THEN = "then"; //then
const std::string KEYWORD_LITERAL_DATA = "data"; //data
const std::string KEYWORD_LITERAL_EXIT = "exit";

const std::map<const std::string, const TOKEN_IDENTIFIER> KEYWORD_LITERAL_TO_KEYWORD_TOKEN_IDENTIFIER_MAP = {
        {KEYWORD_LITERAL_START,   KEYWORD_START},
        {KEYWORD_LITERAL_STOP,    KEYWORD_STOP},
        {KEYWORD_LITERAL_ITER,    KEYWORD_ITER},
        {KEYWORD_LITERAL_VOID,    KEYWORD_VOID},
        {KEYWORD_LITERAL_LET,     KEYWORD_LET},
        {KEYWORD_LITERAL_RETURN,  KEYWORD_RETURN},
        {KEYWORD_LITERAL_FUNC,    KEYWORD_FUNC},
        {KEYWORD_LITERAL_INT,     KEYWORD_INT}, // int
        {KEYWORD_LITERAL_PRINTF,  KEYWORD_PRINTF}, // printf
        {KEYWORD_LITERAL_SCANF,   KEYWORD_SCANF}, //scanf
        {KEYWORD_LITERAL_MAIN,    KEYWORD_MAIN}, //main
        {KEYWORD_LITERAL_IF,      KEYWORD_IF}, //if
        {KEYWORD_LITERAL_THEN,    KEYWORD_THEN}, //then
        {KEYWORD_LITERAL_DATA,    KEYWORD_DATA}, //data
        {KEYWORD_LITERAL_EXIT,    KEYWORD_EXIT}
};


class Token {
private:
    const TOKEN_IDENTIFIER tokenIdentifier;
    const std::string value;
    const std::string lineNumber;
public:
    Token(const TOKEN_IDENTIFIER tokenIdentifier, const std::string value, const std::string lineNumber) : tokenIdentifier(tokenIdentifier),
                                                                                                           value(value),
                                                                                                           lineNumber(lineNumber) {
    };

    ~Token() = default;

   const TOKEN_IDENTIFIER getTokenIdentifier() const{
        return tokenIdentifier;
    }

    const std::string &getValue() const{
        return value;
    }

    const std::string &getLineNumber() const{
        return lineNumber;
    }

    const std::string toString() const{
        return "Line Number: \"" + getLineNumber() + "\", Token Identifier: \"" + TOKEN_IDENTIFIER_TO_TOKEN_NAME_MAP.at(getTokenIdentifier()) + "\", Token Value: \"" + trim(getValue()) + "\"";
    }
};

#endif
