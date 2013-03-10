#include "Mesh.h"

Mesh::Mesh()
{
}

void Mesh::addTri(unsigned int a, unsigned int b, unsigned int c)
{
	tris_.push_back(a);
	tris_.push_back(b);
	tris_.push_back(c);
}

std::vector<unsigned int> & Mesh::tris()
{
	return tris_;
}

unsigned int Mesh::addVertex(const glm::vec3 & vertex)
{
	unsigned int index = findVertex(vertex);
	if (index != -1)
	{
		return index;
	}
	vertices_.push_back(vertex);
	return (vertices_.size() - 1);
}

void Mesh::addNormal(const glm::vec3 & normal)
{
	normals_.push_back(normal);
}

std::vector<glm::vec3> & Mesh::normals()
{
	return normals_;
}

std::vector<glm::vec3> & Mesh::vertices()
{
	return vertices_;
}

unsigned int Mesh::findVertex(const glm::vec3 & vertex)
{
	for (unsigned int i = 0; i < vertices_.size(); i++)
	{
		if (vertex == vertices_[i])
		{
			return i;
		}
	}
	return -1;
}
