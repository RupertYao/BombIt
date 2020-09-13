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
	m_pMainWnd = new CBombItWnd;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return CWinApp::InitInstance();
}
