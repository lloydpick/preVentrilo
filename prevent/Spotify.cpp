#include "StdAfx.h"
#include "Spotify.h"
#include "defs.h"

CSpotify::CSpotify(void)
{
    m_nType = MEDIA_PLAYER_SPOTIFY;
}

CSpotify::~CSpotify(void)
{
}

bool CSpotify::IsPlaying()
{
    return true;
}

bool CSpotify::IsPaused()
{
	return true;
}

int CSpotify::CurrentVolume()
{
    return 0;
}

void CSpotify::SetVolume(int nVolume)
{
}

void CSpotify::Pause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_MEDIA_PLAY_PAUSE << 16);
}

void CSpotify::UnPause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_MEDIA_PLAY_PAUSE << 16);
}

void CSpotify::Play()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_MEDIA_PLAY << 16);
}

void CSpotify::FindHandle()
{
	m_hwndMediaPlayer = FindWindow(TEXT("SpotifyMainWindow"), NULL);
}