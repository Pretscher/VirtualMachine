#pragma once
#include <string>
#include <vector>
#include "Utility.hpp"
using namespace std;

class Parser {
public:
    Parser(const char* file) : lines(readInput(file)) {
        currentLineIndex = -1;//we do it like that so that we dont have to check in two places if lines[0] is out of bounds
        this->advance();
    }

    bool hasMoreLines() const {
        return currentLineIndex < lines.size();
    }

    void advance();

    string getLine() const {
        return currentLine;
    }

    int getLineNumber() const {
        return currentLineIndex;
    }

    void reset();
    void writeFile(vector<string> lines) const;

    string makeUniform(string line) const;
    vector<string> getTokens();

    vector<string> readSingleFile(string file);
    vector<string> readFolder(string path);
private:
    string fileNameToWrite;
    vector<string> lines;
    int currentLineIndex;
    string currentLine;
    vector<string> readInput(const char* file);
};