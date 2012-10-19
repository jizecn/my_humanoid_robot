#ifndef _SOUND_PLAY_H
#define _SOUND_PLAY_H

#include "SDL.h"
#include "SDL_audio.h"

#include "utils/Resources.h"

#define NUMSOUNDS 2

struct sample {
	Uint8 *data;
	Uint32 dpos;
	Uint32 dlen;
};
static sample sounds[NUMSOUNDS];

class SoundPlay
{
public:

	SoundPlay()
	{
		ACE_OS::printf("SOUND PLAY CLASS\n");
		if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
			fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
			deviceInit = false;
		}
		deviceInit = true;
		atexit(SDL_Quit);    // execute function at exit
	}

	~SoundPlay()
	{
	}

	bool play(const char *filename, const char *filepath)
	{
		ACE_OS::printf("SOUND PLAY CLASS. %s / %s\n", filepath, filename);
		if (!deviceInit)
		{
			ACE_OS::fprintf(stderr, "SDL device was not initialised\n");
			return false;
		}
		if(strcmp(filepath, "") == 0)
		{
			this->filename = filename;
		}
		else
		{
			this->filename = string(filepath) + FILESEPARATOR + filename;
			ACE_OS::printf("%s\n", this->filename.c_str());
		}
		PlaySound(this->filename.c_str());

		return true;
	}

private:
	string filename;
	bool deviceInit; 


	//this is callback function,it is executed in separate thread.
	static void mixaudio(void *unused, Uint8 *stream, int len)
	{
		int i;
		Uint32 amount;

		for ( i=0; i<NUMSOUNDS; ++i ) {
			amount = (sounds[i].dlen - sounds[i].dpos);
			if ( amount > len ) {
				amount = len;
			}

			//convert original sound data from wav file to destnation format
			SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos], amount, SDL_MIX_MAXVOLUME);
			sounds[i].dpos += amount;
		}
	}

	int Open_Audio()
	{
		SDL_AudioSpec fmt;

		//open audio device and set properties
		fmt.freq = 22050;
		fmt.format = AUDIO_S16;
		fmt.channels = 2;
		fmt.samples = 512;
		fmt.callback = mixaudio;
		fmt.userdata = NULL;

		if ( SDL_OpenAudio(&fmt, NULL) < 0 )
		{
			return -1;
		}

		//begin play
		SDL_PauseAudio(0);
		return 0;
	}

	void PlaySound(const char *file)
	{
		int index;
		SDL_AudioSpec wave;
		Uint8 *data;
		Uint32 dlen;
		SDL_AudioCVT cvt;

		//find an empty sound port
		for ( index=0; index<NUMSOUNDS; ++index )
		{
			if ( sounds[index].dpos == sounds[index].dlen )
			{
				break;
			}
		}
		if ( index == NUMSOUNDS )
		{
			return;
		}

		//load wav file, and copy sound data to buffer
		if ( SDL_LoadWAV(file, &wave, &data, &dlen) == NULL )
		{
			//   debug("call SDL_LoadWAV failed\n");
			return;
		}
		//build audio convertion struct
		SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq,AUDIO_S16, 2,22050);
		//build cvt.buf, and copy sound data to this buffer,then free sound data buffer
		cvt.buf = (Uint8 *)malloc(dlen*cvt.len_mult);
		memcpy(cvt.buf, data, dlen);
		cvt.len = dlen;
		SDL_ConvertAudio(&cvt);
		SDL_FreeWAV(data);

		//free history sound buffer,it is equal to cvt.buf
		if ( sounds[index].data )
		{
			free(sounds[index].data);
		}

		//protect resource which will be used in thread mixaudio
		SDL_LockAudio();
		sounds[index].data = cvt.buf;
		sounds[index].dlen = cvt.len_cvt;
		sounds[index].dpos = 0;
		SDL_UnlockAudio();

		//open audio to play
		Open_Audio();
		//sounds[0].data;
	}

};
#endif