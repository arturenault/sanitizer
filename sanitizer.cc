#include "sanitizer.h"

/* I started using deques because I thought I was going to pop from both sides.
 * that turned out to be unnecessary and I'm lazy to change it now */
deque<string> currentPath, tmpPath;

int main() {
  string line;  // C++ needs a better way to get the current directory
  char* buf = get_current_dir_name();
  if (!buf) cout << "malloc failed" << endl;
  string currentPathString(buf);
  free(buf);

  /* create deques from temp and the current path for future comparison */
  PopulateDeque(tmpPath, "/tmp");
  PopulateDeque(currentPath, currentPathString);

  while (getline(cin, line)) {
    string fileName, dataField, sanitizedFileName, sanitizedDataField,
        resolvedFileName;

    /* sanitize both fields */
    if (!SanitizeFields(line, sanitizedFileName, sanitizedDataField)) continue;

    /* check the path */
    if (!ResolvePath(sanitizedFileName + "." + UNI, resolvedFileName)) continue;

    /* safely escape the outputs */
    string escapedFileName = EscapeField(resolvedFileName);
    string escapedDataField = EscapeField(sanitizedDataField);

    string command = "echo " + escapedDataField + " >> " + escapedFileName;
    system(command.c_str());
  }
}

string EscapeField(const string& field) {
  stringstream output;

  /* enclose fields in single quotes so things don't get expanded */
  output << '\'';
  for (string::const_iterator iter = field.cbegin(); iter != field.cend();
       ++iter) {
    char current = *iter;

    if (current == '\'') {
      /* you need to close the string, create a new one with a single quote,
       * and reopen the original one to insert the single quote */
      output << "\'\"\'\"\'";
    } else {
      output << current;
    }
  }

  output << '\'';
  return output.str();
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
  char quote = '\0';
  bool matched = false;

  if (*iter == '\'' || *iter == '\"') {
    /* if quoted, remember if single or double and move iterator one forward */
    quote = *(iter++);
  }

  for (; iter != end; ++iter) {
    unsigned char outChar = '\0';
    unsigned char current = *iter;

    if (!quote) {
      if (IsSpaceOrTab(current)) {
        // end of unquoted field
        break;
      } else if (!IsAlphanumeric(current)) {
        // invalid char
        return false;
      }
    } else {
      if (current == '\0') {
        // invalid char
        return false;
      } else if (current == quote) {
        // end of quoted string
        matched = true;
        ++iter;
        break;
      } else if (current == '\\') {
        // escape sequence
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
  return (letter == ' ' || letter == '\t');
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
  } else if (current >= '0' && current < '8') {
    /* handle octal character */
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
    // for all other escapes, just return the character. people can be paranoid.
    return current;
  }
}

bool ResolvePath(const string& filename, string& output) {
  deque<string> path;

  // Build deque of the file path, check that it's in either /tmp or the current
  // directory, and return a shortened version.
  PopulateDeque(path, filename);

  string file = path.back();
  path.pop_back();
  if (path != tmpPath && path != currentPath) return false;
  path.push_back(file);

  stringstream outputStream;
  for (auto iter = path.begin(); iter != path.end(); ++iter)
    outputStream << "/" << *iter;

  output = outputStream.str();

  return true;
}

void PopulateDeque(deque<string>& path, const string& filename) {
  stringstream pathstream(filename);
  string file;
  if (filename.front() != '/') {
    path = currentPath;
  } else {
    getline(pathstream, file, '/');
  }

  while (getline(pathstream, file, '/')) {
    if (file == "." || file == "") {
      // ignore
    } else if (file == ".." && !path.empty()) {
      path.pop_back();
    } else {
      path.push_back(file);
    }
  }
}
