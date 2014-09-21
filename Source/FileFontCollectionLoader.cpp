#include "FileFontCollectionLoader.h"
#include "stdafx.h"

FileFontCollectionLoader::FileFontCollectionLoader()
		: m_refCount(1)
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
	if (!fontFileEnumerator) return E_POINTER;
	
	if (m_factory) {
		m_factory->Release();
	}
	m_factory = factory;
	m_factory->AddRef();

	wchar_t path[MAX_PATH];

	AddRef();
	*fontFileEnumerator = this;
	return S_OK;
}

HRESULT FileFontCollectionLoader::MoveNext(BOOL* hasCurrentFile) {
	return E_NOTIMPL;
}

HRESULT FileFontCollectionLoader::GetCurrentFontFile(IDWriteFontFile** fontFile) {
	return E_NOTIMPL;
}