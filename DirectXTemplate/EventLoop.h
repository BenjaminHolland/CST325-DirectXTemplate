#pragma once
#include <Windows.h>
class EventLoop {
private:
	static bool _is_initialized;
	static bool _continue;
public:
	static HRESULT initialize();
	static inline bool is_initialized() { return _is_initialized; }
	static void run();
};