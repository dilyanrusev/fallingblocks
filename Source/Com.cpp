#include "stdafx.h"
#include "Com.h"
#include <cassert>

Com::Com()
		: m_initialized(false) {
	HRESULT hr = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	assert(SUCCEEDED(hr));
	m_initialized = true;
}


Com::~Com() {
	if (m_initialized) {
		::CoUninitialize();
	}
}
