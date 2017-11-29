#pragma once
#include "Standard.h"
namespace cst {
	class EventService {
	private:
		EventService();
	public:
		EventService(EventService const&) = delete;
		static EventService & current();
		void update();
		
	};
}