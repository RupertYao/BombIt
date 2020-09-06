#pragma once


// CDeclarreWinDlg 对话框

class CDeclarreWinDlg : public CDialog
{
	DECLARE_DYNAMIC(CDeclarreWinDlg)

public:
	CDeclarreWinDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDeclarreWinDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDeclarreWinDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
