// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "Blackboard/BlackboardListener.hpp"
#include "net/http/GeminiClient.hpp"
#include "Audio/TTSManager.hpp"
#include "thread/Mutex.hxx"
#include "time/TimeStamp.hpp"

namespace Engine {

/**
 * Assistente de Voo baseado em IA.
 * Monitora o Blackboard e fornece conselhos de voz via Gemini.
 */
class AIFlightAssistant final : public NullBlackboardListener {
  Net::GeminiClient &gemini;
  Audio::TTSManager &tts;

  Mutex mutex;
  TimeStamp last_advice_time;
  
  // Limitar a frequência de conselhos (ex: a cada 5 minutos ou evento importante)
  static constexpr unsigned ADVICE_INTERVAL_MS = 300000; 

public:
  AIFlightAssistant(Net::GeminiClient &_gemini, Audio::TTSManager &_tts)
    :gemini(_gemini), tts(_tts), last_advice_time(TimeStamp::Now()) {}

  /* virtual methods from BlackboardListener */
  void OnCalculatedUpdate(const MoreData &basic,
                          const DerivedInfo &calculated) override;

private:
  /**
   * Decide se deve pedir um conselho baseado no estado atual.
   */
  bool ShouldRequestAdvice(const MoreData &basic,
                           const DerivedInfo &calculated);

  /**
   * Gera o prompt para o Gemini baseado nos dados de voo.
   */
  std::string GeneratePrompt(const MoreData &basic,
                             const DerivedInfo &calculated);
};

} // namespace Engine
