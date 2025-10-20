#pragma once
#include "ui/panel.h"
#include <memory>

namespace ui {

	class GamePanel : public Panel {
		public:
			GamePanel() = default;
			void init() override;
			void render() override;
			void end() override {}
			std::vector<Game>* games = nullptr;
			void definePointers() override;

  	protected:
    	std::vector<Game>* games_ = nullptr;
  };

  using GamePanelPtr = std::unique_ptr<GamePanel>;

} //namespace ui