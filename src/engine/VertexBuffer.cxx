/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "VertexBuffer.h"
#include "Mesh.h"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include <log4cxx/logger.h>

VertexBuffer::VertexBuffer(boost::shared_ptr<Mesh> & mesh) :
	ebo_(0),
	vbo_(0),
	type_(BUFFER_TYPE_STATIC)
{
	attribute(0, 3, ATTRIBUTE_TYPE_VERTEX, mesh->vertices().size());
	attribute(1, 3, ATTRIBUTE_TYPE_NORMAL, mesh->normals().size());
	allocate();

	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("voxel.log"));
	std::stringstream msg;

	// fill buffer with mesh data
	std::list<glm::vec3> vertexList = mesh->vertices();
	std::vector<glm::vec3> vertices;
	msg << "reserving " << vertexList.size() << " vertices ";
	vertices.reserve(vertexList.size());
	vertices.assign(vertexList.begin(), vertexList.end());
	data3f(0, vertices);

	std::list<glm::vec3> normalList = mesh->normals();
	std::vector<glm::vec3> normals;
	msg << "reserving " << normalList.size() << " normals ";
	normals.reserve(normalList.size());
	normals.assign(normalList.begin(), normalList.end());
	data3f(1, normals);

	std::list<glm::ivec3> triList = mesh->tris();
	std::vector<unsigned int> tris;
	msg << "reserving " << triList.size() << " tris " << std::endl;
	tris.reserve(triList.size() * 3);
	std::list<glm::ivec3>::iterator tit = triList.begin();
	for (; tit != triList.end(); tit++)
	{
		tris.push_back(tit->x);
		tris.push_back(tit->y);
		tris.push_back(tit->z);
	}
	indices(tris);

	LOG4CXX_INFO(logger, msg.str());
}


VertexBuffer::VertexBuffer() :
	ebo_(0),
	vbo_(0),
	type_(BUFFER_TYPE_DYNAMIC)
{
}

VertexBuffer::~VertexBuffer()
{
	if (ebo_ != 0)
	{
		glDeleteBuffers(1, &ebo_);
	}
	if (vbo_ != 0)
	{
		glDeleteBuffers(1, &vbo_);
	}
}

void VertexBuffer::attribute(unsigned int position, unsigned int size, AttributeType type, unsigned int length)
{
	VertexAttribute attr;
	attr.position_ = position;
	attr.size_ = size;
	attr.type_ = type;
	attr.length_ = length;
	attr.stride_ = sizeof(float) * size;

	size_t offset = 0;
	bool exists = false;
	unsigned int index;
	for (unsigned int i = 0; i < attributes_.size(); i++)
	{
		if (attributes_[i].position_ < position)
		{
			offset += attributes_[i].stride_ * attributes_[i].length_;
		}
		if (attributes_[i].position_ == position)
		{
			exists = true;
			index = i;
		}
	}
	attr.offset_ = offset;

	// if the attribute in the same poisition has previously been set then just replace the old version
	if (exists)
	{
		attributes_[index] = attr;
	}
	else
	{
		attributes_.push_back(attr);
	}
}

void VertexBuffer::indices(const std::vector<unsigned int> & data)
{
	indices_ = data.size();
	if (ebo_ == 0)
	{
		glGenBuffers(1, &ebo_);
	}
	GLenum usage;
	switch (type_)
	{
		case BUFFER_TYPE_STATIC:
			usage = GL_STATIC_DRAW;
			break;
		case BUFFER_TYPE_DYNAMIC:
			usage = GL_DYNAMIC_DRAW;
			break;
		default:
			usage = GL_STATIC_DRAW;
			break;
	}
	// NB - ebo's are bound to the vao so the vao must already be bound
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_ * sizeof(unsigned int), &data[0], usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexBuffer::allocate()
{
	if (vbo_ == 0)
	{
		glGenBuffers(1, &vbo_);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	size_t bufferSize = 0;
	for (unsigned int i = 0; i < attributes_.size(); i++)
	{
		bufferSize += attributes_[i].stride_ * attributes_[i].length_;
	}

	GLenum usage;
	switch (type_)
	{
		case BUFFER_TYPE_STATIC:
			usage = GL_STATIC_DRAW;
			break;
		case BUFFER_TYPE_DYNAMIC:
			usage = GL_DYNAMIC_DRAW;
			break;
		default:
			usage = GL_STATIC_DRAW;
			break;
	}

	// allocate buffer
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::data1f(unsigned int attr, const std::vector<float> & data)
{
	set(attr, &data[0], data.size());
}

void VertexBuffer::data2f(unsigned int attr, const std::vector<glm::vec2> & data)
{
	set(attr, glm::value_ptr(data[0]), data.size());
}

void VertexBuffer::data3f(unsigned int attr, const std::vector<glm::vec3> & data)
{
	set(attr, glm::value_ptr(data[0]), data.size());
}

void VertexBuffer::data4f(unsigned int attr, const std::vector<glm::vec4> & data)
{
	set(attr, glm::value_ptr(data[0]), data.size());
}

void VertexBuffer::set(unsigned int attr, const float * data, size_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	unsigned int offset = attributes_[attr].offset_;
	size_t byteSize = attributes_[attr].stride_ * size;
	glBufferSubData(GL_ARRAY_BUFFER, offset, byteSize, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::render()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	for (unsigned int i = 0; i < attributes_.size(); i++)
	{
		glEnableVertexAttribArray(attributes_[i].position_);
		glVertexAttribPointer(
			attributes_[i].position_, 
			attributes_[i].size_, 
			GL_FLOAT, 
			GL_FALSE, 
			attributes_[i].stride_, 
			reinterpret_cast<void*>(attributes_[i].offset_)
		);
	}

	glDrawElements(GL_TRIANGLES, indices_, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

	for (unsigned int i = 0; i < attributes_.size(); i++)
	{
		glDisableVertexAttribArray(attributes_[i].position_);
	}
}
