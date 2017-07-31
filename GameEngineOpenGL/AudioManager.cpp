#include "AudioManager.h"
#include "ErrManager.h"

namespace ge {

	void SoundEffect::play(int loops /*= 0*/)
	{
		if (-1 == Mix_PlayChannel(-1, chunk_, loops)) {
			if (-1 == Mix_PlayChannel(0, chunk_, loops)) {
				fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}
	}

	void Music::play(int loops /*= *-1*/)
	{
		if (-1 == Mix_PlayMusic(music_, loops)) {
			fatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
		}
	}

	AudioManager::AudioManager() { /*empty*/ }

	AudioManager::~AudioManager()
	{
		destroy();
	}

	void AudioManager::init()
	{
		if (isInititalized_) {
			fatalError("Tried to initialize Audio Engine twice!\n");
		}
		// Can be combo of: MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (-1 == Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG)) {
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}
		if (-1 == Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024)) {
			fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
		}
		isInititalized_ = true;
	}

	void AudioManager::destroy()
	{
		if (isInititalized_) {
			isInititalized_ = false;

			for (auto& it : effectsMap_) {
				Mix_FreeChunk(it.second);
			}
			effectsMap_.clear();

			for (auto& it : musicsMap_) {
				Mix_FreeMusic(it.second);
			}
			musicsMap_.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	SoundEffect AudioManager::loadSoundEffect(const std::string & filePath)
	{
		// Attempting to find this audoi effect in the cache map
		auto it = effectsMap_.find(filePath);

		SoundEffect effect;

		if (it == effectsMap_.end()) {
			// loading sound effect
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (nullptr == chunk) {
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}

			effectsMap_[filePath] = chunk;
			effect.chunk_ = chunk;
		}
		else {
			// it's already cached
			effect.chunk_ = it->second;
		}

		return effect;
	}

	Music AudioManager::loadMusic(const std::string & filePath)
	{
		// Attempting to find this audoi effect in the cache map
		auto it = musicsMap_.find(filePath);

		Music music;

		if (it == musicsMap_.end()) {
			// loading sound effect
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (nullptr == mixMusic) {
				fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
			}

			musicsMap_[filePath] = mixMusic;
			music.music_ = mixMusic;
		}
		else {
			// it's already cached
			music.music_ = it->second;
		}

		return music;

		return Music();
	}
}