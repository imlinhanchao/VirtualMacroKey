#pragma once
#include "Base.h"

namespace Easy {

typedef void (CALLBACK* HOTKEYCALLBACK)(DWORD dwHotKey, LPVOID lpParam);

class CHotKey : public Base
{
public:
	CHotKey(void);
	~CHotKey(void);

	static void SetCallback(HOTKEYCALLBACK pCallback);
	static bool RemoveHotKey(DWORD dwHotKey);
	static bool SetHotKey(DWORD dwHotKey);
	static CString GetHotKeyName(DWORD dwHotKey);

	static void Execute(DWORD dwHotKey);

protected:
	static UINT GetModifiers(DWORD dwHotKey);
	static bool IsExtendedKey(DWORD vKey);

	static HOTKEYCALLBACK m_pCallback;
};

}