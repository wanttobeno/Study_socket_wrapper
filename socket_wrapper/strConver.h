#pragma once

#include <string.h>
#include <xstring>

// UTF8 <=> Unicode
std::string UnicodeToUTF8(const std::wstring & wstr);

std::wstring UTF8ToUnicode(const std::string & str);

std::string UnicodeToANSI(const std::wstring & wstr);

std::wstring ANSIToUnicode(const std::string & str);

// UTF-8 <=> ANSI
std::string UTF8ToANSI(const std::string & str);

std::string ANSIToUTF8(const std::string & str);
