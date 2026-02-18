// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "PCMBufferDataSource.hpp"
#include <string>
#include <memory>

class PCMPlayer;

namespace Audio {

/**
 * Gerenciador de Text-to-Speech (TTS) utilizando eSpeak-NG.
 */
class TTSManager {
  std::unique_ptr<PCMPlayer> player;
  PCMBufferDataSource source;
  std::string current_language;

public:
  TTSManager();
  ~TTSManager();

  /**
   * Define o idioma para o TTS (ex: "pt", "en").
   */
  void SetLanguage(const char *lang);

  /**
   * Converte texto em fala e reproduz.
   */
  void Speak(const char *text);

  /**
   * Para qualquer reprodução em curso.
   */
  void Stop();

private:
  /**
   * Callback para o eSpeak-NG processar os samples gerados.
   */
  static int SynthCallback(short *wav, int num_samples, void *user_data);
};

} // namespace Audio
