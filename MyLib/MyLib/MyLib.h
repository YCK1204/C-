#ifndef MYLIB_H
# define MYLIB_H

#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define DLLEXPORT __declspec(dllexport)


DLLEXPORT char* Trim(char* str);
DLLEXPORT char* _Substr(char* str, size_t start);
DLLEXPORT char* Substr(char* str, size_t start, size_t count);
DLLEXPORT char* Getline(int fd);
DLLEXPORT char** Split(char* str, int delim, bool trim);
DLLEXPORT char* StrJoin(char* s1, char* s2);
DLLEXPORT int StrFindByChar(char* str, char target, size_t pos);
DLLEXPORT int StrFindByString(char* str, char* target, size_t pos);
DLLEXPORT void ReplaceString(char** oldString, char* newString);
DLLEXPORT char* IntToAscii(int n);
DLLEXPORT char* CharToString(char c);
DLLEXPORT void _InsertString(char** original, size_t pos, char c);
DLLEXPORT void InsertString(char** original, size_t pos, char* str);
DLLEXPORT char* MyStrcpy(char* dest, char* src, size_t pos);
DLLEXPORT char* MyStrncpy(char* dest, char* src, size_t pos, size_t n);

#endif