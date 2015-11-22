#include <iostream>
#include <sstream>
#include "sanitizer.h"

int main() {
  string line;
  while(getline(cin, line)) {
    string fileName, dataField;
    stringstream lineReader(line);

    lineReader >> fileName;
    lineReader >> dataField;


    string sanitizedFileName;
    if(!sanitizeField(fileName, sanitizedFileName)) continue;

    cout << sanitizedFileName << " " << dataField << endl;
  }
}

bool sanitizeField(const string& name, string& output) {
  output = "";

  bool quoted = false;

  if (name.front() == '\'' || name.front() == '\"') {
    if (name.front() != name.back()) {
      return false;
    }

    quoted = true;
  }

  for (auto iter = name.cbegin() + (quoted? 1 : 0) ;
      iter != name.cend() - (quoted? 1 : 0);
      ++iter) {

    unsigned char outChar = '\0';
    unsigned char current = *iter;

    if (!quoted) {
      if (!isAlphanumeric(current)) {
        return false;
      }
    } else {
      if (current == '\\') {
        if ((outChar = escapeChars(name, iter)) == '\0') {
          return false;
        }
      } 
    }

    if (outChar == '\0') outChar = current;
    output += outChar;
  }

  return true;
}

bool isAlphanumeric(unsigned char letter) {
  if ((letter >= '0' && letter <= '9') ||
      (letter >= 'A' && letter <= 'Z') ||
      (letter >= 'a' && letter <= 'z') ||
      (letter == 131) || // ƒ
      (letter == 138) || // Š
      (letter == 140) || // Œ
      (letter == 142) || // Ž
      (letter == 154) || // š
      (letter == 156) || // œ
      (letter == 158) || // ž
      (letter == 159) || // Ÿ
      (letter == 170) || // ª
      (letter == 178) || // ²
      (letter == 179) || // ³
      (letter == 185) || // ¹
      (letter == 186) || // º
      (letter >= 192 && letter <= 214) ||
      (letter >= 216 && letter <= 246) ||
      (letter >= 248 && letter <= 255)) {
    return true;
  } else {
    return false;
  }
}

unsigned char escapeChars(const string& name, string::const_iterator& iter) {
  if (++iter == name.cend()) return '\0';

  unsigned char current = *iter;
  if(current == '\\') {
    return '\\';
  } else if (current == 'n') {
    return '\n';
  } else if (current == 'r') {
    return '\r';
  } else if (current == 't') {
    return '\t';
  } else if (current >= '0' && current < '8') {
    int value = current - '0';

    if(++iter == name.cend() - 1) return '\0';
    current = *iter;

    if (!(current >= '0' && current <= '8')) {
      return '\0';
    }
    value = value * 8 + (current - '0'); 

    if(++iter == name.cend() - 1) return '\0';
    current = *iter;

    if (!(current >= '0' && current <= '8')) {
      return '\0';
    }
    value = value * 8 + (current - '0');

    return (unsigned char) value;
  } else {
    return '\0';
  }
}
