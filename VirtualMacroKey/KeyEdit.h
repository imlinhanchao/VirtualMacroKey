#pragma once
#include "afxwin.h"
#include <vector>

class CKeyEdit : public CEdit
{
public:
	CKeyEdit(void);
	~CKeyEdit(void);


	void SetSingleKey(bool bSingle, bool bHotKey=false);

private:
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnKillfocus();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

	void PushKey(USHORT vKeyCode);
	void ShowKeys(void);
	bool IsExtendKey(USHORT vKeyCode);

	std::vector<USHORT> m_lstKeyCode;
	bool m_bSingleKey;
	bool m_bHotKey;
	bool m_bRecord;
};

