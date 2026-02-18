// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "co/Task.hxx"
#include "lib/curl/CoRequest.hxx"

#include <string>
#include <string_view>

class CurlGlobal;

namespace Net {

/**
 * Cliente para a API Google Gemini 3.0.
 */
class GeminiClient {
  CurlGlobal &curl;
  std::string api_key;

public:
  GeminiClient(CurlGlobal &_curl, std::string_view _api_key) noexcept
    :curl(_curl), api_key(_api_key) {}

  /**
   * Envia um prompt para o modelo Gemini e retorna a resposta.
   * 
   * @param model_name Nome do modelo (ex: "gemini-3-pro-preview")
   * @param prompt O texto a ser enviado
   */
  Co::InvokeTask<std::string> Chat(const char *model_name,
                                  const char *prompt);

private:
  /**
   * Monta a URL para a requisição.
   */
  std::string MakeUrl(const char *model_name) const;
};

} // namespace Net
