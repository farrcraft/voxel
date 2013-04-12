/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "VertexBuffer.h"
#include "Mesh.h"
#include "../voxel/MeshCache.h"
#include "../voxel/Surface.h"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include <log4cxx/logger.h>

VertexBuffer::VertexBuffer(boost::shared_ptr<Mesh> & mesh) :
	ebo_(0),
	vbo_(0),
	type_(BUFFER_TYPE_STATIC)
{
	boost::unordered_map<unsigned int, glm::ivec4> faces = mesh->faces();
	unsigned int faceCount = faces.size();
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> info;
	std::vector<unsigned int> tris;
	tris.reserve(faceCount * 6);
	vertices.reserve(faceCount * 6);
	info.reserve(faceCount * 6);

	boost::unordered_map<unsigned int, glm::vec3> meshVertices = mesh->vertices();
	boost::unordered_map<unsigned int, glm::ivec3> meshTris = mesh->tris();
	glm::ivec3 leftTri;
	glm::ivec3 rightTri;
	unsigned int vertexCount = 0;
	for (unsigned int i = 0; i < faceCount; i++)
	{
		glm::ivec4 face = faces[i];
		leftTri = meshTris[face.x];
		rightTri = meshTris[face.y];
		vertices.push_back(meshVertices[leftTri.x]);
		vertices.push_back(meshVertices[leftTri.y]);
		vertices.push_back(meshVertices[leftTri.z]);

		vertices.push_back(meshVertices[rightTri.x]);
		vertices.push_back(meshVertices[rightTri.y]);
		vertices.push_back(meshVertices[rightTri.z]);

		info.push_back(glm::vec2(face.z, face.w));
		info.push_back(glm::vec2(face.z, face.w));
		info.push_back(glm::vec2(face.z, face.w));

		info.push_back(glm::vec2(face.z, face.w));
		info.push_back(glm::vec2(face.z, face.w));
		info.push_back(glm::vec2(face.z, face.w));

		tris.push_back(vertexCount + 0);
		tris.push_back(vertexCount + 1);
		tris.push_back(vertexCount + 2);

		tris.push_back(vertexCount + 3);
		tris.push_back(vertexCount + 4);
		tris.push_back(vertexCount + 5);
		vertexCount += 6;
	}

	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("voxel.log"));
	std::stringstream msg;
	msg << "allocated " << vertices.size() << " vertices ";
	LOG4CXX_INFO(logger, msg.str());

	attribute(0, 3, ATTRIBUTE_TYPE_VERTEX, vertices.size());
	attribute(1, 2, ATTRIBUTE_TYPE_GENERIC, info.size());
	allocate();
	data3f(0, vertices);
	data2f(1, info);
	indices(tris);
}


VertexBuffer::VertexBuffer(boost::shared_ptr<MeshCache> & mesh) :
	ebo_(0),
	vbo_(0),
	type_(BUFFER_TYPE_STATIC)
{
	glm::ivec4 * faces = mesh->faces();
	unsigned int faceCount = mesh->faceCount();
	std::vector<glm::vec2> info;
	info.reserve(faceCount * 6);

	glm::vec3 * meshVertices = mesh->vertices();
	glm::ivec3 * meshTris = mesh->tris();
	glm::ivec3 leftTri;
	glm::ivec3 rightTri;
	unsigned int vertexCount = mesh->vertexCount();
	for (unsigned int i = 0; i < faceCount; i++)
	{
		glm::ivec4 face = faces[i];

		leftTri = meshTris[face.x];
		rightTri = meshTris[face.y];
		info.push_back(glm::vec2(face.z, face.w));
		info.push_back(glm::vec2(face.z, face.w));
		info.push_back(glm::vec2(face.z, face.w));

		info.push_back(glm::vec2(face.z, face.w));
		info.push_back(glm::vec2(face.z, face.w));
		info.push_back(glm::vec2(face.z, face.w));
	}
	attribute(0, 3, ATTRIBUTE_TYPE_VERTEX, vertexCount);
	attribute(1, 2, ATTRIBUTE_TYPE_GENERIC, info.size());

	allocate();
	set(0, glm::value_ptr(meshVertices[0]), vertexCount);
	data2f(1, info);
	indices(meshTris, mesh->triCount() * 3);
}


VertexBuffer::VertexBuffer(boost::shared_ptr<Surface> & surface) :
	ebo_(0),
	vbo_(0),
	type_(BUFFER_TYPE_STATIC)
{
	boost::unordered_map<unsigned int, glm::vec3> surfaceVertices = surface->vertices();
	boost::unordered_map<unsigned int, glm::ivec3> surfaceTris = surface->tris();
	std::list<glm::ivec4> faces = surface->faces();

	std::vector<unsigned int> tris;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> info;

	// two tris per face & 4 unique vertices per face
	unsigned int triCount = faces.size() * 2;
	// extract compacted vertices when passing to GPU
	unsigned int vertexCount = triCount * 3;

	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("voxel.log"));
	std::stringstream msg;
	msg << "reserving " << vertexCount << " vertices ";
	msg << "reserving " << triCount << " tris " << std::endl;
	LOG4CXX_INFO(logger, msg.str());

	// allocate vector space in advance
	tris.reserve(triCount);
	vertices.reserve(vertexCount);
	info.reserve(vertexCount);

	glm::ivec3 leftTri;
	glm::ivec3 rightTri;
	unsigned int faceType = 0;
	unsigned int blockType = 0;

	unsigned int firstVertex;

	for (std::list<glm::ivec4>::iterator face = faces.begin();
		 face != faces.end();
		 face++)
	{
		leftTri = surfaceTris[(*face).x];
		rightTri = surfaceTris[(*face).y];
		faceType = (*face).z;
		blockType = (*face).w;

		// index of first vertex on this tri
		firstVertex = vertices.size();

		// add vertex data for first tri
		for (unsigned int leftIndex = 0; leftIndex < 3; leftIndex++)
		{
			vertices.push_back(surfaceVertices[leftTri[leftIndex]]);
			info.push_back(glm::vec2(faceType, blockType));
			tris.push_back(firstVertex + leftIndex);
		}

		// vertex data for second tri
		firstVertex = vertices.size();
		for (unsigned int rightIndex = 0; rightIndex < 3; rightIndex++)
		{
			vertices.push_back(surfaceVertices[rightTri[rightIndex]]);
			info.push_back(glm::vec2(faceType, blockType));
			tris.push_back(firstVertex + rightIndex);
			/* can't reuse vertices at this stage...
			// reuse vertex indices from the first tri for shared vertices
			bool sharedVertex = false;
			for (unsigned int leftIndex = 0; leftIndex < 3; leftIndex++)
			{
				if (rightTri[rightIndex] == leftTri[leftIndex])
				{
					tris.push_back(firstVertex + leftIndex);
					sharedVertex = true;
					break;
				}
			}
			if (!sharedVertex)
			{
				vertices.push_back(surfaceVertices[rightTri[rightIndex]]);
				info.push_back(glm::vec2(faceType, blockType));
				tris.push_back(firstVertex + 3);
			}
			*/
		}
	}

	attribute(0, 3, ATTRIBUTE_TYPE_VERTEX, vertices.size());
	attribute(1, 2, ATTRIBUTE_TYPE_GENERIC, info.size());
	allocate();

	data3f(0, vertices);
	data2f(1, info);
	indices(tris);
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

void VertexBuffer::indices(glm::ivec3 * data, size_t count)
{
	indices_ = count;
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_ * sizeof(unsigned int), &(data[0].x), usage);
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
