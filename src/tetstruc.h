/*
 * structure.h
 *
 *  Created on: Jun 6, 2011
 *      Author: phanindrabhagavatula
 */

#ifndef STRUCTURE_H_
#define STRUCTURE_H_

using namespace std;
class Face;
class Solid;
class Cell;

class Coordinates	{
public:
	Coordinates(double, double, double);
	void setCoords(double, double, double);
	double* getCoords();
	double getXCoord();
	double getYCoord();
	double getZCoord();
private:
	double coords[3];
};

class Vertex {
public:
	Vertex(double, double, double,bool);
	Vertex():vertexCoord(NULL),boundaryMarker(false),numOfOpenFaces(-1),oppositeFace(NULL){};
	double getXCoord();
	double getYCoord();
	double getZCoord();
	double getCoord(axisToSort);
	double getSqDistance(Vertex&);
	bool   sameAs(Vertex&);
	void   setOppFace(deque<Face>::pointer);
	void   incrNumOfOpenFaces();
	void	*sparePtr;						// a pointer to be used for temporary extension of a vertex object.
private:
	Coordinates *vertexCoord;
	bool		boundaryMarker;
	int			numOfOpenFaces;
	deque<Face>::pointer oppositeFace;
};

class Hole {
public:
	Hole(double,double,double);
private:
	Coordinates *holeCoords;
};

class Edge {
public:
	Edge(deque<Vertex>::pointer, deque<Vertex>::pointer);
	Edge(){};
	void setVertices(deque<Vertex>::pointer, deque<Vertex>::pointer);
	Vertex** getVertex();
	bool testCircumCircle(Vertex&, double[4]);
private:
	vector<Vertex>::pointer vertices[2];
};

class PlcEdge {
public:
	void setVertexPtrs(Vertex*, Vertex*);
	PlcEdge(Vertex*,Vertex*);
private:
	Vertex		*vertices[2];
};

class Face	{
public:
	Face();
	void addVertices(deque<Vertex>::pointer, deque<Vertex>::pointer, deque<Vertex>::pointer);
	void addEdges(deque<Edge>::pointer, deque<Edge>::pointer, deque<Edge>::pointer);
	void addOppositeVertex(deque<Vertex>::pointer);
	void addNormalDir(double[3]);
	bool testCircumSphere(Vertex&,double[4]);
	deque<Vertex>::pointer* getVertices();
	deque<Edge>::pointer* getEdges();
	void setParentCell(Cell*);
	Cell* getParentCell();
	void incrNumOfOpenFaces(deque<deque<deque<deque<deque<Vertex>::pointer > > > >&);
	deque<Vertex>::pointer getOppositeVertex();
	int 		id;
private:
	deque<Vertex>::pointer	vertices[3];
	deque<Edge>::pointer	edges[3];
	deque<Vertex>::pointer	oppositeVertex;
	deque<Cell>::pointer    parentCell;
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

class Cell	{
public:
	Cell();
	Cell(int);
	void addVertex(Vertex&);
	void addFace(Face&);
	void addNeighbour(deque<Cell>::pointer, deque<Face>::pointer);
	void addEdge(Edge&);
	size_t getVertexListSize();
	size_t getFaceListSize();
	size_t getNeighListSize();
	size_t getEdgeListSize();
	deque<Vertex>& getVertices();
	deque<Edge>& getEdges();
	deque<Face>& getFaces();
	multimap<deque<Cell>::pointer, deque<Face>::pointer>& getNeighbours();
	bool testCircumCircle(Vertex&,double[4]);
	bool testCircumSphere(Vertex&,double[4]);
	void addFEVs(Face&);
	bool checkOrientation(Vertex&);

private:
	deque<Vertex> vertices;
	deque<Face>	faces;
	multimap<deque<Cell>::pointer, deque<Face>::pointer> neighbours;
	deque<Edge>	edges;
};

class Solid	{
public:
	void populateVertices(deque<Vertex>&);
	void delaunize();
	//void sortVertices(axisToSort, deque<Vertex>, deque<Vertex>);
	void dewall(axisToSort,deque<Vertex>&,map<Face,int>&, deque<deque<deque<deque<deque<Vertex>::pointer > > > >&);
	void *sparePtr;                                          // a pointer to be used for temporary extension of a Solid object.
	void makeCell(deque<Cell>::reference, deque<deque<deque<deque<deque<Vertex>::pointer > > > >&, double);
	void drawEdges();
	int  listOfCellsSize();
protected:
	void confirmWinnerVertex(deque<Cell>::reference, Vertex*&, double[4],deque<deque<deque<deque<deque<Vertex>::pointer > > > >&);
	deque<Cell> listOfCells;
	deque<Face>	listOfFaces;
	deque<Edge>	listOfEdges;
	deque<Vertex>	listOfVertices;
	deque<Vertex>	verticesXsort;
	deque<Vertex>	verticesYsort;
	deque<Vertex>	verticesZsort;
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
