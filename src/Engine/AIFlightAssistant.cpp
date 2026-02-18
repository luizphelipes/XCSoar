// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "AIFlightAssistant.hpp"
#include "NMEA/MoreData.hpp"
#include "NMEA/Derived.hpp"
#include <fmt/format.h>

namespace Engine {

void 
AIFlightAssistant::OnCalculatedUpdate(const MoreData &basic,
                                      const DerivedInfo &calculated)
{
  if (!ShouldRequestAdvice(basic, calculated))
    return;

  std::string prompt = GeneratePrompt(basic, calculated);
  
  // Usar uma corrotina para não bloquear a thread UI
  // Capturamos as referências necessárias de forma segura
  auto task = gemini.Chat("gemini-3-pro-preview", prompt.c_str());
  
  // Como OnCalculatedUpdate roda na UI thread, podemos usar a corrotina
  // Mas precisamos de um mecanismo para disparar e esquecer (Fire & Forget)
  // No XCSoar, Co::InvokeTask não inicia sozinho sem um co_await.
  // Usaremos um padrão de "Task Runner" ou similar se disponível.
  
  // Por simplicidade neste protótipo, assumimos que gemini.Chat retorna um InvokeTask
  // que podemos gerenciar.
}

bool 
AIFlightAssistant::ShouldRequestAdvice(const MoreData &basic,
                                       const DerivedInfo &calculated)
{
  TimeStamp now = TimeStamp::Now();
  if (now.ToMS() - last_advice_time.ToMS() < ADVICE_INTERVAL_MS)
    return false;

  // Só dar conselhos se estiver voando
  if (!calculated.flight.flying)
    return false;

  last_advice_time = now;
  return true;
}

std::string 
AIFlightAssistant::GeneratePrompt(const MoreData &basic,
                                  const DerivedInfo &calculated)
{
  return fmt::format(
    "Você é um assistente de voo para um piloto de planador. "
    "Dados atuais: Altitude: {:.0f}m, Vário: {:.1f}m/s, Vento: {:.0f}km/h de {:03.0f}°. "
    "Estado de voo: {}. "
    "Dê um conselho tático curto (máximo 20 palavras) sobre térmicas ou navegação.",
    basic.location.altitude, 
    calculated.netto_vario,
    calculated.wind.speed * 3.6,
    calculated.wind.bearing.Degrees(),
    calculated.flight.circling ? "Círculando em térmica" : "Planeio reto"
  );
}

} // namespace Engine
