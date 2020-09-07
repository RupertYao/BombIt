// CBombItWnd.cpp: 实现文件
//

#define _WIN32_WINNT _WIN32_WINNT_WIN10

#include "game.h"
#include "CBombItWnd.h"
#include "CDeclarreWinDlg.h"
#include "CDeclareDeathDlg.h"
#include "AboutBoxDlg.h"
#include "resource.h"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

// CBombItWnd

#define ROW_NUM 15
#define COLUMN_NUM 15

IMPLEMENT_DYNCREATE(CBombItWnd, CFrameWnd)

int map[ROW_NUM][COLUMN_NUM] = {};

void initMap();
void drawGround(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY);
void drawTree(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY);
void drawMashroom(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY);
void drawWall(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY);
void drawBomb(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY);
void drawSheild(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY);
void drawBombing(CBombItWnd* Wnd,HWND hWnd, CBitmap* pBitmap[], CDC* mdc, CPaintDC& dc, int PosX, int PosY);
void drawPlayer(CBitmap* pPlayerPic[][4], CDC* mdc, CPaintDC& dc, int PosX, int PosY, int dir, int WithBomb = 0);


CBombItWnd::CBombItWnd()
{
    this->Create(NULL, _T("BombIt"));
    this->SetWindowTextW(_T("简化Q版泡泡堂"));
    this->SetWindowPos(NULL, 100, 50, 60*COLUMN_NUM +20, 60*ROW_NUM +75, 0);

    menu.LoadMenuW(IDR_MENU1);
    this->SetMenu(&menu);

    mciSendString(_T("open Resource\\bgm.mp3 repeat"), NULL, 0, NULL);
    mciSendString(_T("play Resource\\bgm.mp3 repeat"), NULL, 0, NULL);

    mdc = new CDC;
    CClientDC dc(this);
    mdc->CreateCompatibleDC(&dc);

    initMap();
    WCHAR fileName[32];
    pTree = new CBitmap;
    pTree->m_hObject = LoadImage(NULL, _T("Resource\\tree.bmp"), IMAGE_BITMAP, 60, 60, LR_LOADFROMFILE);
    pMashroom = new CBitmap;
    pMashroom->m_hObject = LoadImage(NULL, _T("Resource\\mashroom.bmp"), IMAGE_BITMAP, 60, 60, LR_LOADFROMFILE);
    pGround = new CBitmap;
    pGround->m_hObject = LoadImage(NULL, _T("Resource\\ground.bmp"), IMAGE_BITMAP, 60, 60, LR_LOADFROMFILE);
    pWall = new CBitmap;
    pWall->m_hObject = LoadImage(NULL, _T("Resource\\wall.bmp"), IMAGE_BITMAP, 60, 60, LR_LOADFROMFILE);
    pShield = new CBitmap;
    pShield->m_hObject = LoadImage(NULL, _T("Resource\\shield.bmp"), IMAGE_BITMAP, 60, 60, LR_LOADFROMFILE);
    pBomb = new CBitmap;
    pBomb->m_hObject = LoadImage(NULL, _T("Resource\\bomb.bmp"), IMAGE_BITMAP, 60, 60, LR_LOADFROMFILE);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pPlayerPic[i][j] = new CBitmap;
            if (i)
            {
                swprintf_s(fileName, _T("Resource\\PlayerWithBomb\\%d.bmp"), j + 1);
            }
            else
            {
                swprintf_s(fileName, _T("Resource\\Player\\%d.bmp"), j + 1);
            }
            pPlayerPic[i][j]->m_hObject = LoadImage(NULL, fileName, IMAGE_BITMAP, 60, 60, LR_LOADFROMFILE);
        }
    }
    for (int i = 0; i < 5; i++)
    {
        pBombing[i] = new CBitmap;
        if (i)
        {
            swprintf_s(fileName, _T("Resource\\Bombing\\%d.bmp"), i);
            pBombing[i]->m_hObject = LoadImage(NULL, fileName, IMAGE_BITMAP, 60, 60, LR_LOADFROMFILE);
        }
        else
        {
            pBombing[i]->m_hObject = LoadImage(NULL, _T("Resource\\Bombing\\center.bmp"), IMAGE_BITMAP, 60, 60, LR_LOADFROMFILE);
        }
    }
    fOut = std::ofstream("temp.txt", std::ios_base::out);
}

CBombItWnd::~CBombItWnd()
{
    mciSendString(_T("close Resource\\bgm.mp3 repeat"), NULL, 0, NULL);
    fOut.close();
    pTree->DeleteObject();
    pMashroom->DeleteObject();
    pGround->DeleteObject();
    pWall->DeleteObject();
    pShield->DeleteObject();
    pBomb->DeleteObject();
    delete pTree;
    delete pMashroom;
    delete pGround;
    delete pWall;
    delete pShield;
    delete pBomb;
    for (int i = 0; i < 5; i++)
    {
        pBombing[i]->DeleteObject();
        delete pBombing[i];
    }
    delete mdc;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pPlayerPic[i][j]->DeleteObject();
            delete pPlayerPic[i][j];
        }
    }
}


BEGIN_MESSAGE_MAP(CBombItWnd, CFrameWnd)
    ON_WM_PAINT()
    ON_WM_KEYDOWN()
    ON_COMMAND(ID_ABOUT, &CBombItWnd::OnAbout)
    ON_COMMAND(ID_QUIT, &CBombItWnd::OnQuit)
    ON_COMMAND(ID_RESTART, &CBombItWnd::OnRestart)
END_MESSAGE_MAP()


// CBombItWnd 消息处理程序


void CBombItWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    statistic = 0;
    if (died)
    {
        initMap();
        dir = 0;
        PosX = 0;
        PosY = 0;
        died = 0;
    }
    int tmp, seed;

    int tmpI, tmpJ;
    bool flag = false;

    seed = (int)time(0);
    srand((unsigned int)seed);

    for (int i = 0; i < ROW_NUM; i++)
    {
        for (int j = 0; j < COLUMN_NUM; j++)
        {
            
            switch (map[i][j])
            {
            case 0:
                drawGround(pGround, mdc, dc, j * 60, i * 60);
                break;
            case -1:
            {
                tmp = rand() % 2;
                switch (tmp)
                {
                case 0:
                {map[i][j] = 2;
                drawMashroom(pMashroom, mdc, dc, j * 60, i * 60);
                break; }
                case 1:
                {map[i][j] = 3;
                drawTree(pTree, mdc, dc, j * 60, i * 60);
                break; }
                }
            }
            statistic++;
                break;
            case 1:
                drawWall(pWall, mdc, dc, j * 60, i * 60);
                break;
            case 2:
                drawMashroom(pMashroom, mdc, dc, j * 60, i * 60);
                statistic++;
                break;
            case 3:
                drawTree(pTree, mdc, dc, j * 60, i * 60);
                statistic++;
                break;
            case 4:
                drawSheild(pShield, mdc, dc, j * 60, i * 60);
                break;
            case 5:
                drawPlayer(pPlayerPic, mdc, dc, j * 60, i * 60, dir);
                break;
            case 6:
                drawPlayer(pPlayerPic, mdc, dc, j * 60, i * 60, dir, 1);
                break;
            case 7:
                drawBomb(pBomb, mdc, dc, j * 60, i * 60);
                if (stepNum >= 1)
                {
                    map[i][j] = 8;
                }
                break;
            case 8:
                tmpI = i;
                tmpJ = j;
                flag = true;
                break;
            default:
                break;
            }
            
            fOut << map[i][j] << ' ';
        }
        fOut << std::endl;
    }
    if (flag)
    {
        drawBombing(this,this->GetSafeHwnd(), pBombing, mdc, dc, tmpJ * 60, tmpI * 60);
        BombNum++;
        stepNum = -1;
    }
    fOut << statistic;
    if (count == 0 && statistic == 0)
    {
        MessageBox(_T("资源加载失败..."), _T("游戏提示"));
        ASSERT(this != NULL);
        this->SendMessage(WM_CLOSE);
    }
    if (count && !statistic)
    {
        SetDied(1);
        CDeclarreWinDlg WinDlg;
        WinDlg.DoModal();
        Invalidate();
    }
    fOut << "--------------"<<count<<"----------------" << std::endl;
    count++;
}

void initMap()
{
    char fileName[10];
    int num, seed;
    seed = (int)time(0);
    srand((unsigned int)seed);
    num = rand() % 3 + 1;
    sprintf_s(fileName, "map%d.txt", num);
    std::ifstream fInput(fileName, std::ios_base::in);
    for (int i = 0; i < ROW_NUM; i++)
    {
        for (int j = 0; j < COLUMN_NUM; j++)
        {
            fInput >> map[i][j];
        }
    }
    fInput.close();
}

void drawGround(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pBitmap);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}

void drawTree(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pBitmap);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}

void drawMashroom(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pBitmap);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}

void drawWall(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pBitmap);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}

void drawBomb(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pBitmap);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);

}

void drawSheild(CBitmap* pBitmap, CDC* mdc, CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pBitmap);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}

void drawBombing(CBombItWnd* Wnd,HWND hWnd, CBitmap* pBitmap[], CDC* mdc, CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pBitmap[0]);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
    int tmpI = PosY / 60;
    int tmpJ = PosX / 60;
    if (map[tmpI - 1][tmpJ] == 0)
    {
        mdc->SelectObject(pBitmap[1]);
        dc.BitBlt(PosX, PosY - 60, 60, 60, mdc, 0, 0, SRCCOPY);
    }
    if (map[tmpI + 1][tmpJ] == 0)
    {
        mdc->SelectObject(pBitmap[2]);
        dc.BitBlt(PosX, PosY + 60, 60, 60, mdc, 0, 0, SRCCOPY);

    }
    if (map[tmpI][tmpJ - 1] == 0)
    {
        mdc->SelectObject(pBitmap[3]);
        dc.BitBlt(PosX - 60, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
    }
    if (map[tmpI][tmpJ + 1] == 0)
    {
        mdc->SelectObject(pBitmap[4]);
        dc.BitBlt(PosX + 60, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
    }
    InvalidateRect(hWnd, CRect(PosX > 0 ? PosX - 60 : PosX, PosY > 0 ? PosY - 60 : PosY, PosX < 840 ? PosX + 120 : PosX + 60, PosY < 840 ? PosY + 120 : PosY + 60), true);
    Sleep(200);
    if (map[tmpI][tmpJ] == 8)
    {
        map[tmpI][tmpJ] = 0;
        if (tmpI > 0 && map[tmpI - 1][tmpJ] != 1)
        {
            if (map[tmpI - 1][tmpJ] == 5)
            {
                Wnd->SetDied(1);
                CDeclareDeathDlg DeathDlg;
                DeathDlg.DoModal();
                Wnd->Invalidate();
                return;
            }
            map[tmpI - 1][tmpJ] = 0;
            //Wnd->minusOneObject();
        }

        if (tmpI < ROW_NUM - 1 && map[tmpI + 1][tmpJ] != 1)
        {
            if (map[tmpI + 1][tmpJ] == 5)
            {
                Wnd->SetDied(1);
                CDeclareDeathDlg DeathDlg;
                DeathDlg.DoModal();
                Wnd->Invalidate();
                return;
            }
            map[tmpI + 1][tmpJ] = 0;
            //Wnd->minusOneObject();
        }
        if (tmpJ > 0 && map[tmpI][tmpJ - 1] != 1)
        {
            if (map[tmpI][tmpJ - 1] == 5)
            {
                Wnd->SetDied(1);
                CDeclareDeathDlg DeathDlg;
                DeathDlg.DoModal();
                Wnd->Invalidate();
                return;
            }
            map[tmpI][tmpJ - 1] = 0;
            //Wnd->minusOneObject();
        }
        if (tmpJ < COLUMN_NUM - 1 && map[tmpI][tmpJ + 1] != 1)
        {
            if (map[tmpI][tmpJ + 1] == 5)
            {
                Wnd->SetDied(1);
                CDeclareDeathDlg DeathDlg;
                DeathDlg.DoModal();
                Wnd->Invalidate();
                return;
            }
            map[tmpI][tmpJ + 1] = 0;
            //Wnd->minusOneObject();
        }
    }
    
    //InvalidateRect(hWnd, CRect(PosX > 0 ? PosX - 60 : PosX, PosY > 0 ? PosY - 60 : PosY, PosX < 840 ? PosX + 120 : PosX, PosY < 840 ? PosY + 120 : PosY), true);
}

void drawPlayer(CBitmap* pPlayerPic[][4], CDC* mdc, CPaintDC& dc, int PosX, int PosY, int dir, int WithBomb)
{
    mdc->SelectObject(pPlayerPic[WithBomb][dir]);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}


void CBombItWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    int tmpX, tmpY;
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    switch (nChar)
    {
    case VK_DOWN:
        dir = 0;
        tmpX = PosX;
        tmpY = PosY;
        if (PosY < ROW_NUM - 1 && map[PosY][PosX] == 5 && map[PosY + 1][PosX] == 0)
        {
            map[PosY][PosX] = 0;
            map[PosY + 1][PosX] = 5;
            PosY++;
        }
        if (PosY < ROW_NUM - 1 && map[PosY][PosX] == 6 && map[PosY + 1][PosX] == 0)
        {
            map[PosY][PosX] = 7;
            map[PosY + 1][PosX] = 5;
            PosY++;
        }
        if (stepNum >= 0)
        {
            stepNum++;
        }

        InvalidateRect(CRect((tmpX > 1 ? tmpX - 2 : (tmpX > 0 ? tmpX - 1 : tmpX)) * 60, (tmpY > 1 ? tmpY - 2 : (tmpY > 0 ? tmpY - 1 : tmpY)) * 60, (tmpX < 13 ? tmpX + 3 : (tmpX < 14 ? tmpX + 2 : (tmpX < 15 ? tmpX + 1 : tmpX))) * 60, (tmpY < 13 ? tmpY + 3 : (tmpY < 14 ? tmpY + 2 : (tmpY < 15 ? tmpY + 1 : tmpY))) * 60), true);
        break;
    case VK_UP: 
        dir = 1;
        tmpX = PosX;
        tmpY = PosY;
        if (PosY > 0 && map[PosY][PosX] == 5 && map[PosY - 1][PosX] == 0)
        {
            map[PosY][PosX] = 0;
            map[PosY -1][PosX] = 5;
            PosY--;
        }
        if (PosY > 0 && map[PosY][PosX] == 6 && map[PosY - 1][PosX] == 0)
        {
            map[PosY][PosX] = 7;
            map[PosY - 1][PosX] = 5;
            PosY--;
        }
        if (stepNum >= 0)
        {
            stepNum++;
        }
        InvalidateRect(CRect((tmpX > 1 ? tmpX - 2 : (tmpX > 0 ? tmpX - 1 : tmpX)) * 60, (tmpY > 1 ? tmpY - 2 : (tmpY > 0 ? tmpY - 1 : tmpY)) * 60, (tmpX < 13 ? tmpX + 3 : (tmpX < 14 ? tmpX + 2 : (tmpX < 15 ? tmpX + 1 : tmpX))) * 60, (tmpY < 13 ? tmpY + 3 : (tmpY < 14 ? tmpY + 2 : (tmpY < 15 ? tmpY + 1 : tmpY))) * 60), true);
        break;
    case VK_LEFT:
        dir = 2;
        tmpX = PosX;
        tmpY = PosY;
        if (PosX > 0 && map[PosY][PosX] == 5 && map[PosY][PosX - 1] == 0)
        {
            map[PosY][PosX] = 0;
            map[PosY][PosX - 1] = 5;
            PosX--;
        }
        if (PosX > 0 && map[PosY][PosX] == 6 && map[PosY][PosX - 1] == 0)
        {
            map[PosY][PosX] = 7;
            map[PosY][PosX - 1] = 5;
            PosX--;
        }
        if (stepNum >= 0)
        {
            stepNum++;
        }
        InvalidateRect(CRect((tmpX > 1 ? tmpX - 2 : (tmpX > 0 ? tmpX - 1 : tmpX)) * 60, (tmpY > 1 ? tmpY - 2 : (tmpY > 0 ? tmpY - 1 : tmpY)) * 60, (tmpX < 13 ? tmpX + 3 : (tmpX < 14 ? tmpX + 2 : (tmpX < 15 ? tmpX + 1 : tmpX))) * 60, (tmpY < 13 ? tmpY + 3 : (tmpY < 14 ? tmpY + 2 : (tmpY < 15 ? tmpY + 1 : tmpY))) * 60), true);
        break;
    case VK_RIGHT:
        dir = 3;
        tmpX = PosX;
        tmpY = PosY;
        if (PosX < COLUMN_NUM - 1 && map[PosY][PosX] == 5 && map[PosY][PosX + 1] == 0)
        {
            map[PosY][PosX] = 0;
            map[PosY][PosX + 1] = 5;
            PosX++;
        }
        if (PosX < COLUMN_NUM - 1 && map[PosY][PosX] == 6 && map[PosY][PosX + 1] == 0)
        {
            map[PosY][PosX] = 7;
            map[PosY][PosX + 1] = 5;
            PosX++;
        }
        if (stepNum >= 0)
        {
            stepNum++;
        }
        InvalidateRect(CRect((tmpX > 1 ? tmpX - 2 : (tmpX > 0 ? tmpX - 1 : tmpX)) * 60, (tmpY > 1 ? tmpY - 2 : (tmpY > 0 ? tmpY - 1 : tmpY)) * 60, (tmpX < 13 ? tmpX + 3 : (tmpX < 14 ? tmpX + 2 : (tmpX < 15 ? tmpX + 1 : tmpX))) * 60, (tmpY < 13 ? tmpY + 3 : (tmpY < 14 ? tmpY + 2 : (tmpY < 15 ? tmpY + 1 : tmpY))) * 60), true);
        break;
    case VK_SPACE:
        if (BombNum)
        {
            map[PosY][PosX] = 6;
            BombNum--;
            stepNum = 0;
            InvalidateRect(CRect(PosX * 60, PosY * 60, (PosX + 1) * 60, (PosY + 1) * 60), true);
        }
        break;
    default:
        break;
    }
    CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBombItWnd::SetDied(int N)
{
    died = N;
}

int CBombItWnd::getStatistic()
{
    return statistic;
}

void CBombItWnd::minusOneObject()
{
    statistic--;
}


void CBombItWnd::OnAbout()
{
    AboutBoxDlg ABtDlg;
    ABtDlg.DoModal();
    // MessageBox(_T("本小游戏归开发者版权所有，仿冒必究", ), _T("关于"));
    // TODO: 在此添加命令处理程序代码
}


void CBombItWnd::OnQuit()
{
    ASSERT(this != NULL);
    this->SendMessage(WM_CLOSE);
    // TODO: 在此添加命令处理程序代码
}



void CBombItWnd::OnRestart()
{
    initMap();
    dir = 0;
    PosX = 0;
    PosY = 0;
    died = 0;
    Invalidate();
    // TODO: 在此添加命令处理程序代码
}
