#pragma once
#include <string>
#include <map>

#include <SDL/SDL_mixer.h>

namespace ge {

	class SoundEffect {
	public:
		friend class AudioManager;

		/// <summary>
		/// Plays an sound effect file
		/// </summary>
		/// <param name="loops">
		/// to play forever set to -1, otherwise, play loops + 1 times.
		/// </param>
		void play(int loops = 0);
	private:
		Mix_Chunk* chunk_ = nullptr;
	};

	class Music {
	public:
		friend class AudioManager;

		/// <summary>
		/// Plays an audio file
		/// </summary>
		/// <param name="loops">
		/// to play forever set to -1, otherwise, play 'loop' times.
		/// </param>
		void play(int loops = 1);

		static void pause() { Mix_PauseMusic(); }
		static void stop() { Mix_HaltMusic(); }
		static void resume() { Mix_ResumeMusic(); }
	private:
		Mix_Music* m_music = nullptr;
	};

	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();

		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);
	private:	
		std::map<std::string, Mix_Chunk*> m_effectsMap;
		std::map<std::string, Mix_Music*> m_musicsMap;

		bool m_isInititalized = false;
	};
}

