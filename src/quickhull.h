 /*
 * quickhull.h
 *
 *  Created on: Sep 30, 2011
 *      Author: phanindrabhagavatula
 */

#ifndef QUICKHULL_H_
#define QUICKHULL_H_
//using namespace std;

namespace qhull {
class facet;
class ridge;

class vertex4d : public BaseVertex
{
public:
	vertex4d(double, double, double, double);
	bool isAbove(facet*);
	void setAssignedToBucket(bool);
	bool getBucketAssignmentStatus();
	deque<long>& getVisibleFacets();
	void insertVisibleFacet(long);
private:
	deque<long> visibleFacets;
	bool bucketStatus;
};
class ridge : public Base2dSimplex
{
public:
	ridge();
};
class facet : public Base3dSimplex
{
public:
	facet();
	double* getNormal();
	double 	getOffset();
	deque<long>& getBucketVertices();
	long	getFurthestPoint();
	void 	setNormal(double[4]);
	void	putInBucket(long);
	void 	calcNormOffset(const deque<vertex4d>::pointer*, double* );
	void	setFurthestPoint(long);

private:
	double normal[4];
	double offset;
	long furthestPoint;
	deque<long> bucket;
};

class convexHull4d
{
public:
	static convexHull4d* createHullObject();
	map<long, vertex4d>& getVertexList();
	map<long, ridge>& getRidgeList();
	map<long, facet>& getFacetList();
	void makeHull();
	void makeInitSimplex();
	void initBucketing();
	void insertFacet(facet);
	void insertVertex(vertex4d);
	void populateVerticesList(map<long,Vertex>);
	void insertTempFacet(long,facet);
private:
	convexHull4d(){}
	~convexHull4d();
	void finalCleaning();
	void setFurthestPoint(long);
	void setVisibleFacetsForVertex(long);
	void getHorizonRidges(deque<long>&, vertex4d*);
	void deleteDiscardedFacets(deque<long>&,vertex4d*, deque<long>&, deque<long>&);
	void createNewFacets(deque<long>&,vertex4d*, vector<long>&, deque<long>&, double[4]);
	void calcCentroid();
	long generateId(const vector<long>*);
	void printMap();
	double centroid[4];
	map<long,vertex4d> verticesOfHull;
	map<long,facet> tempFacets;
	map<long,ridge> tempRidges;
	map<string,long> idMap;
	static long idCounter;
	static bool objCreated;
};
}
#endif /* QUICKHULL_H_ */
