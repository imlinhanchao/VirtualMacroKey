
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
	if(!m_bRecord) return CDialogEx::OnRawInput(nInputcode, hRawInput);
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
		RAWKEYBOARD keyboard = raw->data.keyboard;
		if(IsUsePause() && keyboard.VKey == VK_PAUSE){
			OnBnClickedBtnRecord();
			goto DONE;
		}
		PushKey(keyboard);
	}

	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		RAWMOUSE mouse = raw->data.mouse;
		PushKey(mouse);
	}

DONE:
	if(lpb != NULL) delete lpb;
	
	CDialogEx::OnRawInput(nInputcode, hRawInput);
}

void CVirtualMacroKeyDlg::OnBnClickedBtnRecord()
{
	m_bRecord = !m_bRecord;
	m_wRawType = GetInputType();

	if(m_wRawType == 0) {
		MessageBox(_T("Please choose one record type less."));
		return;
	}

	if(m_bRecord) CRawInput::Register(m_hWnd, m_wRawType);
	else CRawInput::Remove(m_hWnd, m_wRawType);

	GetDlgItem(IDC_CHK_KEYBOARD)->EnableWindow(!m_bRecord);
	GetDlgItem(IDC_CHK_MOUSE)->EnableWindow(!m_bRecord);
	GetDlgItem(IDC_CHK_CURSOR)->EnableWindow(!m_bRecord);

	SetDlgItemText(IDC_BTN_RECORD, m_bRecord ? _T("Stop") : _T("Record"));
}

void CVirtualMacroKeyDlg::InitUi()
{
	CString sTimes[] = { _T("Auto Time"), _T("Fixed Time"), _T("No Delay") };
	const int nSize = sizeof(sTimes) / sizeof(CString);
	for (int i = 0; i < nSize; i++)
	{
		m_cmbTime.AddString(sTimes[i]);
	}
	m_cmbTime.SetCurSel(0);
}

WORD CVirtualMacroKeyDlg::GetInputType()
{
	WORD wRawType = 0;

	if(((CButton*)GetDlgItem(IDC_CHK_KEYBOARD))->GetCheck() == BST_CHECKED) wRawType |= RAW_TYPE_KB;
	if(((CButton*)GetDlgItem(IDC_CHK_MOUSE))->GetCheck() == BST_CHECKED) wRawType |= RAW_TYPE_MS;
	if(((CButton*)GetDlgItem(IDC_CHK_CURSOR))->GetCheck() == BST_CHECKED) wRawType |= RAW_TYPE_MS;

	return wRawType;
}

void CVirtualMacroKeyDlg::PushKey( RAWKEYBOARD keyboard )
{
	if (!IsRecordKeyboard()) return;

	static RAWKEYBOARD keyLast = {0};

	if(keyboard.Flags == keyLast.Flags 
	&& keyboard.VKey  == keyLast.VKey) return;

	PushTime();

	MACROS_UNIT unit;
	unit.dwValue = keyboard.VKey;

	CString sName = CHotKey::GetHotKeyName(keyboard.VKey);

	if (keyboard.Flags == RI_KEY_BREAK)
	{
		sName += _T(" Up");
		unit.nType = MACROS_KEYBOARDUP;
	}
	else if (keyboard.Flags == RI_KEY_MAKE)
	{
		sName += _T(" Down");
		unit.nType = MACROS_KEYBOARDOWN;
	}

	m_macros.lstMacro.push_back(unit);
	m_List.AddString(sName);
	m_List.SetTopIndex(m_List.GetCount() - 1);
	keyLast = keyboard;
}

void CVirtualMacroKeyDlg::PushKey( RAWMOUSE mouse )
{
	if(!IsRecordMouse()) return;

	CString sMouse = _T(""); 

	MACROS_UNIT unit;
	switch(mouse.usButtonFlags)
	{
	case RI_MOUSE_LEFT_BUTTON_DOWN:		
	case RI_MOUSE_LEFT_BUTTON_UP:		sMouse += _T("Mouse Left ");
		unit.dwValue = MOUSE_LEFT;
		break;
	case RI_MOUSE_RIGHT_BUTTON_DOWN:	
	case RI_MOUSE_RIGHT_BUTTON_UP:		sMouse += _T("Mouse Right ");
		unit.dwValue = MOUSE_RIGHT;
		break;
	case RI_MOUSE_MIDDLE_BUTTON_DOWN:	
	case RI_MOUSE_MIDDLE_BUTTON_UP:		sMouse += _T("Mouse Middle ");
		unit.dwValue = MOUSE_MIDDLE;
		break;
	default: return;
	}

	if(mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN
		|| mouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_DOWN
		|| mouse.usButtonFlags == RI_MOUSE_MIDDLE_BUTTON_DOWN)
	{
		sMouse += _T("Down");
		unit.nType = MACROS_MOUSEDOWN;
	}
	else if(mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP
		|| mouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_UP
		|| mouse.usButtonFlags == RI_MOUSE_MIDDLE_BUTTON_UP)
	{
		sMouse += _T("Up");
		unit.nType = MACROS_MOUSEUP;
	}
	else
	{
		return;
	}

	PushTime();

	if(IsRecordCursorPos())
	{
		POINT pt;
		CString sMousePt;
		GetCursorPos(&pt);
		unit.nType = MACROS_MOUSEX;
		unit.dwValue = pt.x;
		m_macros.lstMacro.push_back(unit);
		unit.nType = MACROS_MOUSEY;
		unit.dwValue = pt.y;
		m_macros.lstMacro.push_back(unit);
		sMousePt.Format(_T("X:%d, Y:%d"), pt.x, pt.y);
		m_List.AddString(sMousePt);
	}

	m_macros.lstMacro.push_back(unit);
	m_List.AddString(sMouse);
}

void CVirtualMacroKeyDlg::PushTime()
{
	TIME_TYPE time = GetTimeType();
	if (m_dwLastTime <= 0 || time == TIME_NONE) {
		m_dwLastTime = ::GetTickCount();
		return;
	}

	CString sTime;
	DWORD dwTick = GetTimeElapse(m_dwLastTime);
	if (time == TIME_FIXEDTIME)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_TIME))->GetWindowText(sTime);
		dwTick = _ttoi(sTime);
	}
	sTime.Format(_T("Delay %d ms"), dwTick);
	m_List.AddString(sTime);

	MACROS_UNIT unit;
	unit.dwValue = dwTick;
	unit.nType = MACROS_DELAY;
	m_macros.lstMacro.push_back(unit);

	m_dwLastTime = ::GetTickCount();
}

bool CVirtualMacroKeyDlg::IsUsePause()
{
	return ((CButton*)GetDlgItem(IDC_CHK_PAUSE))->GetCheck() == BST_CHECKED;
}

bool CVirtualMacroKeyDlg::IsRecordKeyboard()
{
	return ((CButton*)GetDlgItem(IDC_CHK_KEYBOARD))->GetCheck() == BST_CHECKED;
}

bool CVirtualMacroKeyDlg::IsRecordMouse()
{
	return ((CButton*)GetDlgItem(IDC_CHK_MOUSE))->GetCheck() == BST_CHECKED;
}

bool CVirtualMacroKeyDlg::IsRecordCursorPos()
{
	return ((CButton*)GetDlgItem(IDC_CHK_CURSOR))->GetCheck() == BST_CHECKED;
}

TIME_TYPE CVirtualMacroKeyDlg::GetTimeType()
{
	return (TIME_TYPE)m_cmbTime.GetCurSel();
}

DWORD CVirtualMacroKeyDlg::GetTimeElapse( DWORD dwOldTick )
{
	return (::GetTickCount() - dwOldTick);
}
