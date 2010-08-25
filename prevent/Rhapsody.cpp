#include "StdAfx.h"
#include "Rhapsody.h"
#include "defs.h"

CRhapsody::CRhapsody(void)
{
    m_nType = MEDIA_PLAYER_RHAPSODY;
}

CRhapsody::~CRhapsody(void)
{
}

bool CRhapsody::IsPlaying()
{
    return true;
}

bool CRhapsody::IsPaused()
{
	return true;
}

int CRhapsody::CurrentVolume()
{
    return 0;
}

void CRhapsody::SetVolume(int nVolume)
{
}

void CRhapsody::Pause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY_PAUSE * 65536);
}

void CRhapsody::UnPause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY_PAUSE * 65536);
}

void CRhapsody::Play()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY * 65536);
}

void CRhapsody::FindHandle()
{
	m_hwndMediaPlayer = FindWindow(TEXT("TEST_WIN32WND"), TEXT("Rhapsody"));
}