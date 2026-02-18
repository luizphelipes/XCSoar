// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "Widget/RowFormWidget.hpp"
#include <memory>

class AIConfigPanel final : public RowFormWidget {
public:
  AIConfigPanel();

  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override;
  bool Save(bool &changed) noexcept override;
};

std::unique_ptr<Widget> CreateAIConfigPanel();
