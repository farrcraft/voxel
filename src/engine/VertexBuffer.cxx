#include "VertexBuffer.h"
#include "Mesh.h"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(boost::shared_ptr<Mesh> mesh) :
	ebo_(0),
	vbo_(0)
{

	attribute(0, 3, ATTRIBUTE_TYPE_VERTEX, mesh->vertices().size());
	attribute(1, 3, ATTRIBUTE_TYPE_NORMAL, mesh->normals().size());
	allocate();

	// fill buffer with mesh data
	data3f(0, mesh->vertices());
	data3f(1, mesh->normals());

	indices(mesh->tris());
}


VertexBuffer::VertexBuffer() :
	ebo_(0),
	vbo_(0)
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
	for (unsigned int i = 0; i < position; i++)
	{
		offset += attributes_[i].stride_ * attributes_[i].length_;
	}
	attr.offset_ = offset;

	attributes_.push_back(attr);
}

void VertexBuffer::indices(const std::vector<unsigned int> & data)
{
	indices_ = data.size();
	glGenBuffers(1, &ebo_);
	// NB - ebo's are bound to the vao so the vao must already be bound
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_ * sizeof(unsigned int), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexBuffer::allocate()
{
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	size_t bufferSize = 0;
	for (unsigned int i = 0; i < attributes_.size(); i++)
	{
		bufferSize += attributes_[i].stride_ * attributes_[i].length_;
	}

	// allocate buffer
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::data1f(unsigned int attr, const std::vector<float> & data)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	unsigned int offset = attributes_[attr].offset_;
	size_t size = attributes_[attr].stride_ * data.size();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, &data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::data2f(unsigned int attr, const std::vector<glm::vec2> & data)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	unsigned int offset = attributes_[attr].offset_;
	size_t size = attributes_[attr].stride_ * data.size();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, &data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::data3f(unsigned int attr, const std::vector<glm::vec3> & data)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	unsigned int offset = attributes_[attr].offset_;
	size_t size = attributes_[attr].stride_ * data.size();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, &data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::data4f(unsigned int attr, const std::vector<glm::vec4> & data)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	unsigned int offset = attributes_[attr].offset_;
	size_t size = attributes_[attr].stride_ * data.size();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, &data[0]);
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
