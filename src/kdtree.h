/*
 * kdtree.h
 *
 *  Created on: Jul 21, 2011
 *      Author: phanindrabhagavatula
 */

#ifndef KDTREE_H_
#define KDTREE_H_

using namespace std;
class leafNode;

class treeNode
{
public:
	treeNode():leftChild(NULL),rightChild(NULL),keyVal(0.0),dummy(false){};
	treeNode* getLeftChild();
	treeNode* getRightChild();
	double 	  getKeyValue();
	deque<Vertex>::pointer	getPtObject();
	int		getaxis();
	unsigned int     getsubTreeSize();
	void    setsubtreeSize(unsigned int);
	void	setaxis(int);
	void 	setLeftChild(treeNode*);
	void    setRightChild(treeNode*);
	void    setKeyValue(double);
	void    setRegion(double[2][3]);
	void    setDummy();
	void    attachPtObject(deque<Vertex>::pointer);
	bool    isLeaf();
	bool    isDummy();
	trippleBool    testRegion(double[2][3]);
private:
	treeNode *leftChild;
	treeNode *rightChild;
//	kdTree	 *treeObjPtr;
	unsigned int		  subTreeSize;
	int       axis;
	double    keyVal;
	double    regionLow[3], regionHigh[3];
	deque<Vertex>::pointer pointObj;
	bool      dummy;
};

class kdtree
{
public:
	kdtree():root(NULL){};
	treeNode* getRoot();
	void	  setRoot(treeNode*);
	void      buildTree(deque<Vertex>&);
//	void 	  insertNode(treeNode*);
	void 	  searchNodes(deque<deque<Vertex>::pointer >&, treeNode*, double, double[3]);
	void 	  searchNodes(deque<deque<Vertex>::pointer >&, treeNode*, double[2][3]);
	void	  reportSubTree(deque<deque<Vertex>::pointer >&, treeNode*);
	treeNode* getTreeMin();
	treeNode* getTreeMax();
	deque<Vertex>::pointer nearestNeighbour(deque<Vertex>::pointer);
	deque<Vertex>::pointer nearestNeighbour(deque<Vertex>::pointer, double);
private:
	treeNode*	buildTreeInternal(deque<deque<Vertex>::pointer >& , int, double[2][3]);
	void 		sortList(deque<deque<Vertex>::pointer >& , int);
	treeNode* nearestNeighbourInternal(treeNode*, deque<Vertex>::pointer, double&);
	treeNode *root;
	deque<deque<Vertex>::pointer > listOfVertices;
};

#endif /* KDTREE_H_ */

