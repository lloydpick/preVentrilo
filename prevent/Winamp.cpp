#include "StdAfx.h"
#include "Winamp.h"
#include "defs.h"

/**** WINAMP COMMANDS ****/
#define WM_WA_IPC WM_USER
#define WINAMP_PLAY		40045
#define WINAMP_PAUSE	40046
#define IPC_SETVOLUME 122
/* (requires Winamp 2.0+)
** SendMessage(hwnd_winamp,WM_WA_IPC,volume,IPC_SETVOLUME);
** IPC_SETVOLUME sets the volume of Winamp (between the range of 0 to 255).
**
** If you pass 'volume' as -666 then the message will return the current volume.
** int curvol = SendMessage(hwnd_winamp,WM_WA_IPC,-666,IPC_SETVOLUME);
*/
#define IPC_GETVOLUME(hwnd_winamp) SendMessage(hwnd_winamp,WM_WA_IPC,-666,IPC_SETVOLUME)
/* (requires Winamp 2.0+)
** int curvol = IPC_GETVOLUME(hwnd_winamp);
** This will return the current volume of Winamp or 
*/
#define IPC_ISPLAYING 104
/* int res = SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_ISPLAYING);
** This is sent to retrieve the current playback state of Winamp.
** If it returns 1, Winamp is playing.
** If it returns 3, Winamp is paused.
** If it returns 0, Winamp is not playing.
*/

CWinamp::CWinamp(void)
{
    m_nType = MEDIA_PLAYER_WINAMP;
}

CWinamp::~CWinamp(void)
{
}

bool CWinamp::IsPlaying()
{
	if(m_hwndMediaPlayer)
		return (SendMessage(m_hwndMediaPlayer, WM_WA_IPC, 0, IPC_ISPLAYING) == 1);
	else
		return false;
}

bool CWinamp::IsPaused()
{
	return true;
}

int CWinamp::CurrentVolume()
{
    if(m_hwndMediaPlayer)
    {
	    return IPC_GETVOLUME(m_hwndMediaPlayer);
    }
    else
        return 0;
}

void CWinamp::SetVolume(int nVolume)
{
    if(nVolume > 255)
        nVolume = 255;
    if(nVolume < 0)
        nVolume = 0;

    if(m_hwndMediaPlayer)
    {
	    PostMessage(m_hwndMediaPlayer, WM_WA_IPC, nVolume, IPC_SETVOLUME);
    }
}

void CWinamp::Pause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_COMMAND, WINAMP_PAUSE, NULL);
}

void CWinamp::UnPause()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_COMMAND, WINAMP_PAUSE, NULL);
}

void CWinamp::Play()
{
    if(m_hwndMediaPlayer)
	    PostMessage(m_hwndMediaPlayer, WM_COMMAND, WINAMP_PLAY, NULL);
}

void CWinamp::FindHandle()
{
	m_hwndMediaPlayer = FindWindow(TEXT("Winamp v1.x"), NULL);

	if(m_hwndMediaPlayer == NULL)
		m_hwndMediaPlayer = FindWindow(TEXT("Winamp v3.x"), NULL);
}