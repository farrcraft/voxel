#include "TextureAtlas.h"

#include <GL/glew.h>
#include <log4cxx/logger.h>

#include <vertical3d/image/Image.h>
#include <vertical3d/image/ImageFactory.h>

#include <boost/shared_ptr.hpp>

TextureAtlas::TextureAtlas(unsigned int width, unsigned int height, unsigned int depth) :
	width_(width),
	height_(height),
	depth_(depth),
	id_(0),
	used_(0)
{
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("voxel.log"));

	if (depth != 1 && depth != 3 && depth != 4)
	{
		std::string err = "Invalid texture atlas depth - must be 1, 3, or 4";
		LOG4CXX_ERROR(logger, err);
		throw std::runtime_error(err);
	}

	// 1 pixel border
	nodes_.push_back(glm::ivec3(1, 1, width-2));

	size_t size = width * height * depth;
	data_ = new unsigned char [size];
	memset(data_, 0, size);
}

void TextureAtlas::write(const std::string & filename)
{
	boost::shared_ptr<v3D::Image> img;
	img.reset(new v3D::Image(width_, height_, depth_ * 8));
	size_t size = width_ * height_ * depth_;
	for (size_t i = 0; i < size; i++)
	{
		(*img)[i] = data_[i];
	}
	v3D::ImageFactory factory;
	factory.write(filename, img);
}

TextureAtlas::~TextureAtlas()
{
	if (id_ != 0)
	{
		glDeleteTextures(1, &id_);
	}
	delete [] data_;
}

unsigned int TextureAtlas::width() const
{
	return width_;
}

unsigned int TextureAtlas::height() const
{
	return height_;
}

unsigned int TextureAtlas::depth() const
{
	return depth_;
}

unsigned int TextureAtlas::id() const
{
	return id_;
}

glm::ivec4 TextureAtlas::region(unsigned int width, unsigned int height)
{
	glm::ivec3 node;
	int bestHeight = INT_MAX;
	int bestWidth = INT_MAX;
	int bestIndex = -1;
	glm::ivec4 region(0, 0, width, height);
	for (unsigned int i = 0; i < nodes_.size(); ++i)
	{
		int y = fit(i, width, height);
		if (y >= 0)
		{
			node = nodes_[i];
			if (((y + static_cast<int>(height)) < bestHeight) || 
				(((y + static_cast<int>(height)) == bestHeight) && (node.z < bestWidth)))
			{
				bestHeight = y + height;
				bestIndex = i;
				bestWidth = node.z;
				region.x = node.x;
				region.y = y;
			}
		}
	}

	if(bestIndex == -1)
	{
		region.x = -1;
		region.y = -1;
		region.z = 0;
		region.w = 0;
		return region;
    }

	node.x = region.x;
	node.y = region.y + height;
	node.z = width;
	nodes_.insert(nodes_.begin() + bestIndex, node);

	glm::ivec3 prev;
	for(unsigned int i = bestIndex + 1; i < nodes_.size(); ++i)
	{
		prev = nodes_[i-1];

		if (nodes_[i].x < (prev.x + prev.z))
		{
			int shrink = prev.x + prev.z - nodes_[i].x;
			nodes_[i].x += shrink;
			nodes_[i].z -= shrink;
			if (nodes_[i].z <= 0)
			{
				nodes_.erase(nodes_.begin() + i);
				--i;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	merge();
    used_ += width * height;
	return region;
}

int TextureAtlas::fit(unsigned int index, unsigned int width, unsigned int height)
{
	glm::ivec3 node = nodes_[index];
	int x = node.x;
	if ((x + width) > (width_ - 1))
	{
		return -1;
	}
	int y = node.y;
	int widthLeft = width;
	unsigned int i = index;
	while (widthLeft > 0)
	{
		node = nodes_[i];
		if (node.y > y)
		{
			y = node.y;
		}
		if ((y + height) > (height_ - 1))
		{
			return -1;
		}
		widthLeft -= node.z;
		++i;
	}
	return y;
}

void TextureAtlas::merge()
{
	for (unsigned int i = 0; i < nodes_.size() - 1; ++i)
	{
		if (nodes_[i].y == nodes_[i+1].y)
		{
			nodes_[i].z += nodes_[i+1].z;
			nodes_.erase(nodes_.begin() + (i+1));
			--i;
		}
	}
}

void TextureAtlas::region(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char * data, unsigned int stride)
{
	assert(x > 0);
	assert(y > 0);
	assert(x < (width_ - 1));
	assert((x + width) <= (width_ - 1));
	assert(y < (height_ - 1));
	assert((y + height) <= (height_ - 1));

	size_t charsize = sizeof(char);
	for (unsigned int i = 0; i < height; ++i)
	{
		memcpy(data_ + ((y + i) * width_ + x) * charsize * depth_, 
				data + (i * stride) * charsize, width * charsize * depth_);
	}
}

void TextureAtlas::upload()
{
	if (id_ == 0)
	{
		glGenTextures(1, &id_);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if(depth_ == 4)
	{
#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_,
					 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, data_);
#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, height_,
					 0, GL_RGBA, GL_UNSIGNED_BYTE, self->data);
#endif
	}
	else if(depth_ == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_,
					 0, GL_RGB, GL_UNSIGNED_BYTE, data_);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width_, height_,
					 0, GL_ALPHA, GL_UNSIGNED_BYTE, data_);
    }
}
