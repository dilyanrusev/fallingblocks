/*
Copyright (c) 2014, Dilyan Rusev
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

* Neither the name of the {organization} nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "FileFontEnumerator.h"
#include "stdafx.h"

FileFontEnumerator::FileFontEnumerator(IDWriteFactory* factory, std::wstring path)
		: m_refCount(0)
		, m_currentFileIndex(-1)
		, m_factory(factory) {
	factory->AddRef();
	WIN32_FIND_DATA fd;
	HANDLE handle = ::FindFirstFile(path.c_str(), &fd);
	if (handle != INVALID_HANDLE_VALUE) {
		do {
			m_fontFiles.push_back(std::wstring(fd.cFileName));
		} while (::FindNextFile(handle, &fd));
		::FindClose(handle);
	}
}

FileFontEnumerator::~FileFontEnumerator() {
	m_factory->Release();	
}

ULONG FileFontEnumerator::AddRef() {
	return ++m_refCount;
}

ULONG FileFontEnumerator::Release() {
	ULONG res = --m_refCount;
	if (res == 0) {
		delete this;
	}
	return res;
}

HRESULT FileFontEnumerator::QueryInterface(REFIID riid, void** ppvObject) {
	if (!ppvObject) return E_POINTER;
	if (riid == IID_IUnknown || riid == __uuidof(IDWriteFontFileEnumerator)) {
		AddRef();
		*ppvObject = this;
		return S_OK;
	} else {
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}
}

HRESULT FileFontEnumerator::MoveNext(BOOL* hasCurrentFile) {
	if (!hasCurrentFile) return E_POINTER;

	m_currentFileIndex++;
	*hasCurrentFile = m_currentFileIndex < m_fontFiles.size();

	return S_OK;
}

HRESULT FileFontEnumerator::GetCurrentFontFile(IDWriteFontFile** fontFile) {
	if (!fontFile) return E_POINTER;
	if (!m_factory) return E_FAIL;

	HRESULT hr = m_factory->CreateFontFileReference(m_fontFiles[m_currentFileIndex].c_str(),
		nullptr, fontFile);

	return hr;
}