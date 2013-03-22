#include "Mesh.h"

Mesh::Mesh()
{
}

void Mesh::addTri(unsigned int a, unsigned int b, unsigned int c)
{
	tris_.push_back(glm::ivec3(a, b, c));
}

std::list<glm::ivec3> & Mesh::tris()
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

unsigned int Mesh::addVertex(const glm::vec3 & vertex, const glm::vec3 & normal)
{
	unsigned int index = findVertex(vertex, normal);
	if (index != -1)
	{
		return index;
	}
	vertices_.push_back(vertex);
	normals_.push_back(normal);
	return (vertices_.size() - 1);
}

void Mesh::addNormal(const glm::vec3 & normal)
{
	normals_.push_back(normal);
}

std::list<glm::vec3> & Mesh::normals()
{
	return normals_;
}

std::list<glm::vec3> & Mesh::vertices()
{
	return vertices_;
}

unsigned int Mesh::findVertex(const glm::vec3 & vertex)
{
	// for now just short circuit so new vertices are always added
	// the below vector searches are extremely slow for finding vertices
	return -1;

	std::list<glm::vec3>::iterator it;
	unsigned int index = 0;
	for (it = vertices_.begin(); it != vertices_.end(); it++)
	{
		if (*it == vertex)
		{
			return index;
		}
		index++;
	}

	return -1;
}

unsigned int Mesh::findVertex(const glm::vec3 & vertex, const glm::vec3 & normal)
{
	// for now just short circuit so new vertices are always added
	// the below vector searches are extremely slow for finding vertices
	return -1;

	std::list<glm::vec3>::iterator it = vertices_.begin();
	unsigned int index = 0;
	for (; it != vertices_.end(); it++)
	{
		if (*it == vertex)
		{
			std::list<glm::vec3>::iterator nit = normals_.begin();
			for (unsigned int i = 0; i < index; i++)
			{
				nit++;
			}
			if (*nit == normal)
			{
				return index;
			}
		}
		index++;
	}

	/*
	for (unsigned int i = 0; i < vertices_.size(); i++)
	{
		if (vertex == vertices_[i])
		{
			if (normal == normals_[i])
			{
				return i;
			}
		}
	}
	*/
	/*
	size_t size = vertices_.size();
	if (size == 0)
	{
		return -1;
	}
	glm::vec3 * it = &vertices_.front();
	for (unsigned int i = 0; i < size; i++)
	{
		if (*it == vertex)
		{
			if (normal == normals_[i])
			{
				return i;
			}

		}
		if (i < size-1)
		{
			it++;
		}
	}
	*/
	/*
	std::vector<glm::vec3>::iterator start = vertices_.begin();
	std::vector<glm::vec3>::iterator it;
	do
	{
		it = std::find(start, vertices_.end(), vertex);
		if (it != vertices_.end())
		{
			unsigned int index = it - vertices_.begin();
			if (normal == normals_[index])
			{
				return index;
			}
			start = ++it;
		}
	}
	while(it != vertices_.end());
	*/
	return -1;
}
