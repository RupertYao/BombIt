// CBombItWnd.cpp: 实现文件
//

#define _WIN32_WINNT _WIN32_WINNT_WIN10

#include "game.h"
#include "CBombItWnd.h"
#include "CDeclarreWinDlg.h"
#include "CDeclareDeathDlg.h"
#include "AboutBoxDlg.h"
#include "resource.h"
#include "Map.h"

#include <cstdlib>
#include <ctime>
#include <fstream>

// CBombItWnd

IMPLEMENT_DYNCREATE(CBombItWnd, CFrameWnd)

CBombItWnd::CBombItWnd()
{
    this->Create(NULL, _T("BombIt"));
    this->SetWindowTextW(_T("简化Q版泡泡堂"));
    this->SetWindowPos(NULL, 100, 50, 60*COLUMN_NUM +20, 60*ROW_NUM +75, 0);

    menu.LoadMenuW(IDR_MENU1);
    this->SetMenu(&menu);

    bgm = new BGM;
    bgm->playMusic();
    
    p_mMap = new Map(this);
    fOut = std::ofstream("temp.txt", std::ios_base::out);
}

CBombItWnd::~CBombItWnd()
{
    fOut.close();
    delete p_mMap;
    delete bgm;
}


BEGIN_MESSAGE_MAP(CBombItWnd, CFrameWnd)
    ON_WM_PAINT()
    ON_WM_KEYDOWN()
    ON_COMMAND(ID_ABOUT, &CBombItWnd::OnAbout)
    ON_COMMAND(ID_QUIT, &CBombItWnd::OnQuit)
    ON_COMMAND(ID_RESTART, &CBombItWnd::OnRestart)
    ON_WM_TIMER()
    ON_COMMAND(ID_ABOUT_GAME, &CBombItWnd::OnAboutGame)
    ON_COMMAND(ID_CHANGE_MAP, &CBombItWnd::OnChangemap)
    ON_COMMAND(ID_PREVIOUS_SONG, &CBombItWnd::OnPrevioussong)
    ON_COMMAND(ID_NEXT_SONG, &CBombItWnd::OnNextsong)
    ON_COMMAND(ID_NOBGM, &CBombItWnd::OnNobgm)
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
END_MESSAGE_MAP()


// CBombItWnd 消息处理程序


void CBombItWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    statistic = 0;
    if (died)
    {
        p_mMap->initMap();
        BombNum = 1;
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
            
            switch (p_mMap->map[i][j])
            {
            case 0:
                p_mMap->drawGround(dc, j * 60, i * 60);
                break;
            case -1:
            {
                tmp = rand() % 2;
                switch (tmp)
                {
                case 0:
                {p_mMap->map[i][j] = 2;
                p_mMap->drawMashroom(dc, j * 60, i * 60);
                break; }
                case 1:
                {p_mMap->map[i][j] = 3;
                p_mMap->drawTree(dc, j * 60, i * 60);
                break; }
                }
            }
            statistic++;
                break;
            case 1:
                p_mMap->drawWall(dc, j * 60, i * 60);
                break;
            case 2:
                p_mMap->drawMashroom(dc, j * 60, i * 60);
                statistic++;
                break;
            case 3:
                p_mMap->drawTree(dc, j * 60, i * 60);
                statistic++;
                break;
            case 4:
                p_mMap->drawShield(dc, j * 60, i * 60);
                break;
            case 5:
                p_mMap->drawPlayer(dc, j * 60, i * 60, dir);
                break;
            case 6:
                p_mMap->drawPlayer(dc, j * 60, i * 60, dir, 1);
                break;
            case 7:
                p_mMap->drawBomb(dc, j * 60, i * 60);
                break;
            case 8:
                tmpI = i;
                tmpJ = j;
                flag = true;
                break;
            default:
                break;
            }
            
            fOut << p_mMap->map[i][j] << ' ';
        }
        fOut << std::endl;
    }
    if (flag)
    {
        p_mMap->drawBombing(this, dc, tmpJ * 60, tmpI * 60);
        BombNum++;
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

void CBombItWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    int tmpX, tmpY;
    switch (nChar)
    {
    case VK_DOWN:
        dir = 0;
        tmpX = PosX;
        tmpY = PosY;
        if (PosY < ROW_NUM - 1 && p_mMap->map[PosY][PosX] == 5 && p_mMap->map[PosY + 1][PosX] == 0)
        {
            p_mMap->map[PosY][PosX] = 0;
            p_mMap->map[PosY + 1][PosX] = 5;
            PosY++;
        }
        if (PosY < ROW_NUM - 1 && p_mMap->map[PosY][PosX] == 6 && p_mMap->map[PosY + 1][PosX] == 0)
        {
            p_mMap->map[PosY][PosX] = 7;
            p_mMap->map[PosY + 1][PosX] = 5;
            PosY++;
        }

        InvalidateRect(CRect((tmpX > 1 ? tmpX - 2 : (tmpX > 0 ? tmpX - 1 : tmpX)) * 60, (tmpY > 1 ? tmpY - 2 : (tmpY > 0 ? tmpY - 1 : tmpY)) * 60, (tmpX < 13 ? tmpX + 3 : (tmpX < 14 ? tmpX + 2 : (tmpX < 15 ? tmpX + 1 : tmpX))) * 60, (tmpY < 13 ? tmpY + 3 : (tmpY < 14 ? tmpY + 2 : (tmpY < 15 ? tmpY + 1 : tmpY))) * 60), true);
        break;
    case VK_UP: 
        dir = 1;
        tmpX = PosX;
        tmpY = PosY;
        if (PosY > 0 && p_mMap->map[PosY][PosX] == 5 && p_mMap->map[PosY - 1][PosX] == 0)
        {
            p_mMap->map[PosY][PosX] = 0;
            p_mMap->map[PosY -1][PosX] = 5;
            PosY--;
        }
        if (PosY > 0 && p_mMap->map[PosY][PosX] == 6 && p_mMap->map[PosY - 1][PosX] == 0)
        {
            p_mMap->map[PosY][PosX] = 7;
            p_mMap->map[PosY - 1][PosX] = 5;
            PosY--;
        }
        InvalidateRect(CRect((tmpX > 1 ? tmpX - 2 : (tmpX > 0 ? tmpX - 1 : tmpX)) * 60, (tmpY > 1 ? tmpY - 2 : (tmpY > 0 ? tmpY - 1 : tmpY)) * 60, (tmpX < 13 ? tmpX + 3 : (tmpX < 14 ? tmpX + 2 : (tmpX < 15 ? tmpX + 1 : tmpX))) * 60, (tmpY < 13 ? tmpY + 3 : (tmpY < 14 ? tmpY + 2 : (tmpY < 15 ? tmpY + 1 : tmpY))) * 60), true);
        break;
    case VK_LEFT:
        dir = 2;
        tmpX = PosX;
        tmpY = PosY;
        if (PosX > 0 && p_mMap->map[PosY][PosX] == 5 && p_mMap->map[PosY][PosX - 1] == 0)
        {
            p_mMap->map[PosY][PosX] = 0;
            p_mMap->map[PosY][PosX - 1] = 5;
            PosX--;
        }
        if (PosX > 0 && p_mMap->map[PosY][PosX] == 6 && p_mMap->map[PosY][PosX - 1] == 0)
        {
            p_mMap->map[PosY][PosX] = 7;
            p_mMap->map[PosY][PosX - 1] = 5;
            PosX--;
        }
        InvalidateRect(CRect((tmpX > 1 ? tmpX - 2 : (tmpX > 0 ? tmpX - 1 : tmpX)) * 60, (tmpY > 1 ? tmpY - 2 : (tmpY > 0 ? tmpY - 1 : tmpY)) * 60, (tmpX < 13 ? tmpX + 3 : (tmpX < 14 ? tmpX + 2 : (tmpX < 15 ? tmpX + 1 : tmpX))) * 60, (tmpY < 13 ? tmpY + 3 : (tmpY < 14 ? tmpY + 2 : (tmpY < 15 ? tmpY + 1 : tmpY))) * 60), true);
        break;
    case VK_RIGHT:
        dir = 3;
        tmpX = PosX;
        tmpY = PosY;
        if (PosX < COLUMN_NUM - 1 && p_mMap->map[PosY][PosX] == 5 && p_mMap->map[PosY][PosX + 1] == 0)
        {
            p_mMap->map[PosY][PosX] = 0;
            p_mMap->map[PosY][PosX + 1] = 5;
            PosX++;
        }
        if (PosX < COLUMN_NUM - 1 && p_mMap->map[PosY][PosX] == 6 && p_mMap->map[PosY][PosX + 1] == 0)
        {
            p_mMap->map[PosY][PosX] = 7;
            p_mMap->map[PosY][PosX + 1] = 5;
            PosX++;
        }
        InvalidateRect(CRect((tmpX > 1 ? tmpX - 2 : (tmpX > 0 ? tmpX - 1 : tmpX)) * 60, (tmpY > 1 ? tmpY - 2 : (tmpY > 0 ? tmpY - 1 : tmpY)) * 60, (tmpX < 13 ? tmpX + 3 : (tmpX < 14 ? tmpX + 2 : (tmpX < 15 ? tmpX + 1 : tmpX))) * 60, (tmpY < 13 ? tmpY + 3 : (tmpY < 14 ? tmpY + 2 : (tmpY < 15 ? tmpY + 1 : tmpY))) * 60), true);
        break;
    case VK_SPACE:
        if (BombNum)
        {
            p_mMap->map[PosY][PosX] = 6;
            BombNum--;
            BombPosX = PosX;
            BombPosY = PosY;
            SetTimer(1, BOMB_TIME, NULL);
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
}


void CBombItWnd::OnQuit()
{
    ASSERT(this != NULL);
    this->SendMessage(WM_CLOSE);
}



void CBombItWnd::OnRestart()
{
    p_mMap->initMap();
    dir = 0;
    PosX = 0;
    PosY = 0;
    died = 0;
    Invalidate();
}
void CBombItWnd::OnAboutGame()
{
    MessageBox(_T("玩家用方向键上下左右控制人物的移动，空格键放下炸弹"), _T("游戏玩法说明"));
}

void CBombItWnd::OnTimer(UINT_PTR nIDEvent)
{
    int flag = 0;
    if (p_mMap->map[BombPosY][BombPosX] == 6)
    {
        flag = 1;
    }
    p_mMap->map[BombPosY][BombPosX] = 8;
    Invalidate();
    BombPosX = -1;
    BombPosY = -1;
    if (flag)
    {
        died = 1;
        CDeclareDeathDlg DeathDlg;
        DeathDlg.DoModal();
        Invalidate();
        KillTimer(1);
        CFrameWnd::OnTimer(nIDEvent);
        return;
    }
    CFrameWnd::OnTimer(nIDEvent);
}

void CBombItWnd::OnChangemap()
{
    p_mMap->MapNum = (p_mMap->MapNum + 1) % 3+1;
    BombNum = 1;
    dir = 0;
    PosX = 0;
    PosY = 0;
    died = 0;
    p_mMap->initMap();
    Invalidate();
}

void CBombItWnd::OnPrevioussong()
{
    bgm->previousMusic();
}


void CBombItWnd::OnNextsong()
{
    bgm->nextMusic();
}


void CBombItWnd::OnNobgm()
{
    bgm->stopMusic();
}


BOOL CBombItWnd::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}


int CBombItWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    HICON m_hIcon;
    m_hIcon = AfxGetApp()->LoadIconW(IDI_ICON1);
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    return 0;
}


BOOL CBombItWnd::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style = cs.style & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
    return CFrameWnd::PreCreateWindow(cs);
}
