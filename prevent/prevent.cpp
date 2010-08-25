// prevent.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "defs.h"

#include "MediaPlayer.h"

void PauseSound();
void UnpauseSound();
void PostWMtoMP(bool bPause);
void SetWasMPPlaying();
DWORD WINAPI ThreadMPProc(LPVOID lpParam);

#pragma data_seg("SharedBlock")
HHOOK g_hhook = NULL;           // hook
int   g_pcount = 0;             // count
bool  g_paused = false;         // whether or not the media player is currently paused
DWORD g_lastMsgTick = 0;        // last message tick
HWND  g_hwndXmit = NULL;        // handle to xmit
int   g_mediaPlayer = 0;        // media player type
int   g_typeOfMute = (PAUSE_TYPE_OTHERS | PAUSE_TYPE_SELF);  // type of pause
bool  g_bMPWasPlaying = true;   // whether or not the media player was playing
int   g_nCurrVol = 255;         // the volume before changing it
bool  g_bAdjustVolume = true;   // whether or not to adjust the volume rather than pausing
int   g_nAdjustedVol = 25;      // what to change the volume to
#pragma data_seg()

#pragma comment(linker, "/section:SharedBlock,RWS")

HINSTANCE g_hInstance = NULL;
CMediaPlayer* g_pMediaPlayer = NULL;

LRESULT CALLBACK VentHookProc(int Code, WPARAM wParam, LPARAM lParam)
{
	if (Code < 0) return(CallNextHookEx(g_hhook, Code, wParam, lParam));

	MSG *msg = (MSG *)lParam;

    if(g_pMediaPlayer == NULL)
        g_pMediaPlayer = new CMediaPlayer();

    if(g_pMediaPlayer->GetType() != g_mediaPlayer)
        g_pMediaPlayer->SetType(g_mediaPlayer);

	// look at the WS_VISIBLE status of the --XMIT-- text that shows up when talking
	// when receiving sound it continuously is only sent 0x03BD with wParam equal to 0.
	if(g_typeOfMute & PAUSE_TYPE_OTHERS)
	{
		if(msg->message == 0x03BD && msg->wParam == 0)
		{
			g_lastMsgTick = GetTickCount();
			if(!g_paused)
				PauseSound();
		}
		else if(g_paused && (GetTickCount() - g_lastMsgTick > 500) && (g_lastMsgTick != 0))
		{
			g_lastMsgTick = 0;
			UnpauseSound();
		}
	}
	if(g_typeOfMute & PAUSE_TYPE_SELF)
	{
		if(IsWindow(g_hwndXmit))
		{
			if(GetWindowLongPtr(g_hwndXmit, GWL_STYLE) & WS_VISIBLE)
			{
				g_lastMsgTick = GetTickCount();
				if(!g_paused)
					PauseSound();
			}
			else if(g_paused && (GetTickCount() - g_lastMsgTick > 500) && (g_lastMsgTick != 0))
			{
				g_lastMsgTick = 0;
				UnpauseSound();
			}
		}
	}

	return(CallNextHookEx(g_hhook, Code, wParam, lParam));
}

void PauseSound()
{
    if(!g_pMediaPlayer->HasHandle())
        g_pMediaPlayer->FindHandle();

    if(g_pMediaPlayer->HasHandle())
    {
        SetWasMPPlaying();
        if(g_bMPWasPlaying)
        {
	        PostWMtoMP(true);
	        g_paused = true;
        }
    }
}

void UnpauseSound()
{
    if(!g_pMediaPlayer->HasHandle())
        g_pMediaPlayer->FindHandle();

    if(g_pMediaPlayer->HasHandle())
    {
        if(g_bMPWasPlaying)
        {
	        PostWMtoMP(false);
	        g_paused = false;
        }
    }
}

void PostWMtoMP(bool bPause)
{
    if(g_bAdjustVolume)
    {
	    if(bPause)
	    {
		    g_nCurrVol = g_pMediaPlayer->CurrentVolume();
            g_pMediaPlayer->SetVolume(g_nAdjustedVol);
	    }
	    else
	    {
            g_pMediaPlayer->SetVolume(g_nCurrVol);
	    }
    }
    else
    {
        if(bPause)
            g_pMediaPlayer->Pause();
        else
            g_pMediaPlayer->UnPause();
    }
}

void SetWasMPPlaying()
{
    g_bMPWasPlaying = g_pMediaPlayer->IsPlaying();
}

extern "C" _declspec(dllexport) void SetVentHook(DWORD threadID, HWND hwndX)
{
	if (!g_hhook)
	{
		g_hhook = SetWindowsHookEx(WH_GETMESSAGE, VentHookProc, g_hInstance, threadID);
		g_hwndXmit = hwndX;
	}
}

extern "C" _declspec(dllexport) void KillVentHook()
{
	if(g_hhook) UnhookWindowsHookEx(g_hhook);
	g_hhook = NULL;
	g_hwndXmit = NULL;
}

extern "C" _declspec(dllexport) void SetMuteType(int type, bool bAdjustVolume, int nAdjustedVol)
{
	g_typeOfMute = type;
	g_bAdjustVolume = bAdjustVolume;
	g_nAdjustedVol = nAdjustedVol;
}

extern "C" _declspec(dllexport) void SetMediaPlayer(int mpID)
{
	g_mediaPlayer = mpID;
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if(g_pcount == 0)
		{
			// Init
			g_hInstance = hInstance;
		}
		g_pcount++;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		if(g_pcount == 1)
		{
		    // Free
		}

        if(g_pMediaPlayer)
        {
            delete g_pMediaPlayer;
            g_pMediaPlayer = NULL;
        }

		g_pcount--;
	}
	return(1);
}