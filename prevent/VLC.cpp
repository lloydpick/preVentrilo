#include "StdAfx.h"
#include "VLC.h"
#include "defs.h"

CVLC::CVLC(void)
{
    m_nType = MEDIA_PLAYER_VLC;
}

CVLC::~CVLC(void)
{
}

bool CVLC::IsPlaying()
{
    return true;
}

bool CVLC::IsPaused()
{
	return true;
}

int CVLC::CurrentVolume()
{
    return 0;
}

void CVLC::SetVolume(int nVolume)
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

void CVLC::Pause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_MEDIA_PLAY_PAUSE << 16);
}

void CVLC::UnPause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_MEDIA_PLAY_PAUSE << 16);
}

void CVLC::Play()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, 0x00000000, APPCOMMAND_MEDIA_PLAY << 16);
}

void CVLC::FindHandle()
{
	m_hwndMediaPlayer = FindWindow(NULL, TEXT("VLC media player"));
}