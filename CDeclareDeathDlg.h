#pragma once


// CDeclareDeathDlg 对话框

class CDeclareDeathDlg : public CDialog
{
	DECLARE_DYNAMIC(CDeclareDeathDlg)

public:
	CDeclareDeathDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDeclareDeathDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDeclareDeathDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
