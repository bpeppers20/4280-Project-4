#include<string>
#include "code-generator.h"

using namespace std;


std::vector<std::string> variableList; // keep track of all variables used for allocation
CodeGenerator::CodeGenerator(const Node *parseTree) : parseTree(parseTree) {
    scopes->push_back(new Scope(0));
}

CodeGenerator::~CodeGenerator() = default;

const string CodeGenerator::generateCode() {
    string generatedCode = generateCodeForNode(parseTree);
    generatedCode += generateSegmentCode();
    generatedCode += CMD_STOP + NEW_LINE;
    generatedCode += generateStorageAllocationCode();

   // cout << "No semantical errors found!\n";
   // cout << "Code generation completed successfully!\n\n";
    return generatedCode;
}

const string CodeGenerator::generateCodeForNode(const Node *node) {
    string generatedCode;

    if (node->isNonTerminal()) {
        const vector<Node *> &childNodes = node->getChildren();
        if (node->getNonTerminalIdentifier() == PROGRAM) { // ✓
            if (childNodes.size() == 5) {
                // Child 0: start
                // Child 1: <vars>
                // Child 2: main
                // Child 3: <block>
                // Child 4: stop
                generatedCode += generateCodeForNode(childNodes.at(1));
                generatedCode += generateCodeForNode(childNodes.at(3));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"PROGRAM\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == BLOCK) { // ✓
            if (childNodes.size() == 4 && childNodes.at(0)->getValue() == "{") {
                // Child 0: {
                // Child 1: <vars>
                // Child 2: <stats>
                // Child 3: }

                scopes->push_back(new Scope(scopes->size()));

                generatedCode += generateCodeForNode(childNodes.at(1));
                generatedCode += generateCodeForNode(childNodes.at(2));

                //scopes->pop_back();
            } else {
                cerr << "Sematical Error: No grammar path was recognized during processing of \"BLOCK\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == VARS) { // ✓
            if (childNodes.empty()) {
                // nothing if $empty
            } else if (childNodes.size() == 5 && childNodes.at(0)->getValue() == "let") { // ✓
                // Child 0: let
                // Child 1: $identifier
                // Child 2: :
                // Child 3: Integer
                // Child 4: <vars>
                if (variableUniqueToThisScope(childNodes.at(1)->getValue())) {
                    vector<string> *variableNames = scopes->at(scopes->size() - 1)->getVariableNames();
                    variableNames->push_back(childNodes.at(1)->getValue());
                    //variableList.push_back(childNodes.at(1)->getValue());
                    scopes->at(scopes->size() - 1)->setVariableNames(variableNames);

                    vector<string> *throwAwayVariableNames = scopes->at(scopes->size() - 1)->getThrowAwayVariableNames();
                    throwAwayVariableNames->push_back(generateThrowAwayIdentifier());
                    scopes->at(scopes->size() - 1)->setThrowAwayVariableNames(throwAwayVariableNames);
                } else {
                    cerr << "Code Generator Error: Cannot create variable \"" + childNodes.at(1)->getValue() + "\", because it is already defined in this scope!\n";
                    exit(-1);
                }

                if (isDistinct(childNodes.at(1)->getValue())) {
                    variables->push_back(childNodes.at(1)->getValue());
                    variableList.push_back(childNodes.at(1)->getValue());
                }
                generatedCode += generateCodeForNode(childNodes.at(4));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"VARS\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == EXPR) { // ✓
            if (childNodes.size() == 3 && childNodes.at(1)->getValue() == "/") {
                // Child 0: <N>
                // Child 1: /
                // Child 2: <expr>
                generatedCode += generateCodeForNode(childNodes.at(2));
                const string throwAwayIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + throwAwayIdentifier + NEW_LINE;
                generatedCode += generateCodeForNode(childNodes.at(0));
                generatedCode += CMD_DIV + SPACE + throwAwayIdentifier + NEW_LINE; // Divide
            } else if (childNodes.size() == 3 && childNodes.at(1)->getValue() == "*") {
                // Child 0: <N>
                // Child 1: *
                // Child 2: <expr>
                generatedCode += generateCodeForNode(childNodes.at(2));
                const string throwAwayIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + throwAwayIdentifier + NEW_LINE;
                generatedCode += generateCodeForNode(childNodes.at(0));
                generatedCode += CMD_MULT + SPACE + throwAwayIdentifier + NEW_LINE;
            } else if (childNodes.size() == 1) {
                // Child 0: <N>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"EXPR\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == N) { // ✓
            if (childNodes.size() == 3 && childNodes.at(1)->getValue() == "+") {
                // Child 0: <A>
                // Child 1: +
                // Child 2: <N>
                generatedCode += generateCodeForNode(childNodes.at(2));
                const string throwAwayIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + throwAwayIdentifier + NEW_LINE;
                generatedCode += generateCodeForNode(childNodes.at(0));
                generatedCode += CMD_ADD + SPACE + throwAwayIdentifier + NEW_LINE;
            } else if (childNodes.size() == 3 && childNodes.at(1)->getValue() == "-") { // ✓
                // Child 0: <A>
                // Child 1: -
                // Child 2: <N>
                generatedCode += generateCodeForNode(childNodes.at(2));
                const string throwAwayIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + throwAwayIdentifier + NEW_LINE;
                generatedCode += generateCodeForNode(childNodes.at(0));
                generatedCode += CMD_SUB + SPACE + throwAwayIdentifier + NEW_LINE;
            } else if (childNodes.size() == 1) {
                // Child 0: <N>
                generatedCode += generateCodeForNode(childNodes.at(0));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"M\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == A) { // <A> negation✓
            if (childNodes.size() == 2 && childNodes.at(0)->getValue() == "%") {
                // Child 0: %
                // Child 1: <A>

                generatedCode += generateCodeForNode(childNodes.at(1)); // Load variable
                const string throwAwayIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + throwAwayIdentifier + NEW_LINE;
                // Generate Negation
                 generatedCode += CMD_MULT + SPACE + "-1" + NEW_LINE;
                
            } else if (childNodes.size() == 1) {
                // Child 0: <R>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"F\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == R) { // Has Identifier ✓
            if (childNodes.size() == 3 && childNodes.at(0)->getValue() == "[") {
                // Child 0: [
                // Child 1: <expr>
                // Child 2: ]

                generatedCode += generateCodeForNode(childNodes.at(1));
            } else if (childNodes.size() == 1 && isalpha(childNodes.at(0)->getValue()[0])) {
                // Child 0: $identifier

                generatedCode += CMD_LOAD + SPACE + resolveThrowAwayVariableName(childNodes.at(0)->getValue()) + NEW_LINE;
            } else if (childNodes.size() == 1 && isdigit(childNodes.at(0)->getValue()[0])) {
                // Child 0: $number

                generatedCode += CMD_LOAD + SPACE + childNodes.at(0)->getValue() + NEW_LINE;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"R\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == STATS) { // ✓
            if (childNodes.size() == 2) {
                // Child 0: <stat>
                // Child 1: <mStat>

                generatedCode += generateCodeForNode(childNodes.at(0));
                generatedCode += generateCodeForNode(childNodes.at(1));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"STATS\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == MSTAT) { // ✓
            if (childNodes.empty()) {
                // $empty
            } else if (childNodes.size() == 2 && childNodes.at(0)->getValue() == "stat") {
                // Child 0: <stat>
                // Child 1: <mStat>

                generatedCode += generateCodeForNode(childNodes.at(0));
                generatedCode += generateCodeForNode(childNodes.at(1));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"MSTAT\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == STAT) { // ✓
            if (childNodes.size() == 2 && childNodes.at(0)->getValue() == "in") {
                // Child 0: <in>
                // Child 1: .
                generatedCode += generateCodeForNode(childNodes.at(0));
            } else if (childNodes.size() == 2 && childNodes.at(0)->getValue() == "out") {
                // Child 0: <out>
                // Child 1: .
                generatedCode += generateCodeForNode(childNodes.at(0));
            } else if (childNodes.size() == 2 && childNodes.at(0)->getValue() == "block") {
                // Child 0: <block>

                generatedCode += generateCodeForNode(childNodes.at(0));
            } else if (childNodes.size() == 2 && childNodes.at(0)->getValue() == "if") {
                // Child 0: <if>
                // Child 1: .
                generatedCode += generateCodeForNode(childNodes.at(0));
            } else if (childNodes.size() == 2 && childNodes.at(0)->getValue() == "loop") {
                // Child 0: <loop>
                // Child 1: .
                generatedCode += generateCodeForNode(childNodes.at(0));
            } else if (childNodes.size() == 2 && childNodes.at(0)->getValue() == "assign") {
                // Child 0: <assign>
                // Child 1: .
                generatedCode += generateCodeForNode(childNodes.at(0));
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"STAT\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == IN) { // ✓
            if (childNodes.size() == 4) {
                // Child 0: scanf
                // Child 1: [
                // Child 2: $identifier
                // Child 3: ]

                generatedCode += CMD_READ + SPACE + resolveThrowAwayVariableName(childNodes.at(2)->getValue()) + NEW_LINE;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"IN\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == OUT) { // ✓
            if (childNodes.size() == 4) {
                // Child 0: printf
                // Child 1: [
                // Child 2: <expr>
                // Child 3: ]

                generatedCode += generateCodeForNode(childNodes.at(2));
                const string throwAwayIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + throwAwayIdentifier + NEW_LINE;
                generatedCode += CMD_WRITE + SPACE + throwAwayIdentifier + NEW_LINE;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"OUT\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == IF) { // ✓
            if (childNodes.size() == 8) {
                // Child 0: if
                // Child 1: [
                // Child 2: <expr>
                // Child 3: <RO>
                // Child 4: <expr>
                // Child 5: ]
                // Child 6: then
                // Child 7: <block>
                const string parentThrowAwayLabel = generateThrowAwayLabel();
                string labelCode = parentThrowAwayLabel + COLON + SPACE + CMD_NOOP + NEW_LINE; // to be stored at the end
                const string throwAwayIdentifier = generateThrowAwayIdentifier();
                const string generatedSegmentCode =  generateCodeForNode(childNodes.at(4))
                                                    + CMD_STORE + SPACE + throwAwayIdentifier + NEW_LINE
                                                    + generateCodeForNode(childNodes.at(2))
                                                    + CMD_SUB + SPACE + throwAwayIdentifier + NEW_LINE
                                                    + generateCodeForNode(childNodes.at(3)) + SPACE + parentThrowAwayLabel + NEW_LINE
                                                    + generateCodeForNode(childNodes.at(7));
                generatedSegments->insert(generatedSegments->begin(), generatedSegmentCode);
                generatedSegments->push_back(labelCode); // place labels at end of segment
              //  generatedCode += CMD_STOP + NEW_LINE; segmentThrowAwayLabel + COLON + SPACE + NEW_LINE
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"IF\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == LOOP) { // ✓
            if (childNodes.size() == 7) {
                // Child 0: iter
                // Child 1: [
                // Child 2: <expr>
                // Child 3: <RO>
                // Child 4: <expr>
                // Child 5: ]
                // Child 6: <block>

                const string segmentThrowAwayLabel = generateThrowAwayLabel();
                const string parentThrowAwayLabel = generateThrowAwayLabel();
                string labelCode = parentThrowAwayLabel + COLON + SPACE + CMD_NOOP + NEW_LINE;
                string branchCode = CMD_BR + SPACE + segmentThrowAwayLabel + NEW_LINE;
                const string throwAwayIdentifier = generateThrowAwayIdentifier();
                const string generatedSegmentCode = segmentThrowAwayLabel + COLON + SPACE //label we return to/ start with
                                                    + generateCodeForNode(childNodes.at(4))
                                                    + CMD_STORE + SPACE + throwAwayIdentifier + NEW_LINE
                                                    + generateCodeForNode(childNodes.at(2))
                                                    + CMD_SUB + SPACE + throwAwayIdentifier + NEW_LINE
                                                    + generateCodeForNode(childNodes.at(3)) + SPACE + parentThrowAwayLabel + NEW_LINE
                                                    + generateCodeForNode(childNodes.at(6));
                generatedSegments->insert(generatedSegments->begin(), generatedSegmentCode); // start of segment this part was
                generatedSegments->push_back(branchCode); // place braches at end of segment
                generatedSegments->push_back(labelCode); // place labels at end of segment
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"LOOP\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == ASSIGN) { // ✓
            if (childNodes.size() == 3) {
                // Child 0: $identifier
                // Child 1: =
                // Child 2: <expr>

                generatedCode += generateCodeForNode(childNodes.at(2));
                generatedCode += CMD_STORE + SPACE + resolveThrowAwayVariableName(childNodes.at(0)->getValue()) + NEW_LINE;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"ASSIGN\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == RO) {
            if (childNodes.size() == 2 && childNodes.at(1)->getValue() == "<") {
                // Child 0: <=
                generatedCode += CMD_BRPOS;
            }else if (childNodes.size() == 2 && childNodes.at(1)->getValue() == ">") {
                // Child 0: >=

                generatedCode += CMD_BRNEG;
            } else if (childNodes.size() == 1 && childNodes.at(0)->getValue() == "==") {
                // Child 0: ==
                const string throwAwayIdentifier = generateThrowAwayIdentifier();
                generatedCode += CMD_STORE + SPACE + throwAwayIdentifier + NEW_LINE;
                generatedCode += CMD_MULT + SPACE + throwAwayIdentifier + NEW_LINE;
                generatedCode += CMD_BRPOS;
            } else if (childNodes.size() == 2 && childNodes.at(0)->getValue() == ":") {
                // Child 0: !=
                generatedCode += CMD_BRZERO;
            } else {
                cerr << "Code Generator Error: No grammar path was recognized during processing of \"RO\" node.\n";
            }
        } else if (node->getNonTerminalIdentifier() == TERMINAL) {
            // This should not be possible to reach, but is included for consistency anyway.
            cerr << "Code Generator Error: Processing of non-terminal node is marked as terminal. This is unexpected behavior!\n";
        } else {
            cerr << "Code Generator Error: Processing of non-terminal node is unrecognized. Marked as value \"" + to_string(node->getNonTerminalIdentifier()) + "\n";
        }
    }

    return generatedCode;
}

const string CodeGenerator::generateThrowAwayIdentifier() {
    return THROW_AWAY_IDENTIFIER_PREFIX + to_string(quantityThrowAwayIdentifiers++);
}

const string CodeGenerator::generateThrowAwayLabel() {
    return THROW_AWAY_LABEL_PREFIX + to_string(quantityThrowAwayLabels++);
}

const string CodeGenerator::generateSegmentCode() const {
    string generatedSegmentCode;

    for (const string &generatedSegment : *generatedSegments) {
        generatedSegmentCode += generatedSegment;
       // generatedSegmentCode += CMD_STOP + NEW_LINE;
    }

    return generatedSegmentCode;
}

const string CodeGenerator::generateStorageAllocationCode() const {
    string generatedStorageAllocationCode;

    for (int throwAwayIdentifierIndex = 0; throwAwayIdentifierIndex < variableList.size(); throwAwayIdentifierIndex++) {
        generatedStorageAllocationCode += variableList.at(throwAwayIdentifierIndex) + SPACE + to_string(INITIAL_VARIABLE_VALUE) + NEW_LINE;
    }

    for (int throwAwayIdentifierIndex = 0; throwAwayIdentifierIndex < quantityThrowAwayIdentifiers; throwAwayIdentifierIndex++) {
        generatedStorageAllocationCode += THROW_AWAY_IDENTIFIER_PREFIX + to_string(throwAwayIdentifierIndex) + SPACE + to_string(INITIAL_VARIABLE_VALUE) + NEW_LINE;
    }

    return generatedStorageAllocationCode;
}

const string CodeGenerator::resolveThrowAwayVariableName(const string &variableName) const {
    bool found = false;
    int scopeIndex = scopes->size() - 1;
    string throwAwayVariableName;

    while (scopeIndex >= 0 && !found) {
        for (unsigned int i = 0; i < scopes->at(scopeIndex)->getVariableNames()->size(); i++) {
            if (scopes->at(scopeIndex)->getVariableNames()->at(i) == variableName) {
                found = true;
                throwAwayVariableName = scopes->at(scopeIndex)->getVariableNames()->at(i); //throwAwayVariableName = scopes->at(scopeIndex)->getThrowAwayVariableNames()->at(i);
            }
        }

        scopeIndex--;
    }

    if (found) {
        return variableName;   //throwAwayVariableName
    } else {
        cerr << "Sematical Error: Variable \"" + variableName + "\" is not defined in any currently valid scope!\n";
        exit(-1);
    }
}

const bool CodeGenerator::isDistinct(const string &variableName) {
    for (unsigned int i = 0; i < variables->size(); i++) {
        if (variableName == variables->at(i)) {
            return false;
        }
    }

    return true;
}

const bool CodeGenerator::variableUniqueToThisScope(const std::string &variableName) {
    for (unsigned int i = 0; i < scopes->at(scopes->size() - 1)->getVariableNames()->size(); i++) {
        if (variableName == scopes->at(scopes->size() - 1)->getVariableNames()->at(i)) {
            return false;
        }
    }

    return true;
}
