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
};
