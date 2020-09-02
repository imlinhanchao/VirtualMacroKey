#include "StdAfx.h"
#include "KeyEdit.h"


CKeyEdit::CKeyEdit(void)
{
}


CKeyEdit::~CKeyEdit(void)
{
}
BEGIN_MESSAGE_MAP(CKeyEdit, CEdit)
	ON_WM_INPUT()
END_MESSAGE_MAP()


void CKeyEdit::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your message handler code here and/or call default

	CEdit::OnRawInput(nInputcode, hRawInput);
}
