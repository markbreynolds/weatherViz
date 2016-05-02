#include <iostream>
#include <string>
#include <map>

#define CLEARLINE "\x1B\x5B 1F \x1B\x5B 2K"

using namespace std;

void printLine(string line);
void printLineVal(string line, float val);
void printLineVals(map<const string, float>& lines);
