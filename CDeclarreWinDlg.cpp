// CDeclarreWinDlg.cpp: 实现文件
//
#define _WIN32_WINNT _WIN32_WINNT_WIN10

#include "game.h"
#include "CDeclarreWinDlg.h"
#include "afxdialogex.h"


// CDeclarreWinDlg 对话框

IMPLEMENT_DYNAMIC(CDeclarreWinDlg, CDialog)

CDeclarreWinDlg::CDeclarreWinDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CDeclarreWinDlg, pParent)
{

}

CDeclarreWinDlg::~CDeclarreWinDlg()
{
}

void CDeclarreWinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDeclarreWinDlg, CDialog)
END_MESSAGE_MAP()


// CDeclarreWinDlg 消息处理程序
