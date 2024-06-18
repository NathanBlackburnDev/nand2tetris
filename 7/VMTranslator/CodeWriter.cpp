#include <boost/algorithm/string/trim.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "CodeWriter.h"
#include "FileOp.h"


void CodeWriter::initializer(const char *outputFile, const std::string &VMFileName) {
    asmFile = fileWriter.writeFile(outputFile);   
    VMfileName = VMFileName;
}


void CodeWriter::writeArithmetic(std::string &command) {
    labelCounterArithmetic++;
    std::stringstream labelCounterStream;
    std::string labelCounterStr;
    labelCounterStream << labelCounterArithmetic;
    labelCounterStream >> labelCounterStr;

    std::string push = 
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";

    std::string pop = 
        "@SP\n"
        "M=M-1\n"
        "A=M\n"
        "D=M\n";
    
    std::string popR13 = 
        "@R13\n"
        "M=D\n";

    std::string popR14 = 
        "@R14\n"
        "M=D\n";

    std::string add = 
        "@R13\n"
        "D=M\n"
        "@R14\n"
        "D=D+M\n";

    std::string sub = 
        "@R13\n"
        "D=M\n"
        "@R14\n"
        "D=D-M\n";

    std::string neg = 
        "@R13\n"
        "D=-M\n";

    std::string gt = 
        "@R13\n"
        "D=M\n"
        "@R14\n"
        "D=D-M\n"
        "@GT" + labelCounterStr + "\n"
        "D;JGT\n"
        "@NO.gt" + labelCounterStr + "\n"
        "0;JMP\n"
        "(GT" + labelCounterStr + ")" + "\n"
        "D=-1\n"
        "@GT.skip" + labelCounterStr + "\n"
        "0;JMP\n"
        "(NO.gt" + labelCounterStr + ")" + "\n"
        "D=0\n"
        "(GT.skip" + labelCounterStr + ")" + "\n";

    std::string lt = 
        "@R13\n"
        "D=M\n"
        "@R14\n"
        "D=D-M\n"
        "@LT" + labelCounterStr + "\n"
        "D;JLT\n"
        "@NO.lt" + labelCounterStr + "\n"
        "0;JMP\n"
        "(LT" + labelCounterStr + ")" + "\n"
        "D=-1\n"
        "@LT.skip" + labelCounterStr + "\n"
        "0;JMP\n"
        "(NO.lt" + labelCounterStr + ")" + "\n"
        "D=0\n"
        "(LT.skip" + labelCounterStr + ")" + "\n";

    std::string eq = 
        "@R13\n"
        "D=M\n"
        "@R14\n"
        "D=D-M\n"
        "@EQ" + labelCounterStr + "\n"
        "D;JEQ\n"
        "@NO.eq" + labelCounterStr + "\n"
        "0;JMP\n"
        "(EQ" + labelCounterStr + ")" + "\n"
        "D=-1\n"
        "@EQ.skip" + labelCounterStr + "\n"
        "0;JMP\n"
        "(NO.eq" + labelCounterStr + ")" + "\n"
        "D=0\n"
        "(EQ.skip" + labelCounterStr + ")" + "\n";

    std::string bitNot = 
        "@R13\n"
        "D=!M\n";

    std::string bitAnd = 
        "@R13\n"
        "D=M\n"
        "@R14\n"
        "D=D&M\n";

    std::string bitOr =
        "@R13\n"
        "D=M\n"
        "@R14\n"
        "D=D|M\n";

    std::unordered_map<std::string, Commands> commandMap = {
        {"add", ADD},
        {"sub", SUB},
        {"neg", NEG},
        {"eq", EQ},
        {"gt", GT},
        {"lt", LT},
        {"and", AND},
        {"or", OR},
        {"not", NOT}
    };

    std::unordered_map<std::string, Commands>::const_iterator commandIter = commandMap.find(command);
    switch (commandIter->second) {
        case 1: 
            asmFile << "// pop R13 and R14, add" << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR13 << "      // save to R13" << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR14 << "      // save to R14" << std::endl;
            asmFile << add << "     // add" << std::endl;
            break;
        case 2:
            asmFile << "// pop R14 and R13, sub" << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR14 << "      // save to R14" << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR13 << "      // save to R13" << std::endl;
            asmFile << sub << "     // sub" << std::endl;
            break;
        case 3:
            asmFile << "// popR13, neg" << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR13 << "      // save to R13" << std::endl;
            asmFile << neg << std::endl;
            break;
        case 4:
            asmFile << "// popR14 and popR13, eq" << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR14 << "      // save to R14" << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR13 << "      // save to R13" << std::endl;
            asmFile << eq << std::endl;
            break;
        case 5:
            asmFile << "// popR14 and popR13, gt" << std::endl;
            asmFile << pop << std::endl;
            asmFile << popR14 << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR13 << "      // save to R13" << std::endl;
            asmFile << gt << std::endl;
            break;
        case 6:
            asmFile << "// pop14 and popR13, lt" << std::endl;
            asmFile << pop << std::endl;
            asmFile << popR14 << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR13 << "      // save to R13" << std::endl;
            asmFile << lt << std::endl;
            break;
        case 7:
            asmFile << "// popR14 and popR13, bitAnd" << std::endl;
            asmFile << pop << std::endl;
            asmFile << popR14 << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR13 << "      // save to R13" << std::endl;
            asmFile << bitAnd << std::endl;
            break;
        case 8:
            asmFile << "// popR14 and popR13, bitOr" << std::endl;
            asmFile << pop << std::endl;
            asmFile << popR14 << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR13 << "      // save to R13" << std::endl;
            asmFile << bitOr << std::endl;
            break;
        case 9:
            asmFile << "// pop R13, bitNot" << std::endl;
            asmFile << pop << "     // pop" << std::endl;
            asmFile << popR13 << "      // save to R13" << std::endl;
            asmFile << bitNot << std::endl;
            break;
    }

    asmFile << "// push" << std::endl;
    asmFile << push << std::endl;
}


void CodeWriter::writePushPop(std::string &command, std::string &segment, int &index) {
    labelCounterOther++;

    std::stringstream labelCounterStream;
    std::string labelCounterStr;
    labelCounterStream << labelCounterOther;
    labelCounterStream >> labelCounterStr;

    if (segment == "temp")
        index = index + 5;

    std::string strIndex;
    std::stringstream indexStream;
    indexStream << index;
    indexStream >> strIndex;

    std::string ogSegment = segment;

    if (segment == "local")
        segment = "LCL";
    else if (segment == "argument")
        segment = "ARG";
    else if (segment == "this")
        segment = "THIS";
    else if (segment == "that")
        segment = "THAT";
    else if (segment == "pointer" && index == 0)
        segment = "THIS";
    else if (segment == "pointer" && index == 1)
        segment = "THAT";

    std::string push1 = 
        "@" + segment + "\n"
        "D=M\n"
        "@" + strIndex + "\n"
        "A=D+A\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";

    std::string pop1 = 
        "@" + segment + "\n"
        "D=M\n"
        "@" + strIndex + "\n"
        "D=D+A\n"
        "@R15\n"
        "M=D\n"
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "@R15\n"
        "A=M\n"
        "M=D\n";

    std::string pushPointer = 
        "@" + segment + "\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";

    std::string popPointer = 
        "@" + segment + "\n"
        "D=A\n"             
        "@R15\n"
        "M=D\n"
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "@R15\n"
        "A=M\n"
        "M=D\n";

    std::string pushTemp = 
        "@R5\n" 
        "D=M\n"
        "@" + strIndex + "\n"
        "A=D+A\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";

    std::string popTemp = 
        "@R5\n"
        "D=M\n"
        "@" + strIndex + "\n"
        "D=D+A\n"
        "@R15\n"
        "M=D\n"
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "@R15\n"
        "A=M\n"
        "M=D\n";

    std::string pushConst = 
        "@" + strIndex + "\n"
        "D=A\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";

    std::string pushStatic = 
        "@" + VMfileName + "." + strIndex + "\n"
        "D=A\n"
        "@" + strIndex + "\n"
        "A=D+A\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";

    std::string popStatic = 
        "@" + VMfileName + "." + strIndex + "\n"
        "D=A\n"
        "@" + strIndex + "\n"
        "D=D+A\n"
        "@R15\n"
        "M=D\n"
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "@R15\n"
        "A=M\n"
        "M=D\n";

    std::string segmentsArr[5] = {"LCL", "ARG", "THIS", "THAT"};
    std::string *foundSegment = std::find(std::begin(segmentsArr), std::end(segmentsArr), segment);

    if (command == "C_PUSH" && ogSegment == "pointer") {
        asmFile << "// push " + ogSegment + " " + strIndex << std::endl;
        asmFile << pushPointer << std::endl;
    } else if (command == "C_POP" && ogSegment == "pointer") {
        asmFile << "// pop " + ogSegment + " " + strIndex << std::endl;
        asmFile << popPointer << std::endl;
    } else if (command == "C_PUSH" && foundSegment != std::end(segmentsArr)) {
        asmFile << "// push " + segment + " " + strIndex << std::endl;
        asmFile << push1 << std::endl; 
    } else if (command == "C_POP" && foundSegment != std::end(segmentsArr)) {
        asmFile << "// pop " + segment + " " + strIndex << std::endl;
        asmFile << pop1 << std::endl; 
    } else if (command == "C_POP" && segment == "temp") {
        asmFile << "// pop " + segment + " " + strIndex << std::endl;
        asmFile << popTemp << std::endl; 
    } else if (command == "C_PUSH" && segment == "temp") {
        asmFile << "// push " + segment + " " + strIndex << std::endl;
        asmFile << pushTemp << std::endl; 
    } else if (command == "C_PUSH" && segment == "constant") {
        asmFile << "// push " + segment + " " + strIndex << std::endl;
        asmFile << pushConst << std::endl;
    } else if (command == "C_PUSH" && segment == "static") {
        asmFile << "// push " + segment + " " + strIndex << std::endl;
        asmFile << pushStatic << std::endl;
    } else if (command == "C_POP" && segment == "static") {
        asmFile << "// pop " + segment + " " + strIndex << std::endl;
        asmFile << popStatic << std::endl;
    }
}


void CodeWriter::close() {
    std::string endLoop = 
        "@END\n"
        "(END)\n"
        "@END\n"
        "0;JMP\n";
    asmFile << endLoop << std::endl;
    asmFile.close();
}


void CodeWriter::setFileName(const std::string &fileName) {

}


void CodeWriter::writeLabel(const std::string &label) {
    std::string tempFuncName = currentFuncName.empty() ? "NULL" : currentFuncName;
    std::string labelASM = 
        "(" + VMfileName + "." + tempFuncName + "$" + label + ")" + "\n";
    asmFile << "// labelASM" << std::endl;
    asmFile << labelASM << std::endl;
}


void CodeWriter::writeGoto(const std::string &label) {
    std::string tempFuncName = currentFuncName.empty() ? "NULL" : currentFuncName;
    std::string gotoASM = 
        "   @" + VMfileName + "." + tempFuncName + "$" + label + "\n"
        "   0;JMP\n";
    asmFile << "// gotoASM" << std::endl;
    asmFile << gotoASM << std::endl;
}


void CodeWriter::writeIf(const std::string &label) {
    std::string tempFuncName = currentFuncName.empty() ? "NULL" : currentFuncName;
    std::string ifGotoASM = 
        "   @SP\n"
        "   M=M-1\n"
        "   A=M\n"      
        "   D=M\n"      
        "   @" + VMfileName + "." + tempFuncName + "$" + label + "\n"
        "   D;JNE\n";
    asmFile << "// ifGotoASM" << std::endl;
    asmFile << ifGotoASM << std::endl;
}


void CodeWriter::writeFunction(const std::string &functionName, const int &nVars) {
    currentFuncName = functionName;
    std::string writeFuncASM = 
        // TODO: Make function iterate correct number of times
        "(" + functionName + ")\n"
        "   @" + std::to_string(nVars) + "\n"
        "   D=A\n"
        "   @" + functionName + "$" + "end_func\n"
        "   D;JEQ\n"
        "(" + functionName + "$" + "main_loop)\n"
        "   @SP\n"
        "   A=M\n"
        "   M=0\n"
        "   @SP\n"
        "   M=M+1\n"
        "   D=D-1\n"
        "   @" + functionName + "$" + "main_loop\n"
        "   D;JGT\n"
        "(" + functionName + "$" + "end_func)\n";
    asmFile << "//writeFuncASM" << std::endl;
    asmFile << writeFuncASM << std::endl;
}


void CodeWriter::writeCall(const std::string &functionName, const int &nVars) {
    static int runningInt = 0;
    std::string stdPush = 
        "   @SP\n"
        "   A=M\n"
        "   M=D\n"
        "   @SP\n"
        "   M=M+1\n";

    std::string writeCallASM =  
        "// push returnAddress\n"
        "   @" + functionName + "$ret." + std::to_string(runningInt) + "\n"
        "   D=A\n"
        "   " + stdPush + 
        "// push LCL\n"
        "   @LCL\n"
        "   D=M\n"
        "   " + stdPush +
        "// push ARG\n"
        "   @ARG\n"
        "   D=M\n"
        "   " + stdPush +
        "// push THIS\n"
        "   @THIS\n"
        "   D=M\n"
        "   " + stdPush +
        "// push THAT\n"
        "   @THAT\n"
        "   D=M\n"
        "   " + stdPush +
        "// ARG = SP - 5 - nArgs\n"
        "   @SP\n"
        "   D=M\n"
        "   @5\n"
        "   D=D-A\n"
        "   @" + std::to_string(nVars) + "\n"
        "   D=D-A\n"
        "   @ARG\n"
        "   M=D\n" 
        "// LCL = SP\n"
        "   @SP\n"
        "   D=M\n"
        "   @LCL\n"
        "   M=D\n"
        "// goto f\n"
        "@" + functionName + "\n"
        "0;JMP\n"
    "(" + functionName + "$ret." + std::to_string(runningInt) + ")\n";
    asmFile << writeCallASM << std::endl;
    runningInt++;
}


void CodeWriter::writeReturn() {
    std::string writeReturnASM = 
        "// frame = LCL\n"
        "   @LCL\n"
        "   D=M\n"
        "   @" + currentFuncName + "$frame\n"
        "   M=D\n"
        "// retAddr(R15) = *(LCL - 5)\n"
        "   @LCL\n"
        "   D=M\n"
        "   A=D\n"
        "   D=M\n"
        "   @5\n"
        "   D=D-A\n"
        "   M=D\n"
        "   @R15\n"
        "   M=D\n"
        // TODO: Either the value is not being popped of the stack, or ARG is not being derferenced
        "// *ARG = pop()\n"
        "   @SP\n"
        "   AM=M-1\n"
        "   D=M\n"
        "   @ARG\n"
        "   A=M\n"
        "   M=D\n"
        "// SP = ARG++\n"
        "   @ARG\n"
        "   D=M\n"
        "   D=D+1\n"
        "   @SP\n"
        "   M=D\n"
        "// THAT = *(frame--)\n"
        "   @" + currentFuncName + "$frame\n"
        "   M=M-1\n"
        "   D=M\n"
        "   A=D\n"
        "   D=M\n"
        "   @THAT\n"
        "   M=D\n"
        "// THIS = *(frame - 2)\n"
        "   @" + currentFuncName + "$frame\n"
        "   M=M-1\n"
        "   D=M\n"
        "   A=D\n"
        "   D=M\n"
        "   @THIS\n"
        "   M=D\n"
       " // ARG = *(frame - 3)\n"
        "   @" + currentFuncName + "$frame\n"
        "   M=M-1\n"
        "   D=M\n"
        "   A=D\n"
        "   D=M\n"
        "   @ARG\n"
        "   M=D\n"
        // LCL = *(frame - 4)
        "// LCL = *(frame - 4)\n"
        "   @" + currentFuncName + "$frame\n"
        "   M=M-1\n"
        "   D=M\n"
        "   A=D\n"
        "   D=M\n"
        "   @LCL\n"
        "   M=D\n"
        "// goto retAddr\n"
        "   @R15\n"
        "   0;JMP\n";
    asmFile << writeReturnASM << std::endl;
}