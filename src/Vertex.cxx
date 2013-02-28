#include "Vertex.h"

bool operator == (const Vertex & v1, const Vertex & v2)
{
	if (v1.position_[0] == v2.position_[0] &&
		v1.position_[1] == v2.position_[1] &&
		v1.position_[2] == v2.position_[2])
	{
		return true;
	}

	return false;
}
