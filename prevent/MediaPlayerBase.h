#pragma once

#include "stdafx.h"

class CMediaPlayerBase
{
public:
	//-----------------------------------------------------------------------
	//! \brief Returns whether or not the media player is currently playing.
	//-----------------------------------------------------------------------
	virtual bool IsPlaying() = 0;

	//-----------------------------------------------------------------------
	//! \brief Returns whether or not the media player is currently paused.
	//-----------------------------------------------------------------------
	virtual bool IsPaused() = 0;

	//-----------------------------------------------------------------------
	//! \brief Returns the media player's current volume as a percent.
	//-----------------------------------------------------------------------
	virtual int CurrentVolume() = 0;

	//-----------------------------------------------------------------------
	//! \brief Sets the media player's volume.
	//!
	//! \param nVolume What to set the volume to as a percent
	//-----------------------------------------------------------------------
	virtual void SetVolume(int nVolume) = 0;

	//-----------------------------------------------------------------------
	//! \brief Pauses the media player if it is currently playing.
	//-----------------------------------------------------------------------
	virtual void Pause() = 0;

	//-----------------------------------------------------------------------
	//! \brief UnPauses the media player if it is currently paused.
	//-----------------------------------------------------------------------
	virtual void UnPause() = 0;

	//-----------------------------------------------------------------------
	//! \brief Attempts to play the media player if it is not currently 
	//!        playing.
	//-----------------------------------------------------------------------
	virtual void Play() = 0;

	//-----------------------------------------------------------------------
	//! \brief Attempts to get a handle to the media player
	//-----------------------------------------------------------------------
	virtual void FindHandle() = 0;

	//-----------------------------------------------------------------------
	//! \brief Returns the media player type
	//-----------------------------------------------------------------------
    virtual int GetType() { return m_nType; }

	//-----------------------------------------------------------------------
	//! \brief Returns whether or not the object has a handle to its media
	//!        player.
	//-----------------------------------------------------------------------
	virtual bool HasHandle() { return (IsWindow(m_hwndMediaPlayer) == TRUE); }

	//-----------------------------------------------------------------------
	//! \brief Destructor
	//-----------------------------------------------------------------------
	virtual ~CMediaPlayerBase(void);
protected:
	//-----------------------------------------------------------------------
	//! \brief Contructor
	//-----------------------------------------------------------------------
	CMediaPlayerBase(void);

	HWND m_hwndMediaPlayer; //!< Handle to the media player
    int m_nType;            //!< Media player type
};
