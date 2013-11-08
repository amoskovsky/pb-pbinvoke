#pragma once
#pragma warning(disable: 4786)
#pragma warning(disable: 4800)
#pragma warning(disable: 4996)

//#define _DEBUG_MSG

#define NOMINMAX

#include <windows.h>
#include <vector>
#include <string>
#include <tchar.h>

using namespace std;


#define DLLEXPORT __declspec(dllexport)
#define PBCALL __stdcall


#ifdef _DEBUG_MSG
#define dprintf CUtil::putMsg
#define dwprintf CUtil::wputMsg
#define truncatelog CUtil::deleteLog
#else
#define dprintf if (0) CUtil::nullMsg
#define dwprintf if (0) CUtil::wnullMsg
#define truncatelog if (0) CUtil::deleteLog
#endif

#define IsUnicode(marker) (marker[1] == 0)

class CUtil  
{
public:
	static TCHAR * logFile;
	static TCHAR * clone(TCHAR * src);
	CUtil();
	virtual ~CUtil();

	static void deleteLog() {
		_tunlink(logFile);
	}
	static void putMsg(TCHAR * fmt, ...) {
		FILE *f = openLog();
		va_list marker;
		va_start( marker, fmt );
		_vftprintf(f, fmt, marker);
		_vtprintf(fmt, marker);
		if (fmt[0]==TEXT('-') ) {
			_tprintf("\n\n");
			_ftprintf(f, "\n\n");
		}
		va_end( marker );
		closeLog(f);
	}
	static void wputMsg(wchar_t * fmt, ...) {
		FILE *f = openLog();
		va_list marker;
		va_start( marker, fmt );
		vfwprintf(f, fmt, marker);
		va_end( marker );
		closeLog(f);
	}
	static inline void nullMsg(TCHAR * fmt, ...) {
	}
	static inline void wnullMsg(wchar_t * fmt, ...) {
	}
	static inline char * toString(int v) {
		static char buf[20];
		sprintf(buf, "%i", v);
		return buf;
	}
	static wstring toLower(const wstring &s) {
		wstring res = s;
		_wcslwr((wchar_t*)res.c_str());
		return res;
	}
	static string toLower(const string &s) {
		string res = s;
		strlwr((char*)res.c_str());
		return res;
	}
private:
	static FILE * openLog() {
		FILE *f = _tfopen(logFile, TEXT("at+"));
#ifdef _UNICODE
		//_ftprintf(f, TEXT("W>"));
#else
		//_ftprintf(f, TEXT("A>"));
#endif
		return f;
	}
	static void closeLog(FILE*f) {
		if (f)
			fclose(f);
	}


	
};

