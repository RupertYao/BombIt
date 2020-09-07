// AboutBoxDlg.cpp: 实现文件
//

#include "game.h"
#include "AboutBoxDlg.h"
#include "afxdialogex.h"


// AboutBoxDlg 对话框

IMPLEMENT_DYNAMIC(AboutBoxDlg, CDialog)

AboutBoxDlg::AboutBoxDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_AboutBoxDlg, pParent)
{

}

AboutBoxDlg::~AboutBoxDlg()
{
}

void AboutBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AboutBoxDlg, CDialog)
END_MESSAGE_MAP()


// AboutBoxDlg 消息处理程序
