
// VirtualMacroKeyDlg.h : header file
//

#pragma once
#include "afxwin.h"


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

	bool	  m_bRecord;
	WORD	  m_wRawType;

	CListBox  m_List;
	CComboBox m_cmbTime;
};
