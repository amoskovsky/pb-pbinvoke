#pragma once
#include <boost/shared_ptr.hpp>
//using namespace 
#pragma pack(push, 4)
typedef struct HookProcArgs {
	int nCode;
    WPARAM wParam;
    LPARAM lParam;
} HookProcArgs, *PHookProcArgs ;
#pragma pack(pop)

struct IWinHookHelper {
	virtual UINT& getMessage(PHookProcArgs args) = 0;
	virtual ~IWinHookHelper() {}
	bool skip(HHOOK hhk, PHookProcArgs args, vector<UINT>&messageFilter, LRESULT &result);
};

typedef boost::shared_ptr<IWinHookHelper> WinHookHelper;
