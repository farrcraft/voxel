#include "Mesh.h"

Mesh::Mesh()
{
}

unsigned int Mesh::addVertex(const Vertex & vertex)
{
	unsigned int index = findVertex(vertex);
	if (index != -1)
	{
		return index;
	}
	vertices_.push_back(vertex);
	return (vertices_.size() - 1);
}


void Mesh::addTri(unsigned int a, unsigned int b, unsigned int c)
{
	tris_.push_back(a);
	tris_.push_back(b);
	tris_.push_back(c);
}

std::vector<Vertex> & Mesh::vertices()
{
	return vertices_;
}

std::vector<unsigned int> & Mesh::tris()
{
	return tris_;
}

void Mesh::addBuffer(const std::string & name, unsigned int id)
{
	buffers_[name] = id;
}

unsigned int Mesh::buffer(const std::string & name)
{
	if (buffers_.find(name) == buffers_.end())
	{
		return -1;
	}
	return buffers_[name];
}

unsigned int Mesh::findVertex(const Vertex & vertex)
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
