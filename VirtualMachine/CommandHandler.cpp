#include "CommandHandler.hpp"


vector<string> CommandHandler::translate() {
    handleClasses();//first scan the classes for usage of static variables and calculate an optimal distribution of static space
    vector<string> instructions;
    //always start with those instructrions
    Utility::append(instructions, {
    "@256",
    "D=A",
    "@SP",
    "M=D",
        });
    bool init = false;
    while (parser.hasMoreLines()) {
        if (parser.getTokens().size() > 0) {//just skip empty lines (or comment lines that were converted to empty lines earlier)
            vector<string> currentInstructions;
            if (init == false && parser.hasMultipleFiles() == true) {//if multiple files, add "call Sys.init 0" command
                init = true;
                currentInstructions = translateCommand({ "call", "Sys.init", "0" });//call sys.init at the start
            }
            else {
                actualizeCurrentClass();//we always need to know in which class we are to get the correct static segment, if a static var is used
                currentInstructions = translateCommand(parser.getTokens());
                parser.advance();
            }
            instructions.insert(instructions.end(), currentInstructions.begin(), currentInstructions.end());
        }

    }
    return instructions;
}

vector<string> CommandHandler::translateCommand(vector<string> tokens) {
    if (tokens.size() > 0) {
        string command = tokens.at(0);
        if (command == "push") {
            return push(tokens);
        }
        else if (command == "pop") {
            return pop(tokens);
        }
        else if (command == "add") {
            return twoToOneElementsWithOperation({ "D=D+A" });
        }
        else if (command == "sub") {
            return twoToOneElementsWithOperation({ "D=A-D" });
        }
        else if (command == "and") {
            return twoToOneElementsWithOperation({ "D=D&A" });
        }
        else if (command == "or") {
            return twoToOneElementsWithOperation({ "D=D|A" });
        }
        else if (command == "eq") {
            return compare("JEQ");
        }
        else if (command == "gt") {
            return compare("JGT");
        }
        else if (command == "lt") {
            return compare("JLT");
        }
        else if (command == "neg") {
            return changeTopOfStack("D=-D");
        }
        else if (command == "not") {
            return changeTopOfStack("D=!D");
        }
        else if (command == "label") {
            return { "(" + tokens[1] + ")" };
        }
        else if (command == "goto") {
            return gotoLabel(tokens[1]);
        }
        else if (command == "if-goto") {
            return conditionalGotoLabel(tokens[1]);
        }
        else if (command == "call") {
            return call(tokens[1], tokens[2]);
        }
        else if (command == "function") {
            return function(tokens[1], tokens[2]);
        }
        else if (command == "return") {
            return returnFromFunction(currentFunctionName);//currentFunctionName is always set when we call
        }
        else {
            std::cout << "Invalid command. Exiting..." << std::endl;
            std::exit(0);
        }
    }
    return { "ERROR: Token passed to translation function is empty!" };
}

vector<string> CommandHandler::changeTopOfStack(string operation) {
    vector<string> out;
    popToD(out);
    out.push_back(operation);
    pushD(out);
    return out;
}

/* VM command: push {segment} { number }
*  Renember to remove the push-element from the tokens before calling!
*/
vector<string> CommandHandler::push(vector<string> tokens) {
    vector<string> out;
    if (tokens[1] == "constant") {
        out = pushConstantToStack(tokens[2]);
    }
    else {
        out = pushSymbol(tokens);
    }
    return out;
}

vector<string> CommandHandler::pushConstantToStack(string constant) {
    vector<string> out = {
        "@" + constant,
        "D=A"
    };
    pushD(out);
    return out;
}

vector<string> CommandHandler::pushSymbol(vector<string> tokens) {
    vector<string> out;
    if (tokens[1] == "static" || tokens[1] == "temp") {
        int address = 0;
        if (tokens[1] == "static") address = 16 + classStaticOffset[currentClass];//all classes have different places for their static variables.
        if (tokens[1] == "temp") address = 5;
        address += std::stoi(tokens[2]);
        out = {
            "@" + std::to_string(address),
            "D=M"
        };
    }
    else if (tokens[1] == "pointer") {
        string symbol = "THIS";
        if (tokens[2] == "1") symbol = "THAT";
        out = {
            "@" + symbol,
            "D=M"
        };
    }
    else {
        string symbol = symbolToSymbolPointer(tokens);
        string offset = tokens[2];
        out = {
            "@" + offset,//D = i
            "D=A",
            "@" + symbol,
            "A=D+M",//M is the adress that the symbol points to
            "D=M"//D is now the content of the register the symbol points to
        };
    }
    pushD(out);

    return out;
}

vector<string> CommandHandler::pop(vector<string> tokens) {
    vector<string> out;
    if (tokens[1] == "static" || tokens[1] == "temp") {
        int address = 0;
        if (tokens[1] == "static") address = 16 + classStaticOffset[currentClass];
        if (tokens[1] == "temp") {
            address = 5;
        }
        address += std::stoi(tokens[2]);
        popToD(out);
        vector<string> append = {
            "@" + std::to_string(address),
            "M=D"
        };
        Utility::append(out, append);
    }
    else if (tokens[1] == "pointer") {
        string symbol = "THIS";
        if (tokens[2] == "1") symbol = "THAT";
        popToD(out);
        vector<string> append = {
            "@" + symbol,
            "M=D"
        };

        Utility::append(out, append);
    }
    else {
        string symbol = symbolToSymbolPointer(tokens);
        string offset = tokens[2];
        out = {
            "@" + symbol,
            "D=M",//D=the adress of that the symbol points to, so if THIS points to 3000, this is 3000
            "@" + offset,
            "D=D+A",//D = Adress the symbol points to + i
            "@13",//we need this memory segment to save the adress, because we need to reuse A to get the address in the stack and cant access the ram through D
            "M=D"
        };
        popToD(out);
        writeStackToRamAtD(out);
    }

    return out;
}



string CommandHandler::symbolToSymbolPointer(vector<string> tokens) {
    if (tokens[1] == "local") return "LCL";
    else if (tokens[1] == "argument") return "ARG";
    else if (tokens[1] == "this" || (tokens[1] == "pointer" && tokens[2] == "0")) return "THIS";//pointer 0 and pointer 1 will be directed correctly this way
    else if (tokens[1] == "that" || (tokens[1] == "pointer" && tokens[2] == "1")) return "THAT";
    return "ERROR: invalid argument";
}

void CommandHandler::writeStackToRamAtD(vector<string>& toAdd) {
    toAdd.push_back("@13");
    toAdd.push_back("A=M");//Set A to SymbolPointer + i
    toAdd.push_back("M=D");//Ram[symbolPointer+i]=D
}

void CommandHandler::incrementSP(vector<string>& toAdd) {
    toAdd.push_back("@SP");
    toAdd.push_back("M=M+1");
}

void CommandHandler::decrementSP(vector<string>& toAdd) {
    toAdd.push_back("@SP");
    toAdd.push_back("M=M-1");
}

void CommandHandler::popToD(vector<string>& toAdd) {
    decrementSP(toAdd);
    toAdd.push_back("@SP");
    toAdd.push_back("A=M");//A = SP
    toAdd.push_back("D=M");//D = Ram[SP]
}

void CommandHandler::popToA(vector<string>& toAdd) {
    decrementSP(toAdd);
    toAdd.push_back("@SP");
    toAdd.push_back("A=M");//A = SP
    toAdd.push_back("A=M");//A = Ram[SP]
}

void CommandHandler::pushD(vector<string>& toAdd) {
    toAdd.push_back("@SP");
    toAdd.push_back("A=M");
    toAdd.push_back("M=D");
    incrementSP(toAdd);
}

void CommandHandler::pushA(vector<string>& toAdd) {
    toAdd.push_back("D=A");
    toAdd.push_back("@SP");
    toAdd.push_back("A=M");
    toAdd.push_back("M=D");
    incrementSP(toAdd);
}

//when inserting the operation, D will have the top element of the stack in it, and A the second to top element of
//the stack. After the operation, d will be pushed to the stack.
vector<string> CommandHandler::twoToOneElementsWithOperation(vector<string> operation) {
    vector<string> out;
    popToD(out);
    vector<string> append = {
        "@SP",
        "A=M-1",//A = SP - 1
        "A=M"//A = Ram[SP]

        // For Exmplae add would put "D=D+A" in this place
    };
    Utility::append(append, operation);
    decrementSP(append);
    pushD(append);
    Utility::append(out, append);
    return out;
}

vector<string> CommandHandler::compare(string instruction) {
    return twoToOneElementsWithOperation(
        { "D=A-D",
          "@TRUE" + std::to_string(nameCounter),
          "D;" + instruction,
          "(FALSE" + std::to_string(nameCounter) + ")",
          "D=0",
          "@END" + std::to_string(nameCounter),
          "0,JMP",
          "(TRUE" + std::to_string(nameCounter) + ")",
          "D=-1",
          "(END" + std::to_string(nameCounter++) + ")"
        }
    );
}

//projekt 8------------------------------------------------------------------------------

vector<string> CommandHandler::gotoLabel(string label) {
    return {
        "@" + label,
        "0;JMP"
    };
}

vector<string> CommandHandler::conditionalGotoLabel(string label) {
    vector<string> out;
    popToD(out);
    vector<string> append = {
        "@" + label,
        "D;JNE"
    };
    Utility::append(out, append);
    return out;
}

int functionsCalled = 0;
vector<string> CommandHandler::call(string functionName, string argumentCount) {
    currentFunctionName = functionName;
    //push return adrfess
    vector<string> out = { "@" + functionName + "_return" + std::to_string(functionsCalled) };//label is below all the pushes
    pushA(out);
    //push registers
    Utility::append(out, {
        "@LCL",
        "D=M"
        });
    pushD(out);
    Utility::append(out, {
        "@ARG",
        "D=M"
        });
    pushD(out);
    Utility::append(out, {
        "@THIS",
        "D=M"
        });
    pushD(out);
    Utility::append(out, {
        "@THAT",
        "D=M"
        });
    pushD(out);
    //ARG = SP-nArgs-5
    Utility::append(out, {
        "@SP",
        "D=M",
        "@5",
        "D=D-A",
        "@" + argumentCount,
        "D=D-A",
        "@ARG",
        "M=D" });
    //LCL = SP
    Utility::append(out, {
        "@SP",
        "D=M",
        "@LCL",
        "M=D"
        });

    //goto function
    Utility::append(out, gotoLabel(functionName + "_start"));
    //return label
    out.push_back("(" + functionName + "_return" + std::to_string(functionsCalled++) + ")");
    return out;
}

vector<string> CommandHandler::function(string functionName, string localVariableCount) {
    vector<string> out = {
        "(" + functionName + "_start)",
        "D=0" };//so that we always push 0 in the following code
    for (int i = 0; i < std::stoi(localVariableCount); i++) {
        pushD(out);
    }
    return out;
}

int tempCounter;//so that the temporary variables are unique
vector<string> CommandHandler::returnFromFunction(string functionName) {

    string frame = "frame" + std::to_string(tempCounter);
    vector<string> out = {
        "@LCL",
        "D=M",//D = address in LCL
        "@14",//Frame in 14 (popToD uses 13)
        "M=D",//frame = LCL
        "@5",
        "A=D-A",//A = frame - 5
        "D=M",//D = *(frame -5)
        "@15",//return address into R15
        "M=D"
    };
    //pop to *ARG
    popToD(out);
    Utility::append(out, {
        "@ARG",
        "A=M",
        "M=D",

        //revert registers
        //SP=ARG+1
        "@ARG",
        "D=M+1",
        "@SP",
        "M=D",
        //THAT = *(frame-1)
        "@14",
        "D=M",
        "@1",
        "A=D-A",
        "D=M",
        "@THAT",
        "M=D",
        //THIS = *(frame-2)
        "@14",
        "D=M",
        "@2",
        "A=D-A",
        "D=M",
        "@THIS",
        "M=D",
        //ARG = *(frame-3)
        "@14",
        "D=M",
        "@3",
        "A=D-A",
        "D=M",
        "@ARG",
        "M=D",
        //LCL = *(frame-4)
        "@14",
        "D=M",
        "@4",
        "A=D-A",
        "D=M",
        "@LCL",
        "M=D",
        //return
        "@15",
        "A=M",
        "0,JMP"
        });

    return out;
}