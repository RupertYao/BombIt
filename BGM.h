#pragma once

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

class BGM
{
public:
	BGM();
	~BGM();
	void playMusic();
	void nextMusic();
	void previousMusic();
	void stopMusic();
	void closeMusic();
private:
	int musicNumer; //���ڲ��ŵ����ֱ��
	int maxMusicNumber;
};

