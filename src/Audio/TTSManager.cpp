// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "TTSManager.hpp"
#include "PCMPlayer.hpp"
#include "PCMPlayerFactory.hpp"
#include <espeak-ng/espeak_ng.h>
#include <vector>
#include <mutex>

namespace Audio {

static std::vector<int16_t> global_audio_buffer;
static std::mutex audio_mutex;

TTSManager::TTSManager()
{
  espeak_ng_Initialize(nullptr, 0, nullptr, 0);
  espeak_ng_SetSynthCallback(SynthCallback);
  
  // Criar o player para a plataforma atual
  player.reset(PCMPlayerFactory::CreateInstance());
}

TTSManager::~TTSManager()
{
  Stop();
  espeak_ng_Terminate();
}

int 
TTSManager::SynthCallback(short *wav, int num_samples, void *user_data)
{
  if (wav == nullptr) return 0;

  std::lock_guard<std::mutex> lock(audio_mutex);
  global_audio_buffer.insert(global_audio_buffer.end(), wav, wav + num_samples);
  
  return 0; // Continuar síntese
}

void 
TTSManager::SetLanguage(const char *lang)
{
  espeak_ng_SetVoiceByName(lang);
}

void 
TTSManager::Speak(const char *text)
{
  Stop();

  {
    std::lock_guard<std::mutex> lock(audio_mutex);
    global_audio_buffer.clear();
  }

  // eSpeak-NG sintetiza para o buffer global via callback
  espeak_ng_Synthesize(text, strlen(text) + 1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, nullptr, nullptr);
  
  // Adicionar o buffer à fonte de dados do PCMPlayer
  std::lock_guard<std::mutex> lock(audio_mutex);
  source.Add(std::span<const int16_t>(global_audio_buffer.data(), global_audio_buffer.size()));
  
  player->Start(source);
}

void 
TTSManager::Stop()
{
  if (player) {
    player->Stop();
  }
  source.Clear();
}

} // namespace Audio
