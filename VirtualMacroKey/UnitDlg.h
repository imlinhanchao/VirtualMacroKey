#pragma once


// CUnitDlg dialog

class CUnitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitDlg)

public:
	CUnitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUnitDlg();

// Dialog Data
	enum { IDD = IDD_DLG_UINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()


};
