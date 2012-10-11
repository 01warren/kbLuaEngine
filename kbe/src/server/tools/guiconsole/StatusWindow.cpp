// StatusWindow.cpp : implementation file
//

#include "stdafx.h"
#include "guiconsole.h"
#include "StatusWindow.h"


// StatusWindow dialog

IMPLEMENT_DYNAMIC(StatusWindow, CDialog)

StatusWindow::StatusWindow(CWnd* pParent /*=NULL*/)
	: CDialog(StatusWindow::IDD, pParent)
{

}

StatusWindow::~StatusWindow()
{
}

void StatusWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATUS_LIST1, m_statusList);
}


BEGIN_MESSAGE_MAP(StatusWindow, CDialog)
END_MESSAGE_MAP()

BOOL StatusWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	DWORD dwStyle = m_statusList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;					//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;						//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_ONECLICKACTIVATE;
	m_statusList.SetExtendedStyle(dwStyle);				//������չ���

	int idx = 0;
	m_statusList.InsertColumn(idx++, _T("uid"),					LVCFMT_CENTER,	30);
	m_statusList.InsertColumn(idx++, _T("componentType"),		LVCFMT_CENTER,	100);
	m_statusList.InsertColumn(idx++, _T("componentID"),			LVCFMT_CENTER,	120);
	m_statusList.InsertColumn(idx++, _T("cpu"),					LVCFMT_CENTER,	60);
	m_statusList.InsertColumn(idx++, _T("memory"),				LVCFMT_CENTER,	100);
	m_statusList.InsertColumn(idx++, _T("entities"),			LVCFMT_CENTER,	100);
	m_statusList.InsertColumn(idx++, _T("proxies"),				LVCFMT_CENTER,	100);
	m_statusList.InsertColumn(idx++, _T("address"),				LVCFMT_CENTER,	100);
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void StatusWindow::autoWndSize()
{
	CRect rect;
	GetClientRect(&rect);

	m_statusList.MoveWindow(2, 3, rect.right - 2, rect.bottom - 3, TRUE);
	
}
// StatusWindow message handlers
