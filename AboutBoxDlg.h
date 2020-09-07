#pragma once
#include "game.h"

// AboutBoxDlg 对话框

class AboutBoxDlg : public CDialog
{
	DECLARE_DYNAMIC(AboutBoxDlg)

public:
	AboutBoxDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AboutBoxDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AboutBoxDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
