// Translates Hack VM bytecode into Hack ASM

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "VMTranslator.h"


int main(int argc, char **argv) {
    Parser parser;
    std::ifstream inputFile(argv[1]);
    parser.initializer(inputFile);
    parser.getFileVect();
    parser.advance();
    parser.advance();
    parser.advance();
    parser.advance();
    parser.advance();
    parser.advance();
    parser.advance();
    parser.advance();
        parser.advance();
    parser.advance();
    parser.advance();
    parser.advance();
        parser.advance();
    parser.advance();
    parser.advance();
    parser.advance();
    parser.advance();
    parser.commandType();
    parser.arg1();
    return 1;
}


void Parser::initializer(std::ifstream &inputFile) {
    std::string line;
    std::vector<std::string> lineVect;
    if (inputFile.is_open()) {
        while (std::getline(inputFile, line)) {
            lineVect.clear();
            boost::algorithm::trim(line);
            lineVect.push_back(line);
            fileVect.push_back(lineVect);
        }
        inputFile.close();
        return;
    }
    std::cout << "Failed to open file" << std::endl;
}


bool Parser::hasMoreLines() {
    int iter = (currentLine > 0) ? (currentLine + 1) : 0;

    for (int i = iter; i < fileVect.size(); i++) {
        for (int j = 0; j < fileVect[i].size(); j++) {
            std::size_t foundWhiteSpace = fileVect[i][j].find_first_not_of(" \t\n\v\f\r");
            if (fileVect[i][j].substr(foundWhiteSpace, 2) != "//")
                return true;
            continue;
        }
    }

    return false;
}


void Parser::advance() {
    int iter = (currentLine > 0) ? (currentLine + 1) : 0;

    for (int i = iter; i < fileVect.size(); i++) {
        for (int j = 0; j < fileVect[i].size(); j++) {
            std::size_t foundWhiteSpace = fileVect[i][j].find_first_not_of(" \t\n\v\f\r");
            if (foundWhiteSpace != std::string::npos) {
                if (fileVect[i][j].substr(foundWhiteSpace, 2) != "//") {
                    currentLine = i;
                    currentInstruction = fileVect[i][j];
                    return;
                } 
                continue;
            }
        }
    }
}


const std::string Parser::commandType() {
    commandMap = {
        {"add", C_ARITHMETIC},
        {"sub", C_ARITHMETIC},
        {"neq", C_ARITHMETIC},
        {"eq", C_ARITHMETIC},
        {"gt", C_ARITHMETIC},
        {"lt", C_ARITHMETIC},
        {"and", C_ARITHMETIC},
        {"or", C_ARITHMETIC},
        {"not", C_ARITHMETIC},
        {"push", C_PUSH},
        {"pop", C_POP},
        {"label", C_LABEL},
        {"goto", C_GOTO},
        {"if-goto", C_IF},
        {"function", C_CALL},
        {"return", C_RETURN}
    };

    std::size_t whitespacePos = currentInstruction.find_first_of(' ');
    std::string strippedInstruction = currentInstruction.substr(0, whitespacePos);
    std::unordered_map<std::string, Commands>::const_iterator command = commandMap.find(strippedInstruction);

    switch (command->second) {
        case 1:
            return "C_ARITHMETIC";
            break;
        case 2:
            return "C_PUSH";
            break;
        case 3:
            return "C_POP";
            break;
        case 4:
            return "C_LABEL";
            break;
        case 5:
            return "C_GOTO";
            break;
        case 6:
            return "C_IF";
            break;
        case 7:
            return "C_FUNCTION";
            break;
        case 8:
            return "C_RETURN";
            break;
        case 9:
            return "C_CALL";
            break;
    }

    return "std::string Parser::commandType() has encountered an error\n";
}


std::string Parser::arg1() {
    std::size_t push = currentInstruction.find("push");
    std::size_t pop = currentInstruction.find("pop");
    std::size_t whitespacePos = currentInstruction.find_first_of(' ');

    if (push != std::string::npos || pop != std::string::npos) {
        std::string pushPopInstruction = currentInstruction.substr(whitespacePos);
        boost::algorithm::trim(pushPopInstruction);
        std::size_t whitespaceSubPos = pushPopInstruction.find_first_of(' ');
        std::string strippedPushPopInstruction = pushPopInstruction.substr(0, whitespaceSubPos);
        return strippedPushPopInstruction;
    }
    
    std::size_t eos = currentInstruction.find_last_not_of(' ');
    return currentInstruction.substr(0, (eos + 1));
}


std::string Parser::arg2() {
    return "fuck";
}