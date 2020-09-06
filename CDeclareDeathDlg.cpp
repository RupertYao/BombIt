// CDeclareDeathDlg.cpp: 实现文件
//
#define _WIN32_WINNT _WIN32_WINNT_WIN10

#include "game.h"
#include "CDeclareDeathDlg.h"
#include "afxdialogex.h"


// CDeclareDeathDlg 对话框

IMPLEMENT_DYNAMIC(CDeclareDeathDlg, CDialog)

CDeclareDeathDlg::CDeclareDeathDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CDeclareDeathDlg, pParent)
{

}

CDeclareDeathDlg::~CDeclareDeathDlg()
{
}

void CDeclareDeathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDeclareDeathDlg, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDeclareDeathDlg 消息处理程序


int CDeclareDeathDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	
	return 0;
}
