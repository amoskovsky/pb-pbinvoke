
#include "stdafx.h"
#include "types/DataTypes.h"
#include "Compiler.h"
#include "Runtime.h"
#include "types/TypeFactory.h"
#include "Core.h"
//#include <psapi.h>
//#pragma comment(lib, "Psapi.lib")
//#include <crc.h>

#define IMP __declspec(dllimport)
#define A __declspec(align(4))

//typedef int iii;
//struct S {char a;};

//int unsigned a;
//int IMP A unsigned aa;
//int unsigned IMP aaa;
//IMP unsigned b;
//unsigned IMP int bb;
//iii IMP c;
//IMP iii cc;
//S IMP d;
//IMP S A dd;
//IMP struct {int b;}  e;
// struct {int b;} IMP ee;
//unsigned __int8 f;
//const int al = __alignof(dd);
//signed unsigned g; 

int ff[][2][2][2];

TCHAR * CUtil::logFile = TEXT("libPbInvoke.log");


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			truncatelog();
			dprintf("DllMain\n");
#ifdef _DEBUG
			dprintf(" DEBUG DLL\n");
#else
			dprintf(" RELEASE DLL\n");
#endif
			Core::resetState();
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



/// debug    

BOOL PBCALL core_setAppHandle(DWORD handle);

void debug() {
	//FN fn;
	//fn();
	//void a(void  i);
	//aaa a;
	//string exe = "pb600.012420654296875";
	//printf("md5=%s\n", CRC::md5(exe.c_str(), exe.size()).c_str());
	//return;
	try {
		CoreInstance.setUnicode(false);
		core_setAppHandle(1);
		Compiler cc;
		string name = cc.compileAsC("int fn(int a[]); struct S{ char a; int b[]; }");
		dprintf("last name='%s'\n", name.c_str());
	}
	catch(RuntimeException &e) {
		printf("'%s'\n", e.getMessage().c_str());
	}
}
