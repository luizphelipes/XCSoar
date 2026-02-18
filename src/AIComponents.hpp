// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "net/http/GeminiClient.hpp"
#include "Audio/TTSManager.hpp"
#include "Engine/AIFlightAssistant.hpp"

#include <memory>

struct UISettings;
class CurlGlobal;

/**
 * Gerenciador global para os componentes de IA.
 */
struct AIComponents {
  const std::unique_ptr<Net::GeminiClient> gemini;
  const std::unique_ptr<Audio::TTSManager> tts;
  const std::unique_ptr<Engine::AIFlightAssistant> flight_assistant;

  AIComponents(CurlGlobal &curl, const UISettings &settings) noexcept;
  ~AIComponents() noexcept;

  AIComponents(const AIComponents &) = delete;
  AIComponents &operator=(const AIComponents &) = delete;
};
