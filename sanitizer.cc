#include <iostream>
#include <sstream>

using namespace std;

int main() {
  string line;
  while(getline(cin, line)) {
    string fileName, dataField;
    stringstream lineReader(line);

    getline(lineReader, fileName, ' ');
    getline(lineReader, dataField);

    cout << fileName << " " << dataField << endl;
  }
}
