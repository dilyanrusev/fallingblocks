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

#include "FileFontCollectionLoader.h"
#include "stdafx.h"

FileFontCollectionLoader::FileFontCollectionLoader()
		: m_refCount(0)
		, m_currentFileIndex(-1)
		, m_factory(nullptr) {
}


FileFontCollectionLoader::~FileFontCollectionLoader() {
	if (m_factory) {
		m_factory->Release();
	}
}

ULONG FileFontCollectionLoader::AddRef() {
	return ++m_refCount;
}

ULONG FileFontCollectionLoader::Release() {
	ULONG res = --m_refCount;
	if (res == 0) {
		delete this;
	}
	return res;
}

HRESULT FileFontCollectionLoader::QueryInterface(REFIID riid, void** ppvObject) {
	if (!ppvObject) return E_POINTER;
	if (riid == IID_IUnknown || riid == __uuidof(IDWriteFontCollectionLoader) 
			|| riid == __uuidof(IDWriteFontFileEnumerator) ) {
		AddRef();
		*ppvObject = this;
		return S_OK;
	} else {
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}
}

HRESULT FileFontCollectionLoader::CreateEnumeratorFromKey(IDWriteFactory* factory, const void* collectionKey,
		UINT32 collectionKeySize, IDWriteFontFileEnumerator** fontFileEnumerator) {
	UNREFERENCED_PARAMETER(collectionKeySize);

	if (!fontFileEnumerator) return E_POINTER;
	
	if (m_factory) {
		m_factory->Release();
	}
	m_factory = factory;
	m_factory->AddRef();

	m_currentFileIndex = -1;
	m_fontFiles.clear();	
	WIN32_FIND_DATA fd;
	HANDLE handle = ::FindFirstFile(reinterpret_cast<const wchar_t*>(collectionKey), &fd);
	if (handle != INVALID_HANDLE_VALUE) {
		do {
			m_fontFiles.push_back(std::wstring(fd.cFileName));
		} while (::FindNextFile(handle, &fd));
		::FindClose(handle);
	}

	AddRef();
	*fontFileEnumerator = this;
	return S_OK;
}

HRESULT FileFontCollectionLoader::MoveNext(BOOL* hasCurrentFile) {
	if (!hasCurrentFile) return E_POINTER;

	m_currentFileIndex++;
	*hasCurrentFile = m_currentFileIndex < m_fontFiles.size();

	return S_OK;
}

HRESULT FileFontCollectionLoader::GetCurrentFontFile(IDWriteFontFile** fontFile) {
	if (!fontFile) return E_POINTER;
	if (!m_factory) return E_FAIL;

	HRESULT hr = m_factory->CreateFontFileReference(m_fontFiles[m_currentFileIndex].c_str(),
		nullptr, fontFile);

	return hr;
}