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
	// TODO: �ڴ����ר�ô����/����û���
	m_pMainWnd = new CBombItWnd;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return CWinApp::InitInstance();
}
