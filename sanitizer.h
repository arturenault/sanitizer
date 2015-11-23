#ifndef __SANITIZER_H__
#define __SANITIZER_H__

using namespace std;

bool sanitizeFields(const string& name, string& outputFileName, string& outputDataField);

bool isAlphanumeric(unsigned char letter);

bool isSpaceOrTab(unsigned char letter);

unsigned char escapeChars(const string& name, string::const_iterator& iter);

#endif
