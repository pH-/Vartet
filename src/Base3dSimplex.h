/*
 * Base3dSimplex.h
 *
 *  Created on: Mar 5, 2012
 *      Author: phanindrabhagavatula
 */

#ifndef BASE3DSIMPLEX_H_
#define BASE3DSIMPLEX_H_
using namespace std;

class Base3dSimplex {
public:
	Base3dSimplex();
	virtual ~Base3dSimplex();
	deque<long>& getContaining2dSimplices();
	deque<long>& getVertexIndices();
	long 		 getId();

	void	insertVertex(long);
	void	setContaining2dSimplices(long[4]);
	void	insertContaining2dSimplex(long);
	void 	setId(long);
	void 	insertRidge(long);
protected:
	deque<long> vertexIndices;
	deque<long> contained2dSimplexIndices;
	long 		iD;
};

#endif /* BASE3DSIMPLEX_H_ */
