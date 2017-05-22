// MFCPropertyDialogWindow.h : header file
//

#pragma once

#include "PropertyBase.h"

#define _MAX_DATA_COUNT 256

// CMFCPropertyDialog dialog
class CMFCPropertyDialog : public CDialogEx
{
// Construction
public:
	CMFCPropertyDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCPROPERTYDIALOG_DIALOG };

	BYTE m_data1[_MAX_DATA_COUNT];
	BYTE m_data2[_MAX_DATA_COUNT];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	CPropertyBase m_propBase1;
	CPropertyBase m_propBase2;

	CMFCPropertyGridCtrl m_wndPropList1;
	CMFCPropertyGridCtrl m_wndPropList2;

	void InitPropList(CMFCPropertyGridCtrl& propGrid,PBYTE pByte,LPCTSTR pszTitle);
	void DataCollect(CMFCPropertyGridCtrl& propGrid,PBYTE pByte);
	void SetPropListFont();
	CFont m_fntPropList;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};
