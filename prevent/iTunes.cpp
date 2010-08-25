#include "StdAfx.h"
#include "iTunes.h"
#include "defs.h"

CiTunes::CiTunes(void)
{
    m_nType = MEDIA_PLAYER_ITUNES;
}

CiTunes::~CiTunes(void)
{
}

bool CiTunes::IsPlaying()
{
    return true;
}

bool CiTunes::IsPaused()
{
	return true;
}

int CiTunes::CurrentVolume()
{
    return 0;
}

void CiTunes::SetVolume(int nVolume)
{
}

void CiTunes::Pause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY_PAUSE * 65536);
}

void CiTunes::UnPause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY_PAUSE * 65536);
}

void CiTunes::Play()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY * 65536);
}

void CiTunes::FindHandle()
{
	m_hwndMediaPlayer = FindWindow(TEXT("iTunes"), TEXT("iTunes"));
}