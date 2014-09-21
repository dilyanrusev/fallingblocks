#pragma once

#include <dwrite.h>
#include <vector>
#include <string>

class FileFontCollectionLoader : public IDWriteFontCollectionLoader, public IDWriteFontFileEnumerator {
public:
	FileFontCollectionLoader();
	virtual ~FileFontCollectionLoader();

	// IUnknown
	virtual ULONG STDMETHODCALLTYPE AddRef() override;
	virtual ULONG STDMETHODCALLTYPE Release() override;
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;
	
	// IDWriteFontCollectionLoader
	STDMETHOD(CreateEnumeratorFromKey)(IDWriteFactory* factory, const void* collectionKey,
		UINT32 collectionKeySize, IDWriteFontFileEnumerator** fontFileEnumerator) override;

	// IDWriteFontFileEnumerator
	STDMETHOD(GetCurrentFontFile)(IDWriteFontFile** fontFile) override;
	STDMETHOD(MoveNext)(BOOL* hasCurrentFile) override;

private:
	ULONG m_refCount;
	std::vector<std::string> m_fontFiles;
	int m_currentFileIndex;
	IDWriteFactory* m_factory;
};

