/*
 * BaseFace.h
 *
 *  Created on: Mar 5, 2012
 *      Author: phanindrabhagavatula
 */

#ifndef BASE2DSIMPLEX_H_
#define BASE2DSIMPLEX_H_
using namespace std;

class Base2dSimplex {
public:
	Base2dSimplex();
	virtual ~Base2dSimplex();
	deque<long>& getVertexList();
	long getId();
	long getNeighbour1();
	long getNeighbour2();

	void setId(long);
	void insertVertex(long);
	void setNeighbour1(long);
	void setNeighbour2(long);
protected:
	long neigh3dSimplex1, neigh3dSimplex2;
	deque<long> vertices;
	long iD;
};

#endif /* BASEFACE_H_ */
