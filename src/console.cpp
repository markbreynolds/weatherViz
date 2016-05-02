#include "console.hpp"

using namespace std;

void printLine(string line) {
  cout << CLEARLINE;
  cout << line;
}

void printLineVal(string line, float val) {
  cout << CLEARLINE;
  cout << line << val<< endl;
}

void printLineVals(map<const string, float>& lines) {
  for (int i=0;i<lines.size();i++) {
    cout << CLEARLINE;
  }
  for (map<const string, float>::iterator it = lines.begin(); it!=lines.end(); ++it) {
    cout << it->first << it->second << endl;
  }
}
