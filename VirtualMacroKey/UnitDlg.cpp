// UnitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualMacroKey.h"
#include "UnitDlg.h"
#include "afxdialogex.h"


// CUnitDlg dialog

IMPLEMENT_DYNAMIC(CUnitDlg, CDialogEx)

CUnitDlg::CUnitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUnitDlg::IDD, pParent)
{

}

CUnitDlg::~CUnitDlg()
{
}

void CUnitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_KEY, m_editKey);
}


BEGIN_MESSAGE_MAP(CUnitDlg, CDialogEx)
END_MESSAGE_MAP()


// CUnitDlg message handlers


BOOL CUnitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_editKey.SetSingleKey(false, true);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
