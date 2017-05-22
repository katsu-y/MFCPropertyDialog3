// MFCPropertyDialogWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCPropertyDialog.h"
#include "MFCPropertyDialogWindow.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCPropertyDialog dialog

CMFCPropertyDialog::CMFCPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCPropertyDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_wndPropList1);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID2, m_wndPropList2);
}

BEGIN_MESSAGE_MAP(CMFCPropertyDialog, CDialogEx)
END_MESSAGE_MAP()

// CMFCPropertyDialog message handlers

BOOL CMFCPropertyDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// create base window
	m_propBase1.CreateEx(WS_EX_CONTROLPARENT, NULL, NULL, AFX_WS_DEFAULT_VIEW, 
			CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);

	m_propBase2.CreateEx(WS_EX_CONTROLPARENT, NULL, NULL, AFX_WS_DEFAULT_VIEW, 
			CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);

	{ // Block PropGrid1
		// adjust window size to the same as m_wndPropList1.
		CWnd *pPlaceHolder = GetDlgItem( IDC_MFCPROPERTYGRID1 );
		CRect rc;
		pPlaceHolder->GetWindowRect(&rc);
		GetDesktopWindow()->MapWindowPoints(this,&rc);
		m_propBase1.SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),0);
		// move window position
		m_propBase1.SetChildCtrl( &m_wndPropList1 );

		InitPropList(m_wndPropList1,m_data1,_T("Data L"));
	}

	{ // Block PropGrid2
		// adjust window size to the same as m_wndPropList2.
		CWnd *pPlaceHolder = GetDlgItem( IDC_MFCPROPERTYGRID2 );
		CRect rc;
		pPlaceHolder->GetWindowRect(&rc);
		GetDesktopWindow()->MapWindowPoints(this,&rc);
		m_propBase2.SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),0);
		// move window position
		m_propBase2.SetChildCtrl( &m_wndPropList2 );

		InitPropList(m_wndPropList2,m_data2,_T("Data R"));
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCPropertyDialog::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList1.SetFont(&m_fntPropList);
	m_wndPropList2.SetFont(&m_fntPropList);
}

void CMFCPropertyDialog::InitPropList(CMFCPropertyGridCtrl& propGrid,PBYTE pByte,LPCTSTR pszTitle)
{
	SetPropListFont();

	propGrid.EnableHeaderCtrl(FALSE);
	propGrid.EnableDescriptionArea(FALSE);
	propGrid.SetDescriptionRows(1);
	propGrid.SetVSDotNetLook(TRUE);
	propGrid.MarkModifiedProperties(TRUE);

	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(pszTitle);
	CMFCPropertyGridProperty* pProp;
	int i;
	WCHAR szPropText[MAX_PATH];
	WCHAR szPropTitle[MAX_PATH];

	for(i = 0; i < _MAX_DATA_COUNT; i++)
	{
		StringCchPrintf(szPropTitle,MAX_PATH,L"Item:%d",i+1);
		StringCchPrintf(szPropText,MAX_PATH,L"%02X",pByte[i]);

		pProp = new CMFCPropertyGridProperty( szPropTitle, (_variant_t) szPropText, NULL, 0, L"AA",L"__",_T("1234567890ABCDEFabcdef"));

		pGroup->AddSubItem(pProp);
	}

	propGrid.AddProperty(pGroup);
}

void CMFCPropertyDialog::DataCollect(CMFCPropertyGridCtrl& propGrid,PBYTE pByte)
{
	int iCount = propGrid.GetPropertyCount();

	CMFCPropertyGridProperty *pGroup = propGrid.GetProperty(0);
	int i,cItems = pGroup->GetSubItemsCount();
	for(i = 0; i < cItems; i++)
	{
		CMFCPropertyGridProperty *pItem = pGroup->GetSubItem(i);

		const COleVariant& v = pItem->GetValue();

		BSTR s = v.bstrVal;

		pByte[i] = (BYTE)_tcstoul(s,__nullptr,16);
	}
}

void CMFCPropertyDialog::OnOK()
{
	DataCollect(m_wndPropList1,m_data1);
	DataCollect(m_wndPropList2,m_data2);

	CDialogEx::OnOK();
}
