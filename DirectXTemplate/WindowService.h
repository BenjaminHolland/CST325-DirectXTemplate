#pragma once
#include "Standard.h"
namespace cst {
	class WindowService {
	private:
		sf::Window _window;
		WindowService();
	public:
		WindowService(WindowService const&) = delete;
		static cst::WindowService& current();
		void with(std::function<void(sf::Window&)> execute);
	};

}