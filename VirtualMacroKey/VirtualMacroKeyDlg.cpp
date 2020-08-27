
// VirtualMacroKeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualMacroKey.h"
#include "VirtualMacroKeyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVirtualMacroKeyDlg dialog




CVirtualMacroKeyDlg::CVirtualMacroKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVirtualMacroKeyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bRecord = false;
}

void CVirtualMacroKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MACRO, m_List);
	DDX_Control(pDX, IDC_CMB_TIME, m_cmbTime);
}

BEGIN_MESSAGE_MAP(CVirtualMacroKeyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_RECORD, &CVirtualMacroKeyDlg::OnBnClickedBtnRecord)
	ON_WM_INPUT()
END_MESSAGE_MAP()


// CVirtualMacroKeyDlg message handlers

BOOL CVirtualMacroKeyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	InitUi();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVirtualMacroKeyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVirtualMacroKeyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVirtualMacroKeyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVirtualMacroKeyDlg::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	if(!m_bRecord) return;
	bool bPause = ((CButton*)GetDlgItem(IDC_CHK_PAUSE))->GetCheck() == BST_CHECKED;

	UINT dwSize;
	BYTE* lpb;
	RAWINPUT* raw;

	GetRawInputData(hRawInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	if (dwSize < sizeof (lpb))
	{
		if (GetRawInputData(hRawInput, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) == dwSize)
		{
			raw = (RAWINPUT*)lpb;
			if (raw->header.dwType == RIM_TYPEKEYBOARD) 
			{
				RAWKEYBOARD keyboard = raw->data.keyboard;
				if(bPause && keyboard.VKey == VK_PAUSE) return OnBnClickedBtnRecord();
				// show keyboard key
			}
			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				RAWMOUSE mouse = raw->data.mouse;
				// show mouse key
			}
		}
	}
	CDialogEx::OnRawInput(nInputcode, hRawInput);
}

void CVirtualMacroKeyDlg::OnBnClickedBtnRecord()
{
	m_bRecord = !m_bRecord;
	if(m_bRecord) CRawInput::Register(m_hWnd, m_wRawType = GetInputType());
	else CRawInput::Remove(m_hWnd, m_wRawType);

	GetDlgItem(IDC_CHK_KEYBOARD)->EnableWindow(!m_bRecord);
	GetDlgItem(IDC_CHK_MOUSE)->EnableWindow(!m_bRecord);
	GetDlgItem(IDC_CHK_CURSOR)->EnableWindow(!m_bRecord);
}

void CVirtualMacroKeyDlg::InitUi()
{
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_TIME);

	CString sTimes[] = { _T("Auto Time"), _T("Fixed Time"), _T("No Delay") };
	const int nSize = sizeof(sTimes) / sizeof(CString);
	for (int i = 0; i < nSize; i++)
	{
		pCmb->AddString(sTimes[i]);
	}
	pCmb->SetCurSel(0);
}

WORD CVirtualMacroKeyDlg::GetInputType()
{
	WORD wRawType = 0;

	if(((CButton*)GetDlgItem(IDC_CHK_KEYBOARD))->GetCheck() == BST_CHECKED) wRawType |= RAW_TYPE_KB;
	if(((CButton*)GetDlgItem(IDC_CHK_MOUSE))->GetCheck() == BST_CHECKED) wRawType |= RAW_TYPE_MS;
	if(((CButton*)GetDlgItem(IDC_CHK_CURSOR))->GetCheck() == BST_CHECKED) wRawType |= RAW_TYPE_MS;

	return wRawType;
}
