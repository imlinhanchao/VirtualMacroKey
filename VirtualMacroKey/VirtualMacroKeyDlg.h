
// VirtualMacroKeyDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <vector>
using namespace std;

typedef enum _MOUSE_KEY
{
	MOUSE_LEFT = 1,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,

}MOUSE_KEY, *PMOUSE_KEY;

typedef enum _MACROS_TYPE
{
	MACROS_KEYBOARDUP = 0, 
	MACROS_KEYBOARDOWN, 
	MACROS_MOUSEUP,
	MACROS_MOUSEDOWN,
	MACROS_MOUSEX,
	MACROS_MOUSEY,
	MACROS_DELAY,

} MACROS_TYPE, *PMACROS_TYPE;

typedef struct _MACROS_UNIT
{
	MACROS_TYPE nType;
	DWORD dwValue;

} MACROS_UNIT, *PMACROS_UNIT;

typedef enum _RUN_TYPE
{
	RUN_ONCE = 0,
	RUN_LOOP,
	RUN_FIRE,

}RUN_TYPE, *PRUN_TYPE;

typedef enum _TIME_TYPE
{
	TIME_AUTO = 0,
	TIME_FIXEDTIME,
	TIME_NONE,

}TIME_TYPE, *PTIME_TYPE;

typedef struct _MACROS_ATTR
{
	int nIcon;
	TIME_TYPE TimeMode;
	CString sName;

} MACROS_ATTR, *PMACROS_ATTR;

typedef struct _MACROS
{
	MACROS_ATTR attr;
	vector<MACROS_UNIT> lstMacro;

} MACROS, *PMACROS;

// CVirtualMacroKeyDlg dialog
class CVirtualMacroKeyDlg : public CDialogEx
{
// Construction
public:
	CVirtualMacroKeyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VIRTUALMACROKEY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
	afx_msg void OnBnClickedBtnRecord();
	DECLARE_MESSAGE_MAP()

	void InitUi();
	WORD GetInputType();

	bool IsUsePause();
	bool IsRecordKeyboard();
	bool IsRecordMouse();
	bool IsRecordCursorPos();

	void PushKey(RAWMOUSE mouse);
	void PushKey(RAWKEYBOARD keyboard);
	void PushTime();

	TIME_TYPE GetTimeType();
	DWORD GetTimeElapse(DWORD dwOldTick);


	bool	  m_bRecord;
	WORD	  m_wRawType;

	CListBox  m_List;
	CComboBox m_cmbTime;
	DWORD	  m_dwLastTime;

	MACROS	  m_macros;
};
