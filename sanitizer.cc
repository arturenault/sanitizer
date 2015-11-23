#include <iostream>
#include <sstream>
#include "sanitizer.h"

char quote;

int main() {
  string line;
  while (getline(cin, line)) {
    string fileName, dataField, sanitizedFileName, sanitizedDataField;

    if (!SanitizeFields(line, sanitizedFileName, sanitizedDataField)) continue;

    cout << sanitizedFileName << " " << sanitizedDataField << endl;
  }
}

bool SanitizeFields(const string& line, string& outputFileName,
                    string& outputDataField) {
  string::const_iterator iter = line.cbegin();
  string::const_iterator end = line.cend();

  if (!SanitizeField(iter, end, outputFileName)) return false;

  if (iter == end) return false;
  while (IsSpaceOrTab(*iter)) ++iter;

  if (!SanitizeField(iter, end, outputDataField)) return false;

  if (iter != end) return false;

  return true;
}

bool SanitizeField(string::const_iterator& iter,
                   const string::const_iterator& end, string& output) {
  stringstream outBuffer;
  quote = '\0';
  bool matched = false;

  if (*iter == '\'' || *iter == '\"') {
    quote = *(iter++);
  }

  for (iter + (quote ? 1 : 0); iter != end; ++iter) {
    unsigned char outChar = '\0';
    unsigned char current = *iter;

    if (!quote) {
      if (IsSpaceOrTab(current)) {
        break;
      } else if (!IsAlphanumeric(current)) {
        return false;
      }
    } else {
      if (current == '\0') {
        return false;
      } else if (current == quote) {
        matched = true;
        ++iter;
        break;
      } else if (current == '\\') {
        if ((outChar = EscapeChars(iter, end)) == '\0') {
          return false;
        }
      }
    }

    if (outChar == '\0') outChar = current;
    outBuffer << outChar;
  }

  if (quote && !matched) return false;

  output = outBuffer.str();
  return true;
}

bool IsSpaceOrTab(unsigned char letter) {
  if (letter == ' ' || letter == '\t')
    return true;
  else
    return false;
}

bool IsAlphanumeric(unsigned char letter) {
  if ((letter >= '0' && letter <= '9') || (letter >= 'A' && letter <= 'Z') ||
      (letter >= 'a' && letter <= 'z') || (letter == 131) ||  // ƒ
      (letter == 138) ||                                      // Š
      (letter == 140) ||                                      // Œ
      (letter == 142) ||                                      // Ž
      (letter == 154) ||                                      // š
      (letter == 156) ||                                      // œ
      (letter == 158) ||                                      // ž
      (letter == 159) ||                                      // Ÿ
      (letter == 170) ||                                      // ª
      (letter == 178) ||                                      // ²
      (letter == 179) ||                                      // ³
      (letter == 185) ||                                      // ¹
      (letter == 186) ||                                      // º
      (letter >= 192 && letter <= 214) || (letter >= 216 && letter <= 246) ||
      (letter >= 248 && letter <= 255)) {
    return true;
  } else {
    return false;
  }
}

unsigned char EscapeChars(string::const_iterator& iter,
                          const string::const_iterator& end) {
  if (++iter == end) return '\0';

  unsigned char current = *iter;
  if (current == '\\') {
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

    if (++iter == end - 1) return '\0';
    current = *iter;

    if (!(current >= '0' && current <= '8')) {
      return '\0';
    }
    value = value * 8 + (current - '0');

    if (++iter == end - 1) return '\0';
    current = *iter;

    if (!(current >= '0' && current <= '8')) {
      return '\0';
    }
    value = value * 8 + (current - '0');

    return (unsigned char)value;
  } else {
    return '\0';
  }
}
