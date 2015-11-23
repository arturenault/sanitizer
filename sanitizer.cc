#include <iostream>
#include <sstream>
#include "sanitizer.h"

char quote;

int main() {
  string line;
  while(getline(cin, line)) {
    string fileName, dataField, sanitizedFileName, sanitizedDataField;

    if(!sanitizeFields(line, sanitizedFileName, sanitizedDataField)) continue;
    
    cout << sanitizedFileName << " " << sanitizedDataField << endl;
  }
}

bool sanitizeFields(const string& line, string& outputFileName, string& outputDataField) {
  stringstream filename, datafield;

  quote = '\0';

  if (line.front() == '\'' || line.front() == '\"') {
    quote = line.front();
  }

  string::const_iterator iter;

  for (iter = line.cbegin() + (quote? 1 : 0) ;
      iter != line.cend();
      ++iter) {

    unsigned char outChar = '\0';
    unsigned char current = *iter;

    if (!quote) {
      if(isSpaceOrTab(current)) {
        break;
      } else if (!isAlphanumeric(current)) {
        return false;
      }
    } else {
      if (current == '\0') {
        return false;
      } else if (current == quote) {
        ++iter;
        break;
      } else if (current == '\\') {
        if ((outChar = escapeChars(line, iter)) == '\0') {
          return false;
        }
      } 
    }

    if (outChar == '\0')
      outChar = current;

    filename << outChar;
  }

  if (iter == line.cend())
    return false;

  while (isSpaceOrTab(*iter)) iter++;

  quote = '\0';

  if (*iter == '\'' || *iter == '\"') 
    quote = *iter;

  for (iter += (quote? 1 : 0) ;
      iter != line.cend();
      ++iter) {

    unsigned char outChar = '\0';
    unsigned char current = *iter;

    if (!quote) {
      if (!isAlphanumeric(current)) {
        return false;
      }
    } else {
      if (current == '\0') {
        return false;
      } else if (current == quote) {
        ++iter;
        break;
      } else if (current == '\\') {
        if ((outChar = escapeChars(line, iter)) == '\0') {
          return false;
        }
      } 
    }

    if (outChar == '\0')
      outChar = current;
    datafield << outChar;
  }

  if (iter != line.cend())
      return false;


  outputFileName = filename.str();
  outputDataField = datafield.str();
  return true;
}

bool isSpaceOrTab(unsigned char letter) {
  if (letter == ' ' || letter == '\t') return true;
  else return false;
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
  } else if (current == quote) {
    return quote;
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
