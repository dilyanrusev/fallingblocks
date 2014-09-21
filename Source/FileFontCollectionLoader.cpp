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