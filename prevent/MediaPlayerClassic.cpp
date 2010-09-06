#include "StdAfx.h"
#include "MediaPlayerClassic.h"
#include "defs.h"

CMediaPlayerClassic::CMediaPlayerClassic(void)
{
    m_nType = MEDIA_PLAYER_MEDIA_PLAYER_CLASSIC;
}

CMediaPlayerClassic::~CMediaPlayerClassic(void)
{
}

bool CMediaPlayerClassic::IsPlaying()
{
    return true;
}

bool CMediaPlayerClassic::IsPaused()
{
	return true;
}

int CMediaPlayerClassic::CurrentVolume()
{
    return 0;
}

void CMediaPlayerClassic::SetVolume(int nVolume)
{
	if(nVolume > 0) {
		if(m_hwndMediaPlayer) {
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_DOWN << 16);
		}
	}
	if(nVolume == 0) {
		if(m_hwndMediaPlayer) {
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
			PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_VOLUME_UP << 16);
		}
	}
}

void CMediaPlayerClassic::Pause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_MEDIA_PLAY_PAUSE << 16);
}

void CMediaPlayerClassic::UnPause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_MEDIA_PLAY_PAUSE << 16);
}

void CMediaPlayerClassic::Play()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_MEDIA_PLAY << 16);
}

void CMediaPlayerClassic::FindHandle()
{
	m_hwndMediaPlayer = FindWindow(TEXT("MediaPlayerClassicW"), NULL);
}