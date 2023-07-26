#include <map>
#include "Parser.hpp"

class CommandHandler {
public:
    CommandHandler(Parser& parser) : parser(parser) {

    }
    vector<string> translate();
private:
    Parser& parser;
    vector<string> translateCommand(vector<string> tokens);

    vector<string> push(vector<string> tokens);
    vector<string> pop(vector<string> tokens);
    //pushes for local, argument, this, that, static, temp
    vector<string> pushSymbol(vector<string> tokens);
    //converts symbols to their corresponding Hack Assembly pointers (e.g. local to LCL)
    string symbolToSymbolPointer(vector<string> tokens);

    /*pops the TOS and puts it in the D-Register.You can perform an operation on it, like "D=-D", before
    it gets pushed to the stack again*/
    vector<string> changeTopOfStack(string operation);
    vector<string> pushConstantToStack(string constant);

    //project 7---------------------------------------------------------------------

    //increments the sp and adds the performed operations to the input-vector
    void incrementSP(vector<string>& toAdd);
    //decrements the sp and adds the performed operations to the input-vector
    void decrementSP(vector<string>& toAdd);
    //pops the TOS and saves it in the register. 
    void popToD(vector<string>& toAdd);
    void popToA(vector<string>& toAdd);
    //pushes the Register to the stack
    void pushD(vector<string>& toAdd);
    void pushA(vector<string>& toAdd);
    //Writes TOS into RAM[D]
    void writeStackToRamAtD(vector<string>& toAdd);
    /*Pops the Top 2 Elements from the Stack, the First one is in D, the second one is in A.You can now perform
    operations like "D=A-D" on these Elements. Afterwards, D is pushed to the Stack*/
    vector<string> twoToOneElementsWithOperation(vector<string> operation);
    int nameCounter = 0;//used for different names for all the labels used
    /*Takes the top 2 Elements of the Stack, and compares them via the string provided.Possible are
    "JGT", "JLT" and "JEQ"*/
    vector<string> compare(string comparison);


    //project 8-----------------------------------------------------------------------

    vector<string> gotoLabel(string label);
    vector<string> conditionalGotoLabel(string label);

    vector<string> call(string functionName, string argumentCount);
    vector<string> function(string functionName, string localVariableCount);
    vector<string> returnFromFunction(string functionName);
    string currentFunctionName;

    //maps the class and the index of the static variable to the name of a variable declared with @...
    //Like this we can give the responsibility of distributing ram adresses for static variables to the assembler!
    vector<string> staticInitialisations() {
        //this map will hold how many static variables the class needs. If static variables 1 and 3 are used, we create variables 1, 2 and 3, because
        //it seems the user wants the second static variable to exist, and maybe he wants to use them in assembly code that he mixes with the vm-translated
        //code. 
        map<string, int> maxStaticIndexForClass;
        int oldLine = parser.getLineNumber();
        while (parser.hasMoreLines()) {
            //for the collective of all functions of the class, find the highest index of static that is read or written. That, plus 1, is the length
            //of that static segment that should be dedicated to this class.
            if (parser.isLineFunctionStart()) {
                vector<string> functionLines = parser.readFunction();

                int highestStaticVariableIndex = -1;
                int currentStaticVariableIndex = -1;
                for (string line : functionLines) {
                    if (line.substr(0, 11) == "push static") {//indices 0 to 10
                        currentStaticVariableIndex = std::stoi(line.substr(12, 12));//index 12
                    }
                    else if (line.substr(0, 10) == "pop static") {//indices 0 to 9
                        currentStaticVariableIndex = std::stoi(line.substr(11, 11));//index 11
                    }

                    if (currentStaticVariableIndex > highestStaticVariableIndex) {
                        highestStaticVariableIndex = currentStaticVariableIndex;
                    }
                }

                actualizeCurrentClass();
                if (maxStaticIndexForClass[currentClass] < highestStaticVariableIndex + 1) {
                    maxStaticIndexForClass[currentClass] = highestStaticVariableIndex + 1;
                }
            }
            parser.advance();
        }
        parser.gotoLine(oldLine);

        vector<string> out;
        for (auto entry : maxStaticIndexForClass) {
            for (int i = 0; i < entry.second; i++) {
                out.push_back("@" + entry.first + ".static" + std::to_string(i));
                out.push_back("M=0");//init the variables to 0
            }
        }
        return out;
    }

    string getStaticName(string index) {
        return currentClass + ".static" + index;
    }

    void actualizeCurrentClass() {
        if (parser.isLineFunctionStart() == true) {
            int dotPos = parser.getLine().find(".");
            if (dotPos != string::npos) {//if there's a dot in the function name, its from a class
                currentClass = parser.getLine().substr(9, dotPos - 9);//from start of name (index 9) to dot
            }
        }
    }
    string currentClass;
};
