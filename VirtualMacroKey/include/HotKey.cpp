#include "StdAfx.h"
#include "HotKey.h"

namespace Easy {

HOTKEYCALLBACK CHotKey::m_pCallback = NULL;

CHotKey::CHotKey(void)
{
}


CHotKey::~CHotKey(void)
{
}

void CHotKey::SetCallback( HOTKEYCALLBACK pCallback )
{
	m_pCallback = pCallback;
}

bool CHotKey::RemoveHotKey( DWORD dwHotKey )
{
	return UnregisterHotKey(AfxGetMainWnd()->GetSafeHwnd(), dwHotKey);
}

bool CHotKey::SetHotKey( DWORD dwHotKey )
{
	return RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(), dwHotKey, GetModifiers(dwHotKey), dwHotKey & 0xff);
}

UINT CHotKey::GetModifiers( DWORD dwHotKey )
{
	UINT uModifiers = 0;
	dwHotKey >>= 0x08;
	if(dwHotKey & HOTKEYF_ALT)		uModifiers |= MOD_ALT;
	if(dwHotKey & HOTKEYF_SHIFT)	uModifiers |= MOD_SHIFT;
	if(dwHotKey & HOTKEYF_CONTROL)	uModifiers |= MOD_CONTROL;
	return uModifiers;
}

CString CHotKey::GetHotKeyName( DWORD dwHotKey )
{
	DWORD dwKeyCode = dwHotKey & 0xff;
	CString sKey = CHotKeyCtrl::GetKeyName(dwKeyCode, IsExtendedKey(dwKeyCode));

	if (dwKeyCode == VK_PAUSE)
	{
		sKey = _T("Pause");
	}

	if(_T(" ") == sKey) 
	{
		sKey = _T("Space");
	}
	else if(sKey.IsEmpty())
	{
		sKey = CHotKeyCtrl::GetKeyName(dwKeyCode, TRUE);
	}

	dwHotKey >>= 0x08;
	if(dwHotKey & HOTKEYF_ALT)		sKey = _T("Alt + ") + sKey;
	if(dwHotKey & HOTKEYF_SHIFT)	sKey = _T("Shift + ") + sKey;
	if(dwHotKey & HOTKEYF_CONTROL)	sKey = _T("Ctrl + ") + sKey;

	return sKey;
}

bool CHotKey::IsExtendedKey(DWORD vKey)
{
	DWORD dwKeyCodes[] = {
		VK_DELETE,
		VK_HOME,
		VK_PRIOR,
		VK_NEXT,
		VK_END,

		VK_NUMLOCK,
		VK_INSERT,

		VK_LEFT,
		VK_UP,
		VK_RIGHT,
		VK_DOWN,
	};

	int nCount = sizeof(dwKeyCodes) / sizeof(DWORD);
	for (int i = 0; i < nCount; i++)
	{
		if (vKey == dwKeyCodes[i]) return true;
	}
	return false;
}

void CHotKey::Execute( DWORD dwHotKey )
{

}

}