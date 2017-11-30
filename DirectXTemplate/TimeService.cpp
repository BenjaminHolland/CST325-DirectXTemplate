#include "TimeService.h"
#include <iostream>
using namespace cst;
using namespace DirectX;
using namespace std;
TimeService::TimeService() :_frame(0) {

}
TimeService & cst::TimeService::current()
{
	static TimeService current;
	return current;
}

void TimeService::with(function<void(XMFLOAT2)> execute) {
	XMFLOAT2 cont = { 0,0 };
	cout << "Not Implemented" << endl;
	execute(cont);
}
void cst::TimeService::update()
{
	_frame++;
}
void TimeService::with(function<void(XMUINT2)> execute) {
	XMUINT2 disc = { _frame,0 };
	execute(disc);
	_frame = disc.x;
}