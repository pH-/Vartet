/*
 * structure.h
 *
 *  Created on: Jun 6, 2011
 *      Author: phanindrabhagavatula
 */

#ifndef STRUCTURE_H_
#define STRUCTURE_H_
namespace qhull{class convexHull4d;}

using namespace std;
class Face;
class Solid;
class Cell;
class kdtree;

class Vertex : public BaseVertex
{
public:
	Vertex(double, double, double);
	double getXCoord();
	double getYCoord();
	double getZCoord();
	double getCoord(axisToSort);
	double getSqDistance(Vertex&);
	double getSqDistance(double[3]);
	void	*sparePtr;						// a pointer to be used for temporary extension of a vertex object.
private:
	deque<Face>::pointer oppositeFace;
};

class Hole {
public:
	Hole(double,double,double);
private:
	double holeCoords[3];
};

class Edge {
public:
	Edge(long,long);
	Edge(){};
	void insertVertex(long);
	deque<long>& getVertices();
private:
	deque<long> vertices;
};

class PlcEdge {
public:
	void setVertexPtrs(Vertex*, Vertex*);
	PlcEdge(Vertex*,Vertex*);
private:
	Vertex		*vertices[2];
};

class Face : public Base2dSimplex
{
public:
	Face();
	Face(Base2dSimplex*);
	Face(Face* obj);
	double* getNormal();
	deque<long>& getEdges();

	void addEdges(long,long, long);
	void addNormalDir(double[3]);
private:
	deque<long> 	edges;
	double      normDir[3];
};

class PlcPolygon {
public:
	PlcPolygon(deque<int>&,deque<Vertex>&);
private:
	deque<PlcEdge>	plcEdgeList;
	deque<Hole> 	plcHoleList;
};

class PlcFacet {
public:
	void setNewPolygon(deque<int>&,deque<Vertex>&);
private:
	deque<PlcPolygon> 	plcPolygonList;
};

class PlcHole {
public:
private:
	Hole		**VolumeHoleList;
};

class Cell : public Base3dSimplex
{
public:
	Cell();
	Cell(Base3dSimplex*);
	Cell(Cell* obj);

	void 		 addEdge(long);
	deque<long>& getEdges();
	double 		 getCircumRadius();
	double* 	 getCircumCenter();
	int          getFaceNormalDirectionSign(long);

	void 		 setCircumCenter(double[3]);
	void 		 setCircumRadius(double);
	void 		 setDihedralAngles(Face*, Face*, Face*, Face*);
	void 		 setFaceNormalDirectionSign(long,int);

private:
	deque<long>   		edges;
	double				circumCtr[3];
	double				circumRadius;
	map<string,double> 	dihedralAngles;
	map<long,int> 		faceNormalDirectionSign;
};

class Solid	{
public:
//	void populateVertices(deque<Vertex>&);
	void populateVerticesRandom();
	void delaunize(qhull::convexHull4d*);
	void createSolid(qhull::convexHull4d*);
	void drawEdges();
	int  listOfCellsSize();
private:
	bool 			 testNormalDir(Face*,Vertex*);
	map<long,Vertex> listOfVertices;
	map<long,Edge> 	 listOfEdges;
	map<long,Face>   listOfFaces;
	map<long,Cell>   listOfCells;

};

class Plc {
public:
	deque<Vertex>& getPlcVertices();
	void parsePolyFile(char*);
	void addPlcVertex();
	void addPlcFacet();
	void addPlcEdge();

private:
	void parseTypeData(stringstream&, ifstream&, int);
	deque<PlcFacet> listOfFacets;
	deque<PlcEdge>  listOfEdges;
	deque<Hole>		listOfHoles;
	deque<Vertex>   listOfVertices;
};
#endif /* STRUCTURE_H_ */
