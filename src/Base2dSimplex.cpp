/*
 * BaseFace.cpp
 *
 *  Created on: Mar 5, 2012
 *      Author: phanindrabhagavatula
 */

#include "includes.h"

Base2dSimplex::Base2dSimplex() {
	// TODO Auto-generated constructor stub

}

Base2dSimplex::~Base2dSimplex() {
	// TODO Auto-generated destructor stub
}

deque<long>& Base2dSimplex::getVertexList()
{
	return vertices;
}

long Base2dSimplex::getId()
{
	return iD;
}

void Base2dSimplex::insertVertex(long v)
{
	vertices.push_back(v);
}

void Base2dSimplex::setId(long idValue)
{
	iD = idValue;
}

long Base2dSimplex::getNeighbour1()
{
	return neigh3dSimplex1;
}

long Base2dSimplex::getNeighbour2()
{
	return neigh3dSimplex2;
}
void Base2dSimplex::setNeighbour1(long f)
{
	neigh3dSimplex1 = f;
}

void Base2dSimplex::setNeighbour2(long f)
{
	neigh3dSimplex2 = f;
}
