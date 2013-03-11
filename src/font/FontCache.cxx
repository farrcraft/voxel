#include "FontCache.h"
#include "TextureAtlas.h"
#include "TextureFont.h"

wchar_t * wcsdupstr(const wchar_t * string)
{
	wchar_t * result;
	size_t len = (wcslen(string) + 1) * sizeof(wchar_t);
	result = new wchar_t [len];
	wcscpy(result, string);
	return result;
}

FontCache::FontCache(unsigned int width, unsigned int height, unsigned int depth)
{
	atlas_.reset(new TextureAtlas(width, height, depth));
	cache_ = wcsdupstr(L" ");
}

FontCache::~FontCache()
{
	delete [] cache_;
}

boost::shared_ptr<TextureAtlas> FontCache::atlas()
{
	return atlas_;
}

void FontCache::charcodes(const wchar_t * charcodes)
{
	delete [] cache_;
	cache_ = wcsdupstr(charcodes);
}

boost::shared_ptr<TextureFont> FontCache::load(const std::string & filename, float size)
{
	for (unsigned int i = 0; i < fonts_.size(); ++i)
	{
		if (fonts_[i]->filename() == filename && fonts_[i]->size() == size)
		{
			return fonts_[i];
		}
	}
	boost::shared_ptr<TextureFont> font;
	font.reset(new TextureFont(atlas_, filename, size));
	font->loadGlyphs(cache_);
	fonts_.push_back(font);
	return font;
}

bool FontCache::remove(boost::shared_ptr<TextureFont> font)
{
	for (unsigned int i = 0; i < fonts_.size(); ++i)
	{
		if (fonts_[i]->filename() == font->filename() && fonts_[i]->size() == font->size())
		{
			fonts_.erase(fonts_.begin() + i);
			return true;
		}
	}
	return false;
}