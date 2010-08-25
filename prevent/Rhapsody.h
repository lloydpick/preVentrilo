#pragma once
#include "MediaPlayerBase.h"

class CRhapsody : public CMediaPlayerBase
{
public:
	//-----------------------------------------------------------------------
	//! \brief Constructor
	//-----------------------------------------------------------------------
	CRhapsody(void);

	//-----------------------------------------------------------------------
	//! \brief Destructor
	//-----------------------------------------------------------------------
	virtual ~CRhapsody(void);

	//-----------------------------------------------------------------------
	//! \brief Returns whether or not the media player is currently playing.
	//-----------------------------------------------------------------------
	bool IsPlaying();

	//-----------------------------------------------------------------------
	//! \brief Returns whether or not the media player is currently paused.
	//-----------------------------------------------------------------------
	bool IsPaused();

	//-----------------------------------------------------------------------
	//! \brief Returns the media player's current volume as a percent.
	//-----------------------------------------------------------------------
	int CurrentVolume();

	//-----------------------------------------------------------------------
	//! \brief Sets the media player's volume.
	//!
	//! \param nVolume What to set the volume to as a percent
	//-----------------------------------------------------------------------
	void SetVolume(int nVolume);

	//-----------------------------------------------------------------------
	//! \brief Pauses the media player if it is currently playing.
	//-----------------------------------------------------------------------
	void Pause();

	//-----------------------------------------------------------------------
	//! \brief Pauses the media player if it is currently paused.
	//-----------------------------------------------------------------------
	void UnPause();

	//-----------------------------------------------------------------------
	//! \brief Attempts to play the media player if it is not currently 
	//!        playing.
	//-----------------------------------------------------------------------
	void Play();

	//-----------------------------------------------------------------------
	//! \brief Attempts to get a handle to the media player
	//-----------------------------------------------------------------------
	void FindHandle();
};
