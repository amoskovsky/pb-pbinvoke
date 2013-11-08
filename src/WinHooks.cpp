#include "stdafx.h"
#include "WinHooks.h"


bool IWinHookHelper::skip(HHOOK hhk, PHookProcArgs args, vector<UINT>&messageFilter, LRESULT &result)
{
	if (args->nCode >= 0) {
		UINT& message = getMessage(args);
		for (size_t i = 0; i < messageFilter.size(); i ++) {
			if (messageFilter[i] == message)
				return false;
		}
	}
	result = CallNextHookEx(hhk, args->nCode, args->wParam, args->lParam);
	return true;
}
