#include "strConver.h"

#include <memory>
#include <windows.h>

// UTF8 <=> Unicode
std::string UnicodeToUTF8(const std::wstring & wstr)
{
	std::string out;
	int len = wstr.length();
	const wchar_t* in = wstr.c_str();
	size_t nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, in, len, NULL, 0, NULL, NULL);

	char* lpUTF8 = new char[nUtf8Len + 1];
	memset(lpUTF8, 0, nUtf8Len);

	WideCharToMultiByte(CP_UTF8, 0, in, len, lpUTF8, nUtf8Len, NULL, NULL);
	out.assign(lpUTF8, nUtf8Len);

	delete[] lpUTF8;
	lpUTF8 = NULL;
	return out;
}

std::wstring UTF8ToUnicode(const std::string & str)
{
	std::wstring out;
	int len = str.length();
	const char* in = str.c_str();
	wchar_t* lpBuf = new wchar_t[len + 1];
	size_t out_len = (len + 1) * sizeof(wchar_t);

	memset(lpBuf, 0, (len + 1) * sizeof(wchar_t));

	out_len = ::MultiByteToWideChar(CP_UTF8, 0, in, len, lpBuf, len * sizeof(wchar_t));
	out.assign(lpBuf, out_len);

	delete[] lpBuf;
	lpBuf = NULL;
	return out;
}

//Unicode <= > ANSI
std::string UnicodeToANSI(const std::wstring & wstr)
{
	std::string str;
	int nLen = wstr.length() * 2 + 100;

	char* pChar = new char[nLen];
	if (pChar)
	{
		memset(pChar, 0, nLen);
		::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.length(), pChar, nLen, 0, NULL);
		str = pChar;
		delete[] pChar;
		pChar = NULL;
	}
	return str;
}

std::wstring ANSIToUnicode(const std::string & str)
{
	std::wstring out;
	const char* in = str.c_str();
	int len = str.length();
	int wbufferlen = (int)::MultiByteToWideChar(CP_ACP, 0, in, len, NULL, 0);
	wchar_t* lpwbuffer = new wchar_t[wbufferlen + 1];

	wbufferlen = (int)::MultiByteToWideChar(CP_ACP, 0, in, len, lpwbuffer, wbufferlen);
	lpwbuffer[wbufferlen] = '\0';
	out.assign(lpwbuffer, wbufferlen);
	delete[] lpwbuffer;
	lpwbuffer = NULL;
	return out;
}

// UTF-8 <=> ANSI
std::string UTF8ToANSI(const std::string & str)
{
	return UnicodeToANSI(UTF8ToUnicode(str));
}

std::string ANSIToUTF8(const std::string & str)
{
	return UnicodeToUTF8(ANSIToUnicode(str));
}