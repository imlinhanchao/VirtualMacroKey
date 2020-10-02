#include "StdAfx.h"
#include "KeyEdit.h"
#include "include/RawInput.h"
#include "include/HotKey.h"


CKeyEdit::CKeyEdit(void)
{
	m_bHotKey = false;
	m_bSingleKey = false;
	m_bRecord = false;
}


CKeyEdit::~CKeyEdit(void)
{
}
BEGIN_MESSAGE_MAP(CKeyEdit, CEdit)
	ON_WM_INPUT()
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CKeyEdit::OnEnSetfocus)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &CKeyEdit::OnEnKillfocus)
	ON_WM_CHAR()
END_MESSAGE_MAP()


void CKeyEdit::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	UINT dwSize = 0;
	BYTE* lpb = NULL;
	RAWINPUT* raw = NULL;

	GetRawInputData(hRawInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	if (dwSize == 0) goto DONE;

	lpb = new BYTE[dwSize];
	if (GetRawInputData(hRawInput, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		goto DONE;

	raw = (RAWINPUT*)lpb;
	if (raw->header.dwType == RIM_TYPEKEYBOARD) 
	{
		static RAWKEYBOARD keyLast = {0};
		RAWKEYBOARD keyboard = raw->data.keyboard;

		if(keyboard.Flags == keyLast.Flags 
			&& keyboard.VKey  == keyLast.VKey) goto DONE;

		if(keyboard.Flags & RI_KEY_BREAK){
			m_bRecord = false;
		}
		else if (keyboard.Flags == RI_KEY_MAKE || keyboard.Flags == RI_KEY_E0)
		{
			if (!m_bRecord || m_bSingleKey) m_lstKeyCode.clear();
			PushKey(keyboard.VKey);
			m_bRecord = true;
		}

		keyLast = keyboard;
	}

DONE:
	if(lpb != NULL) delete lpb;

	CEdit::OnRawInput(nInputcode, hRawInput);
}


void CKeyEdit::OnEnSetfocus()
{
	CRawInput::Register(m_hWnd, RAW_TYPE_KB);
}


void CKeyEdit::OnEnKillfocus()
{
	CRawInput::Remove(m_hWnd, RAW_TYPE_KB);
}


void CKeyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	// CEdit::OnChar(nChar, nRepCnt, nFlags);
}


BOOL CKeyEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CKeyEdit::PushKey( USHORT vKeyCode )
{
	if (m_bHotKey && IsExtendKey(vKeyCode))
		m_lstKeyCode.clear();

	m_lstKeyCode.push_back(vKeyCode);
	ShowKeys();
}

void CKeyEdit::ShowKeys( void )
{
	CString sValue;
	for (int i = 0; i < m_lstKeyCode.size(); i++)
	{
		sValue += CHotKey::GetHotKeyName(m_lstKeyCode.at(i)) + _T(" + ");
	}
	sValue = sValue.Trim(_T(" + "));
	SetWindowText(sValue);
}

void CKeyEdit::SetSingleKey( bool bSingle, bool bHotKey/*=false*/ )
{
	m_bSingleKey = bSingle;
	m_bHotKey = bHotKey;
}

bool CKeyEdit::IsExtendKey( USHORT vKeyCode )
{
	return (VK_SHIFT == vKeyCode || 
			VK_CONTROL == vKeyCode || 
			VK_MENU == vKeyCode || 
			VK_LWIN == vKeyCode || 
			VK_RWIN == vKeyCode );
}
