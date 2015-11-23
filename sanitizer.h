#ifndef __SANITIZER_H__
#define __SANITIZER_H__

using namespace std;

bool SanitizeFields(const string& name, string& outputFileName,
                    string& outputDataField);

bool SanitizeField(string::const_iterator& iter,
                   const string::const_iterator& end, string& output);

bool IsAlphanumeric(unsigned char letter);

bool IsSpaceOrTab(unsigned char letter);

unsigned char EscapeChars(string::const_iterator& iter,
                          const string::const_iterator& end);

#endif
