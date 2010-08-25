#include "StdAfx.h"
#include "MediaPlayer.h"

#include "defs.h"
#include "Winamp.h"
#include "Foobar.h"
#include "WMP.h"
#include "iTunes.h"
#include "Rhapsody.h"

CMediaPlayer::CMediaPlayer(void)
: m_pMediaPlayer(NULL)
{
}

CMediaPlayer::~CMediaPlayer(void)
{
}

bool CMediaPlayer::IsPlaying()
{
    if(!m_pMediaPlayer)
        return false;

    return m_pMediaPlayer->IsPlaying();
}

bool CMediaPlayer::IsPaused()
{
    if(!m_pMediaPlayer)
        return false;

    return m_pMediaPlayer->IsPaused();
}


int CMediaPlayer::CurrentVolume()
{
    if(!m_pMediaPlayer)
        return 0;

    return m_pMediaPlayer->CurrentVolume();
}


void CMediaPlayer::SetVolume(int nVolume)
{
    if(m_pMediaPlayer)
        m_pMediaPlayer->SetVolume(nVolume);
}


void CMediaPlayer::Pause()
{
    if(m_pMediaPlayer)
        m_pMediaPlayer->Pause();
}


void CMediaPlayer::UnPause()
{
    if(m_pMediaPlayer)
        m_pMediaPlayer->UnPause();
}


void CMediaPlayer::Play()
{
    if(m_pMediaPlayer)
        m_pMediaPlayer->Play();
}


void CMediaPlayer::FindHandle()
{
    if(m_pMediaPlayer)
        m_pMediaPlayer->FindHandle();
}


bool CMediaPlayer::HasHandle()
{
    if(!m_pMediaPlayer)
        return FALSE;

    return m_pMediaPlayer->HasHandle();
}

void CMediaPlayer::SetType(int nType)
{
    if(m_pMediaPlayer)
    {
        delete m_pMediaPlayer;
        m_pMediaPlayer = NULL;
    }

    switch(nType)
    {
        case MEDIA_PLAYER_WINAMP:
        {
            m_pMediaPlayer = new CWinamp();
            break;
        }
        case MEDIA_PLAYER_FOOBAR:
        {
            m_pMediaPlayer = new CFoobar();
            break;
        }
        case MEDIA_PLAYER_WMP:
        {
            m_pMediaPlayer = new CWMP();
            break;
        }
        case MEDIA_PLAYER_ITUNES:
        {
            m_pMediaPlayer = new CiTunes();
            break;
        }
        case MEDIA_PLAYER_RHAPSODY:
        {
            m_pMediaPlayer = new CRhapsody();
            break;
        }
    }
}


int CMediaPlayer::GetType()
{
    if(!m_pMediaPlayer)
        return -1;

    return m_pMediaPlayer->GetType();
}