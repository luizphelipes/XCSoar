// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Dialogs/Dialogs.h"
#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/TextEntry.hpp"
#include "Widget/RichTextWidget.hpp"
#include "Look/DialogLook.hpp"
#include "UIGlobals.hpp"
#include "AIComponents.hpp"
#include "Components.hpp"
#include "Language/Language.hpp"
#include "util/StaticString.hxx"

void
dlgAIAssistantShowModal()
{
  const DialogLook &look = UIGlobals::GetDialogLook();

  WidgetDialog dialog(WidgetDialog::Full{}, UIGlobals::GetMainWindow(),
                      look, _("Assistente IA"));

  auto layout = std::make_unique<VScrollWidget>(
    std::make_unique<RichTextWidget>(look, _("Como posso ajudar com sua configuração hoje?")), 
    look, true);
  
  RichTextWidget *text_widget = (RichTextWidget *)layout->GetInnerWidget();

  dialog.AddButton(_("Perguntar"), mrNext);
  dialog.AddButton(_("Fechar"), mrOK);

  dialog.FinishPreliminary(std::move(layout));

  while (true) {
    const int result = dialog.ShowModal();
    if (result != mrNext)
      break;

    StaticString<256> question;
    if (TouchTextEntry(question, _("Sua pergunta:"))) {
      text_widget->SetText(_("Pensando..."));
      dialog.Invalidate();

      // Chamada síncrona para o protótipo (em produção deve ser assíncrono)
      // Nota: O XCSoar tem padrões de CoDialog para isso.
      try {
        if (ai_components && ai_components->gemini) {
          // Aqui faríamos o co_await, mas para o protótipo vamos simular ou usar um padrão síncrono
          // se for permitido. Como estamos na thread UI, o ideal é usar CoDialog.
          text_widget->SetText(_("Conectando ao Gemini..."));
          // ... lógica de chamada ...
          text_widget->SetText(_("Desculpe, a integração assíncrona do diálogo está em desenvolvimento. "
                               "O GeminiClient já está pronto para uso no backend!"));
        }
      } catch (...) {
        text_widget->SetText(_("Erro ao contatar a IA. Verifique sua chave de API e conexão."));
      }
    }
  }
}
