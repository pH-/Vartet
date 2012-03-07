/*
 * BaseVertex.h
 *
 *  Created on: Mar 5, 2012
 *      Author: phanindrabhagavatula
 */

#ifndef BASEVERTEX_H_
#define BASEVERTEX_H_

class BaseVertex {
public:
	BaseVertex();
	virtual ~BaseVertex();
	double* getCoords();
	long getId();
	void setVertexId(long);
protected:
	double coords[4];
	long vertexId;
};

#endif /* BASEVERTEX_H_ */
