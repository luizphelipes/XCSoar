// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "AIConfigPanel.hpp"
#include "Interface.hpp"
#include "UIGlobals.hpp"
#include "Language/Language.hpp"
#include "Profile/Keys.hpp"

enum ControlIndex {
  EnableAssistant,
  APIKey,
  Language,
};

static const char *const languages[] = {
  "pt", "en", "de", "fr", nullptr
};

AIConfigPanel::AIConfigPanel()
  : RowFormWidget(UIGlobals::GetDialogLook()) {
}

void
AIConfigPanel::Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept
{
  RowFormWidget::Prepare(parent, rc);

  const auto &settings = CommonInterface::GetUISettings().ai;

  AddBoolean(_("Ativar Assistente"),
             _("Ativa o assistente de voz em tempo real durante o voo."),
             settings.enable_flight_assistant);

  AddString(_("Chave API Gemini"),
            _("Insira sua chave de API do Google Gemini 3.0 aqui."),
            settings.api_key.c_str());

  AddEnum(_("Idioma"),
          _("Escolha o idioma do assistente de voz."),
          languages, settings.language.c_str());
}

bool
AIConfigPanel::Save(bool &changed) noexcept
{
  auto &settings = CommonInterface::SetUISettings().ai;

  changed |= SaveValueBoolean(EnableAssistant, ProfileKeys::AIAssistantEnabled,
                              settings.enable_flight_assistant);
  
  changed |= SaveValueString(APIKey, ProfileKeys::AIAssistantKey,
                             settings.api_key.buffer(),
                             settings.api_key.capacity());

  changed |= SaveValueEnum(Language, ProfileKeys::AIAssistantLanguage,
                           languages, settings.language.buffer(),
                           settings.language.capacity());

  return true;
}

std::unique_ptr<Widget>
CreateAIConfigPanel()
{
  return std::make_unique<AIConfigPanel>();
}
