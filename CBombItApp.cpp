#include "CBombItApp.h"
#include "CBombItWnd.h"

CBombItApp theApp;

CBombItApp::CBombItApp()
{
}

CBombItApp::~CBombItApp()
{
}

BOOL CBombItApp::InitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pMainWnd = new CBombItWnd;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return CWinApp::InitInstance();
}
