/*
 * Base3dSimplex.cpp
 *
 *  Created on: Mar 5, 2012
 *      Author: phanindrabhagavatula
 */

#include "includes.h"

Base3dSimplex::Base3dSimplex() {
	// TODO Auto-generated constructor stub

}

Base3dSimplex::~Base3dSimplex() {
	// TODO Auto-generated destructor stub
}

deque<long>& Base3dSimplex::getContaining2dSimplices()
{
	return contained2dSimplexIndices;
}
deque<long>& Base3dSimplex::getVertexIndices()
{
	return vertexIndices;
}

long Base3dSimplex::getId()
{
	return iD;
}

void Base3dSimplex::insertContaining2dSimplex(long r)
{
	contained2dSimplexIndices.push_back(r);
}
void Base3dSimplex::insertVertex(long v)
{
	vertexIndices.push_back(v);
}
void Base3dSimplex::setId(long idValue)
{
	iD=idValue;
}
