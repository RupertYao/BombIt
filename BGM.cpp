#include "game.h"
#include "BGM.h"

BGM::BGM()
{
	maxMusicNumber = 5;
	musicNumer = 0;
}

BGM::~BGM()
{
    closeMusic();
}

void BGM::playMusic()
{
    WCHAR songOpen[32];
    WCHAR songPlay[32];
    swprintf_s(songOpen, _T("open Resource\\bgm%d.mp3"), musicNumer);
    swprintf_s(songPlay, _T("play Resource\\bgm%d.mp3 repeat"), musicNumer);
    mciSendString(songOpen, NULL, 0, NULL);
    mciSendString(songPlay, NULL, 0, NULL);
}

void BGM::nextMusic()
{
    stopMusic();
    closeMusic();
    musicNumer = (musicNumer + 1) % maxMusicNumber;
    playMusic();
}

void BGM::previousMusic()
{
    stopMusic();
    closeMusic();
    musicNumer = (musicNumer + maxMusicNumber - 1) % maxMusicNumber;
    playMusic();
}

void BGM::stopMusic()
{
    WCHAR songStop[32];
    swprintf_s(songStop, _T("stop Resource\\bgm%d.mp3"), musicNumer);
    mciSendString(songStop, NULL, 0, NULL);
}

void BGM::closeMusic()
{
    WCHAR songClose[32];
    swprintf_s(songClose, _T("close Resource\\bgm%d.mp3"), musicNumer);
    mciSendString(songClose, NULL, 0, NULL);
}


