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
//	void   setOppFace(deque<Face>::pointer);
	void   incrNumOfOpenFaces();
	void   decrNumOfOpenFaces();
	int    getNumOfOpenFaces();
	void   setId(int id);
	int    getId();
	void	*sparePtr;						// a pointer to be used for temporary extension of a vertex object.
private:
	Coordinates *vertexCoord;
	bool		boundaryMarker;
	int			numOfOpenFaces;
	deque<Face>::pointer oppositeFace;
	int         id;
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
	void setId(string);
	string getId();
	void addVertices(deque<Vertex>::pointer, deque<Vertex>::pointer, deque<Vertex>::pointer);
	void addEdges(deque<Edge>::pointer, deque<Edge>::pointer, deque<Edge>::pointer);
	void addOppositeVertex(deque<Vertex>::pointer);
	void addNormalDir(double[3]);
	bool testCircumSphere(Vertex&,double[4]);
	deque<Vertex>::pointer* getVertices();
	deque<Edge>::pointer* getEdges();
	void setNeighbourCell(deque<Cell>::pointer);
	void setNeighCell1(deque<Cell>::pointer);
	void setNeighCell2(deque<Cell>::pointer);
	Cell* getNeighCell1();
	Cell* getNeighCell2();
	void incrNumOfOpenFaces();
	void decrNumOfOpenFaces();
	int getNumOfOpenFaces();
	deque<Vertex>::pointer getOppositeVertex1();
	deque<Vertex>::pointer getOppositeVertex2();
private:
	deque<Vertex>::pointer	vertices[3];
	deque<Edge>::pointer	edges[3];
	deque<Vertex>::pointer	oppositeVertex1;
	deque<Vertex>::pointer  oppositeVertex2;
	deque<Cell>::pointer    cell1;
	deque<Cell>::pointer    cell2;
	string					id;
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
	void setId(string);
	string getId();
	void addVertex(deque<Vertex>::pointer);
	void addFace(deque<Face>::pointer);
	//void addNeighbour(deque<Cell>::pointer, deque<Face>::pointer);
	void addEdge(deque<Edge>::pointer);
	size_t getVertexListSize();
	size_t getFaceListSize();
	size_t getNeighListSize();
	size_t getEdgeListSize();
	deque<deque<Vertex>::pointer>& getVertices();
	deque<deque<Edge>::pointer>& getEdges();
	deque<deque<Face>::pointer>& getFaces();
	//multimap<deque<Cell>::pointer, deque<Face>::pointer>& getNeighbours();
	bool testCircumCircle(Vertex&,double[4]);
	trippleBool testCircumSphere(Vertex&,double[4]);
	void addFEVs(deque<Face>::pointer);
	void delFEVs(deque<Face>::pointer);
	bool checkOrientation(Vertex&);
	void setCircumCenter(double[3]);
	void setCircumRadius(double);
	double getCircumRadius();
	double* getCircumCenter();
	void delVertices();
private:
	string 						  id;
	deque<deque<Vertex>::pointer> vertices;
	deque<deque<Edge>::pointer>   edges;
	deque<deque<Face>::pointer>   faces;
	double						  circumCtr[3];
	double						  circumRadius;
	//multimap<deque<Cell>::pointer, deque<Face>::pointer> neighbours;
};

class Solid	{
public:
	void populateVertices(deque<Vertex>&);
	void delaunize();
	//void sortVertices(axisToSort, deque<Vertex>, deque<Vertex>);
	void dewall(axisToSort,deque<Vertex>&,map<string,deque<Face>::pointer>&, deque<deque<deque<deque<deque<Vertex>::pointer > > > >&);
	void *sparePtr;                                          // a pointer to be used for temporary extension of a Solid object.
	bool makeCell(deque<Cell>::reference, deque<deque<deque<deque<deque<Vertex>::pointer > > > >&, double);
	void drawEdges();
	int  listOfCellsSize();
protected:
	void confirmWinnerVertex(deque<Cell>::reference, Vertex*&,deque<deque<deque<deque<deque<Vertex>::pointer > > > >&);
	deque<Cell> listOfCells;
	deque<Face>	listOfFaces;
	deque<Edge>	listOfEdges;
	map<string,deque<Face>::pointer> faceMap;
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
