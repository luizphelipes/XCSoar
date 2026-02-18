// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "GeminiClient.hpp"
#include "lib/curl/Slist.hxx"
#include "lib/curl/CoRequest.hxx"
#include <boost/json.hpp>
#include <fmt/format.h>

namespace Net {

std::string
GeminiClient::MakeUrl(const char *model_name) const
{
  return fmt::format("https://generativelanguage.googleapis.com/v1beta/models/{}:generateContent?key={}",
                     model_name, api_key);
}

Co::InvokeTask<std::string>
GeminiClient::Chat(const char *model_name, const char *prompt)
{
  std::string url = MakeUrl(model_name);
  
  CurlEasy easy(url.c_str());
  easy.SetPost(true);

  // Construir o corpo da requisição JSON usando boost::json
  boost::json::object request_obj;
  boost::json::array contents_array;
  boost::json::object content_obj;
  boost::json::array parts_array;
  boost::json::object part_obj;

  part_obj["text"] = prompt;
  parts_array.push_back(part_obj);
  content_obj["parts"] = parts_array;
  contents_array.push_back(content_obj);
  request_obj["contents"] = contents_array;

  std::string request_body = boost::json::serialize(request_obj);
  easy.SetRequestBody(request_body);

  // Configurar Headers
  CurlSlist headers;
  headers.Append("Content-Type: application/json");
  easy.SetRequestHeaders(headers.Get());

  // Executar a requisição de forma assíncrona
  Curl::CoRequest request(curl, std::move(easy));
  auto response = co_await request;

  if (response.status != 200) {
     throw std::runtime_error(fmt::format("Gemini API error: HTTP {}", response.status));
  }

  // Parsear a resposta
  auto response_val = boost::json::parse(response.body);
  auto const& candidates = response_val.as_object().at("candidates").as_array();
  if (candidates.empty()) {
    co_return "";
  }

  auto const& parts = candidates.at(0).as_object().at("content").as_object().at("parts").as_array();
  if (parts.empty()) {
    co_return "";
  }

  co_return std::string(parts.at(0).as_object().at("text").as_string());
}

} // namespace Net
