#pragma once
#include "game.h"

#include <fstream>
// CBombItWnd 框架

class CBombItWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CBombItWnd)
public:
	CBombItWnd();           // 动态创建所使用的受保护的构造函数
	virtual ~CBombItWnd();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CMenu menu;

	int dir = 0; //人物方向
	int PosX = 0, PosY = 0; //人物当前位置 PosX:所在列，PosY:所在行
	int died = 0; //人物上局是否已死亡
	int BombNum = 1; //当前拥有炸弹数
	int BombPosX = -1, BombPosY = -1;
	int statistic = 0; //记录可被炸物体的数量
	CBitmap* pBombing[5];
	CDC* mdc;
	CBitmap* pPlayerPic[2][4];
	CBitmap* pGround;
	CBitmap* pTree;
	CBitmap* pMashroom;
	CBitmap* pWall;
	CBitmap* pShield;
	CBitmap* pBomb;
	std::ofstream fOut;
	int count = 0;
public:
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void SetDied(int N);
	int getStatistic();
	void minusOneObject();
	afx_msg void OnAbout();
	afx_msg void OnQuit();
	afx_msg void OnRestart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


