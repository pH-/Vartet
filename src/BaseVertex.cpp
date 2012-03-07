/*
 * BaseVertex.cpp
 *
 *  Created on: Mar 5, 2012
 *      Author: phanindrabhagavatula
 */

#include "BaseVertex.h"

BaseVertex::BaseVertex() {
	// TODO Auto-generated constructor stub

}

BaseVertex::~BaseVertex() {
	// TODO Auto-generated destructor stub
}

double* BaseVertex::getCoords()
{
	return coords;
}

long BaseVertex::getId()
{
	return vertexId;
}

void BaseVertex::setVertexId(long id)
{
	vertexId = id;
}
