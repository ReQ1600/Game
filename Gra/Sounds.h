#pragma once
#include <SFML/Audio.hpp>
#include <format>
#include "Utils.h"

namespace Sound
{
	struct Sound
	{
		Sound(const char* filename, sf::Time offset = sf::seconds(0)) : m_offset(offset) {
			m_pBuffer = new sf::SoundBuffer;
			m_pSound = new sf::Sound;

			try
			{
				if (!(m_pBuffer->loadFromFile(std::format("assets\\sounds\\{0}",filename))))
					throw Utils::ErrorLog("Sound creation: sound buffer loading probably bad sound filepath");
			}
			catch (const bool&)
			{
				return;
			}

			m_pSound->setBuffer(*m_pBuffer);
			
		};
		~Sound() {};
		void Play() { m_pSound->setPlayingOffset(m_offset); m_pSound->play(); };
	private:
		sf::SoundBuffer* m_pBuffer = nullptr;
		sf::Sound* m_pSound = nullptr;
		sf::Time m_offset;
	};

	struct LoopedSound
	{
		LoopedSound(const char* filename) 
		{
			m_music = new sf::Music;
			try
			{
				if (!(m_music->openFromFile(std::format("assets\\sounds\\{0}", filename))))
					throw Utils::ErrorLog("LoopedSound creation: music file opening probably bad sound filepath");
			}
			catch (const bool&)
			{
				return;
			}
			m_music->setLoop(true);
		};
		~LoopedSound() {};
		void Play() { m_music->play(); };
		void Stop() { m_music->stop(); };
		void Pause() { m_music->pause(); };
		sf::SoundSource::Status getState() const { return m_music->getStatus(); }
	private:
		sf::Music* m_music = nullptr;
	};

	//sounds
	namespace Sounds
	{
		inline Sound buttonSound("button.wav", sf::milliseconds(600));
		inline Sound PlayerHitSound("hurt.wav");
		inline Sound EnemyDeathSound("eDeath.wav");
		inline Sound GameLost("lose.wav",sf::milliseconds(600));
		inline Sound GameWon("win.wav");
		inline Sound ProjectileSplat("splat.wav");
		inline Sound PlayerProjectileThrow("throw.wav");
		inline LoopedSound Walking("walking.wav");
	}
}