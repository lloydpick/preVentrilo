#pragma once

#include "stdafx.h"
#include "MediaPlayerBase.h"

class CMediaPlayer
{
public:
	//-----------------------------------------------------------------------
	//! \brief Contructor
	//-----------------------------------------------------------------------
	CMediaPlayer(void);

	//-----------------------------------------------------------------------
	//! \brief Destructor
	//-----------------------------------------------------------------------
	virtual ~CMediaPlayer(void);

	//-----------------------------------------------------------------------
	//! \brief Returns whether or not the media player is currently playing.
	//-----------------------------------------------------------------------
	virtual bool IsPlaying();

	//-----------------------------------------------------------------------
	//! \brief Returns whether or not the media player is currently paused.
	//-----------------------------------------------------------------------
	virtual bool IsPaused();

	//-----------------------------------------------------------------------
	//! \brief Returns the media player's current volume as a percent.
	//-----------------------------------------------------------------------
	virtual int CurrentVolume();

	//-----------------------------------------------------------------------
	//! \brief Sets the media player's volume.
	//!
	//! \param nVolume What to set the volume to as a percent
	//-----------------------------------------------------------------------
	virtual void SetVolume(int nVolume);

	//-----------------------------------------------------------------------
	//! \brief Pauses the media player if it is currently playing.
	//-----------------------------------------------------------------------
	virtual void Pause();

	//-----------------------------------------------------------------------
	//! \brief Pauses the media player if it is currently playing.
	//-----------------------------------------------------------------------
	virtual void UnPause();

	//-----------------------------------------------------------------------
	//! \brief Attempts to play the media player if it is not currently 
	//!        playing.
	//-----------------------------------------------------------------------
	virtual void Play();

	//-----------------------------------------------------------------------
	//! \brief Attempts to get a handle to the media player
	//-----------------------------------------------------------------------
	virtual void FindHandle();

	//-----------------------------------------------------------------------
	//! \brief Returns the media player type
	//-----------------------------------------------------------------------
    virtual void SetType(int nType);

	//-----------------------------------------------------------------------
	//! \brief Returns the media player type
	//-----------------------------------------------------------------------
    virtual int GetType();

	//-----------------------------------------------------------------------
	//! \brief Returns whether or not the object has a handle to its media
	//!        player.
	//-----------------------------------------------------------------------
	virtual bool HasHandle();
protected:
	CMediaPlayerBase* m_pMediaPlayer; //!< pointer to the media player
};
