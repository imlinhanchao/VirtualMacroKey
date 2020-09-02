#pragma once
#include "afxwin.h"

class CKeyEdit : public CEdit
{
public:
	CKeyEdit(void);
	~CKeyEdit(void);


	DECLARE_MESSAGE_MAP()
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
};

