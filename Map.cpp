#include "Map.h"
#include "CDeclareDeathDlg.h"

#include <fstream>

Map::Map(CFrameWnd* pWnd)
{
    mdc = new CDC;
    CClientDC dc(pWnd);
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
}

Map::~Map()
{
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

void Map::initMap()
{
    char fileName[10];
    int seed;
    seed = (int)time(0);
    srand((unsigned int)seed);
    if (GameSum == 0)
        MapNum = rand() % 3 + 1;
    GameSum++;
    sprintf_s(fileName, "map%d.txt", MapNum);
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

void Map::drawGround(CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pGround);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}

void Map::drawTree(CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pTree);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}

void Map::drawMashroom(CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pMashroom);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}

void Map::drawWall(CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pWall);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}

void Map:: drawBomb(CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pBomb);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);

}

void Map::drawShield(CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pShield);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}

void Map::drawBombing(CBombItWnd* Wnd, CPaintDC& dc, int PosX, int PosY)
{
    mdc->SelectObject(pBombing[0]);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
    int tmpI = PosY / 60;
    int tmpJ = PosX / 60;
    if (map[tmpI - 1][tmpJ] == 0)
    {
        mdc->SelectObject(pBombing[1]);
        dc.BitBlt(PosX, PosY - 60, 60, 60, mdc, 0, 0, SRCCOPY);
    }
    if (map[tmpI + 1][tmpJ] == 0)
    {
        mdc->SelectObject(pBombing[2]);
        dc.BitBlt(PosX, PosY + 60, 60, 60, mdc, 0, 0, SRCCOPY);
    }
    if (map[tmpI][tmpJ - 1] == 0)
    {
        mdc->SelectObject(pBombing[3]);
        dc.BitBlt(PosX - 60, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
    }
    if (map[tmpI][tmpJ + 1] == 0)
    {
        mdc->SelectObject(pBombing[4]);
        dc.BitBlt(PosX + 60, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
    }
    InvalidateRect(Wnd->GetSafeHwnd(), CRect(PosX > 0 ? PosX - 60 : PosX, PosY > 0 ? PosY - 60 : PosY, PosX < 840 ? PosX + 120 : PosX + 60, PosY < 840 ? PosY + 120 : PosY + 60), true);
    Wnd->KillTimer(1);
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
                //Wnd->KillTimer(1);
                map[tmpI - 1][tmpJ] = 0;
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
                //Wnd->KillTimer(1);
                map[tmpI + 1][tmpJ] = 0;
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
                //Wnd->KillTimer(1);
                map[tmpI][tmpJ - 1] = 0;
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
                //Wnd->KillTimer(1);
                map[tmpI][tmpJ + 1] = 0;
                return;
            }
            map[tmpI][tmpJ + 1] = 0;
            //Wnd->minusOneObject();
        }
    }
    //InvalidateRect(hWnd, CRect(PosX > 0 ? PosX - 60 : PosX, PosY > 0 ? PosY - 60 : PosY, PosX < 840 ? PosX + 120 : PosX, PosY < 840 ? PosY + 120 : PosY), true);
}

void Map::drawPlayer(CPaintDC& dc, int PosX, int PosY, int dir, int WithBomb)
{
    mdc->SelectObject(pPlayerPic[WithBomb][dir]);
    dc.BitBlt(PosX, PosY, 60, 60, mdc, 0, 0, SRCCOPY);
}
