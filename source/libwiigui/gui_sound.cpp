/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_sound.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "gui.h"

/**
 * Constructor for the GuiSound class.
 */
GuiSound::GuiSound(const u8 * s, s32 l, SOUND t)
{
	sound = s;
	length = l;
	type = t;
	voice = -1;
	volume = 100;
	loop = false;
}

/**
 * Destructor for the GuiSound class.
 */
GuiSound::~GuiSound()
{
	#ifndef NO_SOUND
	if(type == SOUND::OGG)
		StopOgg();
	#endif
}

void GuiSound::Play()
{
	#ifndef NO_SOUND
	int vol;

	switch(type)
	{
		case SOUND::PCM:
		vol = 255*(volume/100.0);
		voice = ASND_GetFirstUnusedVoice();
		if(voice >= 0)
			ASND_SetVoice(voice, VOICE_STEREO_16BIT, 48000, 0,
				(u8 *)sound, length, vol, vol, nullptr);
		break;

		case SOUND::OGG:
		voice = 0;
		if(loop)
			PlayOgg((char *)sound, length, 0, OGG_INFINITE_TIME);
		else
			PlayOgg((char *)sound, length, 0, OGG_ONE_TIME);
		SetVolumeOgg(255*(volume/100.0));
		break;
	}
	#endif
}

void GuiSound::Stop()
{
	#ifndef NO_SOUND
	if(voice < 0)
		return;

	switch(type)
	{
		case SOUND::PCM:
		ASND_StopVoice(voice);
		break;

		case SOUND::OGG:
		StopOgg();
		break;
	}
	#endif
}

void GuiSound::Pause()
{
	#ifndef NO_SOUND
	if(voice < 0)
		return;

	switch(type)
	{
		case SOUND::PCM:
		ASND_PauseVoice(voice, 1);
		break;

		case SOUND::OGG:
		PauseOgg(1);
		break;
	}
	#endif
}

void GuiSound::Resume()
{
	#ifndef NO_SOUND
	if(voice < 0)
		return;

	switch(type)
	{
		case SOUND::PCM:
		ASND_PauseVoice(voice, 0);
		break;

		case SOUND::OGG:
		PauseOgg(0);
		break;
	}
	#endif
}

bool GuiSound::IsPlaying()
{
	if(ASND_StatusVoice(voice) == SND_WORKING || ASND_StatusVoice(voice) == SND_WAITING)
		return true;
	else
		return false;
}

void GuiSound::SetVolume(int vol)
{
	#ifndef NO_SOUND
	volume = vol;

	if(voice < 0)
		return;

	int newvol = 255*(volume/100.0);

	switch(type)
	{
		case SOUND::PCM:
		ASND_ChangeVolumeVoice(voice, newvol, newvol);
		break;

		case SOUND::OGG:
		SetVolumeOgg(255*(volume/100.0));
		break;
	}
	#endif
}

void GuiSound::SetLoop(bool l)
{
	loop = l;
}
