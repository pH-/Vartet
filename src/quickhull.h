 /*
 * quickhull.h
 *
 *  Created on: Sep 30, 2011
 *      Author: phanindrabhagavatula
 */

#ifndef QUICKHULL_H_
#define QUICKHULL_H_
using namespace std;

namespace qhull {
class facet;
class ridge;

class vertex4d
{
public:
	vertex4d(double, double, double, double);
	double* getCoords();
	bool isAbove(facet*);
	void setAssignedToBucket(bool);
	bool getBucketAssignmentStatus();
	deque<long>& getVisibleFacets();
	long getId();
	void insertVisibleFacet(long);
	void setVertexId(long);
private:
	double coords4d[4];
	deque<long> visibleFacets;
	bool bucketStatus;
	long vertexId;
};
class ridge
{
public:
	ridge();
	long getNeighbour1();
	long getNeighbour2();
	deque<long>& getVertexList();
	long getId();
	void insertVertex(long);
	void setNeighbour1(long);
	void setNeighbour2(long);
	void setId(long);
private:
	deque<long> ridgeVertices;
	long neighFacet1, neighFacet2;
	long ridgeId;
};
class facet
{
public:
	facet();
	double* getNormal();
	double 	getOffset();
	deque<long>& getBucketVertices();
	deque<long>& getFacetRidges();
	deque<long>& getVertexIndices();
	long 	getId();
	long	getFurthestPoint();
	void 	setNormal(double[4]);
	void	putInBucket(long);
	void	insertRidge(long);
	void 	calcNormOffset(const deque<vertex4d>::pointer*, double* );
	void	setRidges(long[4]);
	void 	setId(long);
	void	setFurthestPoint(long);

private:
	double normal[4];
	double offset;
	long furthestPoint;
	long id;
	deque<long> vertexIndices;
	deque<long> facetRidges;
	deque<long> bucket;
};

class convexHull4d
{
public:
	convexHull4d(){idCounter=0;}
	map<long, vertex4d>& getVertexList();
	map<long, ridge>& getRidgeList();
	map<long, facet>& getFacetList();
	void makeHull();
	void makeInitSimplex();
	void initBucketing();
	void insertFacet(facet);
	void insertVertex(vertex4d);
	void populateVerticesList(deque<Vertex>);
	void insertTempFacet(long,facet);
private:
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
	long idCounter;
};
}
#endif /* QUICKHULL_H_ */
