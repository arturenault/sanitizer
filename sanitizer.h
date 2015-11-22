#ifndef __SANITIZER_H__
#define __SANITIZER_H__

using namespace std;

bool sanitizeField(const string& name, string& output);

bool isAlphanumeric(unsigned char letter);

unsigned char escapeChars(const string& name, string::const_iterator& iter);

#endif
