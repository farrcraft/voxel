/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class MeshCache;

/**
 * A class for GPU buffer data
 */
class VertexBuffer
{
	public:
		typedef enum
		{
			ATTRIBUTE_TYPE_COLOR,
			ATTRIBUTE_TYPE_NORMAL,
			ATTRIBUTE_TYPE_VERTEX,
			ATTRIBUTE_TYPE_TEXTURE,
			ATTRIBUTE_TYPE_GENERIC
		} AttributeType;

		typedef struct
		{
			AttributeType type_;
			unsigned int position_;
			unsigned int size_; // components in the attribute - 1-4
			unsigned int stride_;
			unsigned int length_;
			unsigned int offset_;
		} VertexAttribute;

		typedef enum
		{
			BUFFER_TYPE_STATIC,
			BUFFER_TYPE_DYNAMIC
		} BufferType;

		VertexBuffer(boost::shared_ptr<MeshCache> & mesh);
		VertexBuffer();
		~VertexBuffer();

		void attribute(unsigned int position, unsigned int size, AttributeType type, unsigned int length);

		void data1f(unsigned int attr, const std::vector<float> & data);
		void data2f(unsigned int attr, const std::vector<glm::vec2> & data);
		void data3f(unsigned int attr, const std::vector<glm::vec3> & data);
		void data4f(unsigned int attr, const std::vector<glm::vec4> & data);

		void render();

		void indices(const std::vector<unsigned int> & data);
		void indices(glm::ivec3 * data, size_t count);
		void allocate();

	protected:
		void set(unsigned int attr, const float * data, size_t size);

	private:
		unsigned int ebo_;
		unsigned int vbo_;
		std::vector<VertexAttribute> attributes_;
		unsigned int indices_;
		BufferType type_;
};