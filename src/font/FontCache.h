#pragma once

#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>

class TextureAtlas;
class TextureFont;

class FontCache
{
	public:
		FontCache(unsigned int width, unsigned int height, unsigned int depth);
		~FontCache();

		void load(const std::string & filename, float size);
		void remove(boost::shared_ptr<TextureFont> font);

		boost::shared_ptr<TextureAtlas> atlas();

	private:
		boost::shared_ptr<TextureAtlas> atlas_;
		std::vector<boost::shared_ptr<TextureFont> > fonts_;
		wchar_t * cache_;
};