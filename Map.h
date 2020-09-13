#pragma once
#ifndef MAP_H_
#define MAP_H_
#include "game.h"
#include "CBombItWnd.h"

#define ROW_NUM 15
#define COLUMN_NUM 15

#define BOMB_TIME 700


class Map
{
public :
	Map(CFrameWnd* pWnd);
	~Map();
	void initMap();
	void drawGround(CPaintDC& dc, int PosX, int PosY);
	void drawTree(CPaintDC& dc, int PosX, int PosY);
	void drawMashroom(CPaintDC& dc, int PosX, int PosY);
	void drawWall(CPaintDC& dc, int PosX, int PosY);
	void drawBomb(CPaintDC& dc, int PosX, int PosY);
	void drawShield(CPaintDC& dc, int PosX, int PosY);
	void drawBombing(CBombItWnd* Wnd, CPaintDC& dc, int PosX, int PosY);
	void drawPlayer(CPaintDC& dc, int PosX, int PosY, int dir, int WithBomb = 0);
public:
	int map[ROW_NUM][COLUMN_NUM] = {};
	int GameSum = 0;
	int MapNum = 1;
private:
	CBitmap* pBombing[5];
	CDC* mdc;
	CBitmap* pPlayerPic[2][4];
	CBitmap* pGround;
	CBitmap* pTree;
	CBitmap* pMashroom;
	CBitmap* pWall;
	CBitmap* pShield;
	CBitmap* pBomb;
};

#endif // !MAP_H_

