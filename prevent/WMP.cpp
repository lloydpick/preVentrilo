#include "StdAfx.h"
#include "WMP.h"
#include "defs.h"

/**** WINDOWS MEDIA PLAYER ****/
#define WMP_PAUSE	0x00004978

CWMP::CWMP(void)
{
    m_nType = MEDIA_PLAYER_WMP;
}

CWMP::~CWMP(void)
{
}

bool CWMP::IsPlaying()
{
    return true;
}

bool CWMP::IsPaused()
{
	return true;
}

int CWMP::CurrentVolume()
{
    return 0;
}

void CWMP::SetVolume(int nVolume)
{
}

void CWMP::Pause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY_PAUSE * 65536);
}

void CWMP::UnPause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY_PAUSE * 65536);
}

void CWMP::Play()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY * 65536);
}

void CWMP::FindHandle()
{
	m_hwndMediaPlayer = FindWindow(TEXT("WMPlayerApp"), TEXT("Windows Media Player"));
}