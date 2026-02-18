// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "util/StaticString.hxx"
#include <type_traits>

/**
 * Configurações para as funcionalidades de IA.
 */
struct AISettings {
  /** Chave da API Google Gemini */
  StaticString<128> api_key;

  /** Ativar o assistente de fala em voo */
  bool enable_flight_assistant;

  /** Idioma do Assistente (ex: "pt", "en") */
  StaticString<16> language;

  void SetDefaults() noexcept {
    api_key.Clear();
    enable_flight_assistant = false;
    language = "pt";
  }
};

static_assert(std::is_trivial<AISettings>::value, "type is not trivial");
