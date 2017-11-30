#pragma once
#include "Standard.h"
namespace cst {
	class TimeService {
	private:
		TimeService();
		unsigned int _frame;
	public:
		TimeService(TimeService const&) = delete;
		static TimeService& current();
		void with(std::function<void(DirectX::XMUINT2)> execute);
		void with(std::function<void(DirectX::XMFLOAT2)> execute);
		void update();

	};
}