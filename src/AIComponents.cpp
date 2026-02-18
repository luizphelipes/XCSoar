// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "AIComponents.hpp"
#include "UISettings.hpp"

AIComponents::AIComponents(CurlGlobal &curl, const UISettings &settings) noexcept
  :gemini(new Net::GeminiClient(curl, settings.ai.api_key.c_str())),
   tts(new Audio::TTSManager()),
   flight_assistant(new Engine::AIFlightAssistant(*gemini, *tts))
{
  tts->SetLanguage(settings.ai.language.c_str());
}

AIComponents::~AIComponents() noexcept = default;
