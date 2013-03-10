#pragma once

#include <glm/glm.hpp>

#include <vector>

class TextureAtlas
{
	public:
		TextureAtlas(unsigned int width, unsigned int height, unsigned int depth);
		~TextureAtlas();

		unsigned int width() const;
		unsigned int height() const;
		unsigned int depth() const;
		unsigned int id() const;

		void write(const std::string & filename);

		glm::ivec4 region(unsigned int width, unsigned int height);
		void region(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char * data, unsigned int stride);

		void upload();

	protected:
		int fit(unsigned int index, unsigned int width, unsigned int height);
		void merge();

	private:
		std::vector<glm::ivec3> nodes_;
		unsigned int width_;
		unsigned int height_;
		unsigned int depth_;
		size_t used_;
		unsigned int id_;
		unsigned char * data_;
};