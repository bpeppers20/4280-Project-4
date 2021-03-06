#include <iostream>
#include <fstream>
#include "parser.h"
#include "code-generator.h"

using namespace std;

const static char *ERROR_MESSAGE_ILLEGAL_QUANTITY_ARGUMENTS = "Invalid quantity of command line arguments encountered, pass either zero arguments for keyboard input, or one argument for file input.\n";
const static char *ERROR_MESSAGE_NO_SUCH_FILE = "Scanner Error: No file with the provided name exists: \"";

const static int QUANTITY_DEFAULT_ARGUMENTS = 1;

const static int EXIT_CODE_SUCCESS = 0;
const static int EXIT_CODE_ILLEGAL_QUANTITY_ARGUMENTS = 1;
const static int EXIT_CODE_NO_SUCH_INPUT_FILE = 2;

const static char *KEYBOARD_MODE_OUTPUT_FILE_NAME = "kb";
const static char *IMPLICIT_INPUT_FILE_EXTENSION = ".fs";
const static char *OUTPUT_FILE_EXTENSION = ".asm";

const bool noArgumentsPassed(int quantityCommandLineArgumentsWithoutDefaultArgument);

const void runForKeyboardInput();

const bool oneArgumentPassed(int quantityCommandLineArgumentsWithoutDefaultArgument);

const void runForFileInput(char *const *commandLineArguments);

const string getRawKeyboardData();

const string getRawInputFileData(const string &inputFileName);

const void processData(const string &rawData, const string &fileName);

const string trim(const string &toBeTrimmed);

int main(int quantityCommandLineArguments, char *commandLineArguments[]) {
    int quantityCommandLineArgumentsWithoutDefaultArgument = quantityCommandLineArguments - QUANTITY_DEFAULT_ARGUMENTS;

    if (noArgumentsPassed(quantityCommandLineArgumentsWithoutDefaultArgument)) {
        try {
            runForKeyboardInput();
        } catch (const int &EXIT_CODE) {
            return EXIT_CODE;
        }
    } else if (oneArgumentPassed(quantityCommandLineArgumentsWithoutDefaultArgument)) {
        try {
            runForFileInput(commandLineArguments);
        } catch (const int &EXIT_CODE) {
            return EXIT_CODE;
        }
    } else {
        cerr << ERROR_MESSAGE_ILLEGAL_QUANTITY_ARGUMENTS << endl;
        return EXIT_CODE_ILLEGAL_QUANTITY_ARGUMENTS;
    }

    return EXIT_CODE_SUCCESS;
}

const bool noArgumentsPassed(int quantityCommandLineArgumentsWithoutDefaultArgument) {
    return quantityCommandLineArgumentsWithoutDefaultArgument == 0;
}

const void runForKeyboardInput() {
    string rawKeyboardData = getRawKeyboardData();
    processData(rawKeyboardData, KEYBOARD_MODE_OUTPUT_FILE_NAME);
}

const bool oneArgumentPassed(int quantityCommandLineArgumentsWithoutDefaultArgument) {
    return quantityCommandLineArgumentsWithoutDefaultArgument == 1;
}

const void runForFileInput(char *const *commandLineArguments) {
    string inputFileName = commandLineArguments[1];
    string rawInputFileData = getRawInputFileData(inputFileName);
    processData(rawInputFileData, inputFileName);
}

const string getRawKeyboardData() {
    string rawKeyboardData;
    string rawKeyboardLine;

    while (getline(cin, rawKeyboardLine)) {
        if (rawKeyboardData.empty()) {
            rawKeyboardData = rawKeyboardLine;
        } else {
            rawKeyboardData += "\n" + rawKeyboardLine;
        }
    }
    return rawKeyboardData;
}

const string getRawInputFileData(const string &inputFileName) {
    ifstream inputFile(inputFileName + IMPLICIT_INPUT_FILE_EXTENSION);

    if (inputFile) {
        string rawInputFileData;
        string rawInputFileLine;

        while (!inputFile.eof()) {
            getline(inputFile, rawInputFileLine);
            if (rawInputFileData.empty()) {
                rawInputFileData = rawInputFileLine;
            } else {
                rawInputFileData += "\n" + rawInputFileLine;
            }
        }

        inputFile.close();

        return rawInputFileData;
    } else {
        cerr << ERROR_MESSAGE_NO_SUCH_FILE << inputFileName << ".fs\"\n";
        exit(EXIT_CODE_NO_SUCH_INPUT_FILE);
    }
}

const void processData(const string &rawData, const string &outputFileName) {
    const Parser *parser = new Parser(rawData);
    const Node *parseTree = parser->parser();
    //cout << parseTree->toString() << endl; tree is to no longer be displayed

    CodeGenerator *codeGenerator = new CodeGenerator(parseTree);
    const string generatedCode = codeGenerator->generateCode();
    cout << outputFileName << OUTPUT_FILE_EXTENSION << endl;

    ofstream outputFile;
    outputFile.open(outputFileName + OUTPUT_FILE_EXTENSION);
    outputFile << generatedCode;
    outputFile.close();
}