#include "StdAfx.h"
#include "Foobar.h"
#include "defs.h"

/**** FOOBAR ****/
#define FOO_WINDOW_CLASS TEXT("{97E27FAA-C0B3-4b8e-A693-ED7881E99FC1}")
#define FB_PAUSE_WPARAM	0x00009C6C
#define FB_PAUSE_LPARAM	0x000C0528

CFoobar::CFoobar(void)
{
    m_nType = MEDIA_PLAYER_FOOBAR;
}

CFoobar::~CFoobar(void)
{
}

bool CFoobar::IsPlaying()
{
    return true;
}

bool CFoobar::IsPaused()
{
	return true;
}

int CFoobar::CurrentVolume()
{
    return 0;
}

void CFoobar::SetVolume(int nVolume)
{
    Pause();
}

void CFoobar::Pause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY_PAUSE * 65536);
}

void CFoobar::UnPause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY_PAUSE * 65536);
}

void CFoobar::Play()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_APPCOMMAND, NULL, APPCOMMAND_MEDIA_PLAY * 65536);
}

void CFoobar::FindHandle()
{
	m_hwndMediaPlayer = FindWindow(FOO_WINDOW_CLASS, NULL);
}