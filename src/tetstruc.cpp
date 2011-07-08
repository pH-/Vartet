/*
 * tetstruc.cpp
 *
 *  Created on: Jun 12, 2011
 *      Author: phanindrabhagavatula
 */

#include "includes.h"
using namespace std;
extern int firstCell;
extern int lastCell;
extern int faceToShow;
int counter =0;

///Global Functions

bool sortVertexX(Vertex v1, Vertex v2) { return (v1.getXCoord()<v2.getXCoord());}
bool sortVertexY(Vertex v1, Vertex v2) { return (v1.getYCoord()<v2.getYCoord());}
bool sortVertexZ(Vertex v1, Vertex v2) { return (v1.getZCoord()<v2.getZCoord());}

/// Coordinates Class
Coordinates::Coordinates(double x, double y, double z)
{
	coords[0]=x;
	coords[1]=y;
	coords[2]=z;
}

void Coordinates::setCoords(double x, double y, double z)
{
	coords[0]=x;
	coords[1]=y;
	coords[2]=z;
}

double Coordinates::getXCoord()
{
	return coords[0];
}

double Coordinates::getYCoord()
{
	return coords[1];
}

double Coordinates::getZCoord()
{
	return coords[2];
}

/// Vertex Class

Vertex::Vertex(double x, double y, double z, bool bMarker)
{
	vertexCoord = new Coordinates(x,y,z);
	boundaryMarker = bMarker;
	oppositeFace = NULL;
	numOfOpenFaces = -1;
}

double Vertex::getCoord(axisToSort axis)
{
	if(axis==X)
		return vertexCoord->getXCoord();
	else if(axis==Y)
		return vertexCoord->getYCoord();
	else
		return vertexCoord->getZCoord();
}

double Vertex::getXCoord()
{
	return vertexCoord->getXCoord();
}

double Vertex::getYCoord()
{
	return vertexCoord->getYCoord();
}

double Vertex::getZCoord()
{
	return vertexCoord->getZCoord();
}

double Vertex::getSqDistance(Vertex& v2)
{
	return pow((getXCoord()-v2.getXCoord()),2) + pow((getYCoord()-v2.getYCoord()),2) +pow((getZCoord()-v2.getZCoord()),2);
}

bool Vertex::sameAs(Vertex& v)
{
	return(getXCoord()==v.getXCoord() && getYCoord()==v.getYCoord() && getZCoord()==v.getZCoord());
}

//void Vertex::setOppFace(deque<Face>::pointer face)
//{
//	oppositeFace = face;
//}

void Vertex::incrNumOfOpenFaces()
{
	if(numOfOpenFaces<0)
		numOfOpenFaces=1;
	else
		numOfOpenFaces++;
}

void Vertex::setId(int idNum)
{
	id=idNum;
}

int Vertex::getId()
{
	return id;
}
///Plc Hole class

Hole::Hole(double x, double y, double z)
{
	holeCoords = new Coordinates(x,y,z);
}

///Edge class

Edge::Edge(deque<Vertex>::pointer v1, deque<Vertex>::pointer v2)
{
	vertices[0]=v1;
	vertices[1]=v2;
}

void Edge::setVertices(deque<Vertex>::pointer v1, deque<Vertex>::pointer v2)
{
	vertices[0]=v1;
	vertices[1]=v2;
}

Vertex** Edge::getVertex()
{
	return vertices;
}

bool Edge::testCircumCircle(Vertex& v, double centerRadius[4])
{
	double sideaSq, sidebSq, sidecSq;
	if(vertices[0]->sameAs(v))
		return false;
	if(vertices[0]->sameAs(v))
		return false;
	sideaSq = vertices[1]->getSqDistance(v);
	sidebSq = vertices[0]->getSqDistance(v);
	sidecSq = vertices[0]->getSqDistance(*vertices[1]);
	for(int i=0; i<3; i++)
		centerRadius[i]=((vertices[0]->getCoord((axisToSort)i)*(sideaSq*(sidebSq+sidecSq-sideaSq)))+
				         (vertices[1]->getCoord((axisToSort)i)*(sidebSq*(sidecSq+sideaSq-sidebSq)))+
				         (v.getCoord((axisToSort)i)			  *(sidecSq*(sideaSq+sidebSq-sidecSq))))
				        /(2*(sideaSq*sidebSq+sidebSq*sidecSq+sidecSq*sideaSq)-(sideaSq*sideaSq+sidebSq*sidebSq+sidecSq*sidecSq));
	centerRadius[3]=sqrt(pow((v.getXCoord()-centerRadius[0]),2) + pow((v.getYCoord()-centerRadius[1]),2) +pow((v.getZCoord()-centerRadius[2]),2));
	return true;
}
///Plc Edge class

PlcEdge::PlcEdge(Vertex *vertex1, Vertex *vertex2)
{
	vertices[0]=vertex1;
	vertices[1]=vertex2;
}

///Face class

void Face::setId(string vid)
{
	id.assign(vid);
}

string Face::getId()
{
	return id;
}
void Face::addVertices(deque<Vertex>::pointer v1, deque<Vertex>::pointer v2, deque<Vertex>::pointer v3)
{
	vertices[0]=v1;
	vertices[1]=v2;
	vertices[2]=v3;
}

void Face::addEdges(deque<Edge>::pointer e1, deque<Edge>::pointer e2, deque<Edge>::pointer e3)
{
	edges[0]=e1;
	edges[1]=e2;
	edges[2]=e3;
}

void Face::addOppositeVertex(deque<Vertex>::pointer v)
{
	if(oppositeVertex1)
		oppositeVertex2=v;
	else
		oppositeVertex1=v;
//	v->setOppFace(this);
}

void Face::addNormalDir(double normal[3])
{
	normDir[0]=normal[0];
	normDir[1]=normal[1];
	normDir[2]=normal[2];
}

bool Face::testCircumSphere(Vertex& v, double centerRadius[4])
{
	double a[4][4], dx[4][4], dy[4][4], dz[4][4], c[4][4];
	double deta, detdx, detdy, detdz, detc;
	for(int i=0;i<3;i++)
	{
		dx[i][0]=dy[i][0]=dz[i][0]=c[i][0]=pow(vertices[i]->getCoord(X),2)+pow(vertices[i]->getCoord(Y),2)+pow(vertices[i]->getCoord(Z),2);
		a[i][0]=vertices[i]->getCoord(X);

		dx[i][1]=a[i][1]=vertices[i]->getCoord(Y);
		dy[i][1]=vertices[i]->getCoord(X);
		dz[i][1]=c[i][1]=vertices[i]->getCoord(X);

		dx[i][2]=a[i][2]=vertices[i]->getCoord(Z);
		dy[i][2]=vertices[i]->getCoord(Z);
		dz[i][2]=c[i][2]=vertices[i]->getCoord(Y);

		a[i][3]=1;
		dx[i][3]=1;
		dy[i][3]=1;
		dz[i][3]=1;
		c[i][3]=vertices[i]->getCoord(Z);
	}
	dx[3][0]=dy[3][0]=dz[3][0]=c[3][0]=pow(v.getCoord(X),2)+pow(v.getCoord(Y),2)+pow(v.getCoord(Z),2);
	a[3][0]=v.getCoord(X);

	dx[3][1]=a[3][1]=v.getCoord(Y);
	dy[3][1]=v.getCoord(X);
	dz[3][1]=c[3][1]=v.getCoord(X);

	dx[3][2]=a[3][2]=v.getCoord(Z);
	dy[3][2]=v.getCoord(Z);
	dz[3][2]=c[3][2]=v.getCoord(Y);

	a[3][3]=1;
	dx[3][3]=1;
	dy[3][3]=1;
	dz[3][3]=1;
	c[3][3]=v.getCoord(Z);
	deta  = determinant4x4(a);
	detdx = determinant4x4(dx);
	detdy = -1*determinant4x4(dy);
	detdz = determinant4x4(dz);
	detc  = determinant4x4(c);
	if(deta)
	{
		centerRadius[0]=detdx/2*deta;
		centerRadius[1]=detdy/2*deta;
		centerRadius[2]=detdz/2*deta;
		centerRadius[3]=sqrt(detdx*detdx+ detdy*detdy+ detdz*detdz - 4*deta*detc)/(2*abs(deta));
		return true;
	}
	else
		return false;
}

deque<Vertex>::pointer* Face::getVertices()
{
	return vertices;
}

deque<Edge>::pointer* Face::getEdges()
{
	return edges;
}

void Face::setNeighbourCell(deque<Cell>::pointer pCell)
{
	if(!cell1)
		cell1=pCell;
	else if(!cell2)
		cell2=pCell;
	else
		cout<<"ERROR while setting neighbour"<<endl;
}

void Face::setNeighCell1(deque<Cell>::pointer cellPtr)
{
	cell1=cellPtr;
}

void Face::setNeighCell2(deque<Cell>::pointer cellPtr)
{
	cell2=cellPtr;
}

void Face::incrNumOfOpenFaces(deque<deque<deque<deque<deque<Vertex>::pointer > > > >& grid)
{
	unsigned int *gridCoord;
	gridCoord = (unsigned int*)malloc(sizeof(unsigned int)*4);
	for(int i=0; i<3; i++)
	{
		vertices[i]->incrNumOfOpenFaces();
		//gridCoord=(unsigned int*)vertices[i]->sparePtr;
		//grid[gridCoord[0]][gridCoord[1]][gridCoord[2]][gridCoord[3]]->incrNumOfOpenFaces();
	}
}

deque<Cell>::pointer Face::getNeighCell1()
{
	return cell1;
}

deque<Cell>::pointer Face::getNeighCell2()
{
	return cell2;
}

deque<Vertex>::pointer Face::getOppositeVertex1()
{
	return oppositeVertex1;
}

deque<Vertex>::pointer Face::getOppositeVertex2()
{
	return oppositeVertex2;
}
///Plc Polygon class

PlcPolygon::PlcPolygon(deque<int>& vertexIndices, deque<Vertex>& vertexList)
{
	//cout<<vertexIndices.size()<<endl;
	//cout.flush();
	for(unsigned int i = 0; i<vertexIndices.size()-1;i++)
	{
		PlcEdge *newEdge = new PlcEdge(&vertexList[vertexIndices[i]],&vertexList[vertexIndices[i+1]]);
		plcEdgeList.push_back(*newEdge);
	}
}

///Plc Facet Class
void PlcFacet::setNewPolygon(deque<int>& vertexIndices, deque<Vertex>& vertexList)
{
	PlcPolygon *newPolygon = new PlcPolygon(vertexIndices,vertexList);
	plcPolygonList.push_back(*newPolygon);
}

///Cell class

Cell::Cell() {}

Cell::Cell(int size)
{
	vertices.resize(size);
	faces.resize(size);
	//neighbours.resize(size);
	edges.resize(size);
}

void Cell::addVertex(deque<Vertex>::pointer vertex)
{
	vertices.push_back(vertex);
}

void Cell::addEdge(deque<Edge>::pointer edge)
{
	edges.push_back(edge);
}

void Cell::addFace(deque<Face>::pointer face)
{
	faces.push_back(face);
	faces.back()->setNeighbourCell(this);
}

//void Cell::addNeighbour(deque<Cell>::pointer cell, deque<Face>::pointer face)
//{
//	neighbours.insert(pair<deque<Cell>::pointer, deque<Face>::pointer>(cell,face));
//}

size_t Cell::getVertexListSize()
{
	return vertices.size();
}

size_t Cell::getFaceListSize()
{
	return faces.size();
}

size_t Cell::getNeighListSize()
{
	deque<deque<Face>::pointer>::iterator fit;
	size_t neighSize=0;
	for(fit=faces.begin(); fit!=faces.end(); fit++)
	{
		if((*fit)->getNeighCell1() && (*fit)->getNeighCell2())
			neighSize++;
	}
	return neighSize;
}

size_t Cell::getEdgeListSize()
{
	return edges.size();
}

deque<deque<Vertex>::pointer>& Cell::getVertices()
{
	return vertices;
}

deque<deque<Edge>::pointer>& Cell::getEdges()
{
	return edges;
}

deque<deque<Face>::pointer>& Cell::getFaces()
{
	return faces;
}

//multimap<deque<Cell>::pointer, deque<Face>::pointer>& Cell::getNeighbours()
//{
//	return neighbours;
//}

bool Cell::testCircumCircle(Vertex& v, double centerRadius[4])
{
	return edges[0]->testCircumCircle(v,centerRadius);
}

bool Cell::testCircumSphere(Vertex& v, double centerRadius[4])
{
	return faces[0]->testCircumSphere(v,centerRadius);
}

void Cell::addFEVs(deque<Face>::pointer face)
{
	for(int i=0; i<3; i++)
	{
		vertices.push_back(face->getVertices()[i]);
		edges.push_back(face->getEdges()[i]);
	}
	addFace(face);
}

void Cell::delFEVs(deque<Face>::pointer face)
{
	for(int i=0; i<3; i++)
	{
		vertices.clear();
		edges.clear();
	}
	if(face->getNeighCell1()==this)
		face->setNeighCell1(NULL);
	else
		face->setNeighCell2(NULL);
}
bool Cell::checkOrientation(Vertex& v2)
{
	double matrixp1[3][3];
	double matrixp2[3][3];
	Vertex *oppVertex;
	multimap<deque<Cell>::pointer, deque<Face>::pointer>::iterator niter;
	//niter = neighbours.begin();
	//cout<<"Opp v: "<<(*((*niter).first->getNeighbours().find(this))).second->getOppositeVertex()->getXCoord()<<endl;
	if(faces.back()->getNeighCell1()==this)
		oppVertex=faces.back()->getOppositeVertex2();
	else if(faces.back()->getNeighCell2()==this)
		oppVertex=faces.back()->getOppositeVertex1();
	else
		cout<<"ERROR ERROR::"<<endl;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			matrixp2[i][j] = (faces.back()->getVertices()[i]->getCoord((axisToSort)j)) - (oppVertex->getCoord((axisToSort)j));
			matrixp1[i][j] = (faces.back()->getVertices()[i]->getCoord((axisToSort)j)) - (v2.getCoord((axisToSort)j));
		}
	if(determinant3x3(matrixp1) * determinant3x3(matrixp2)<0)
		return true;
	else
		return false;
}
///Solid class

void Solid::populateVertices(deque<Vertex>& vertexList)
{
	//cout<<listOfVertices.size();
	//cout.flush();
	deque<Vertex>::iterator vit;
	int i=0;
	for(vit=vertexList.begin(); vit!=vertexList.end(); vit++, ++i)
	{
		listOfVertices.push_back(*vit);
		listOfVertices.back().setId(i);
	}
}
void Solid::delaunize()
{
	deque<Vertex> vertices;
	deque<Vertex>::iterator vit;
	map<int,deque<Face>::pointer> afl;
	deque<deque<deque<deque<deque<Vertex>::pointer > > > > grid;
	double minx=0.0, miny=0.0, minz=0.0, maxx=0.0, maxy=0.0,maxz=0.0;
	for(vit=listOfVertices.begin(); vit!=listOfVertices.end(); vit++)
	{
		if(vit==listOfVertices.begin())
		{
			minx=vit->getXCoord();
			miny=vit->getYCoord();
			minz=vit->getZCoord();
			maxx=vit->getXCoord();
			maxy=vit->getYCoord();
			maxz=vit->getZCoord();
		}
		else
		{
			if(vit->getXCoord()<minx)
				minx=vit->getXCoord();
			if(vit->getYCoord()<miny)
				miny=vit->getYCoord();
			if(vit->getZCoord()<minz)
				minz=vit->getZCoord();
			if(vit->getXCoord()>maxx)
				maxx=vit->getXCoord();
			if(vit->getYCoord()>maxy)
				maxy=vit->getYCoord();
			if(vit->getZCoord()>maxz)
				maxz=vit->getZCoord();
		}
	}
	int xrange,yrange,zrange;
	int stepSize=1;
	xrange = 1+floor(maxx)-floor(minx);	yrange=1+floor(maxy)-floor(miny); zrange=1+floor(maxz)-floor(minz);
	grid.resize(xrange);
	for(int i=0;i<xrange;i++)
	{
		grid[i].resize(yrange);
	}
	for(int i=0;i<xrange;i++)
		for(int j=0; j<yrange; j++)
			{
				grid[i][j].resize(zrange);
			}
	//cout<<grid.size();
	//cout.flush();
	// map points to the grid
	//int i;
	int *gridcoords;
	for(unsigned int i=0; i<listOfVertices.size();i++)
	{
		//int gridCoords[3];
		gridcoords= (int*)malloc(sizeof(int)*4);
		gridcoords[0]=(int)(listOfVertices[i].getXCoord()-floor(minx))/stepSize;
		gridcoords[1]=(int)(listOfVertices[i].getYCoord()-floor(miny))/stepSize;
		gridcoords[2]=(int)(listOfVertices[i].getZCoord()-floor(minz))/stepSize;
		grid[gridcoords[0]][gridcoords[1]][gridcoords[2]].push_back(&listOfVertices[i]);
		gridcoords[3]= grid[gridcoords[0]][gridcoords[1]][gridcoords[2]].size()-1;
		listOfVertices[i].sparePtr=(int*)gridcoords;
		vertices.push_back(listOfVertices[i]);
	}
	sort(vertices.begin(),vertices.end(),sortVertexX);
	dewall(X, vertices, afl, grid);
}

void Solid::drawEdges()
{
	deque<deque<Edge>::pointer>::iterator eit;
	//deque<deque<Face>::pointer>::iterator fit;
	deque<Vertex>::iterator vit;
	for(vit=listOfVertices.begin(); vit!=listOfVertices.end(); vit++)
	{
		glPushMatrix();
		glColor4f(1.0,1.0,0.0,1.0);
		glTranslated(vit->getXCoord(), vit->getYCoord(), vit->getZCoord());
		glutSolidSphere(0.05,10,10);
		glPopMatrix();
	}
	for(int i=firstCell; i<lastCell; i++)
	{
		for(eit=listOfCells[i].getEdges().begin(); eit!=listOfCells[i].getEdges().end(); eit++)
		{
			glColor4f(1.0,0.0,0.0,1.0);
			glBegin(GL_LINES);
			glVertex3d((*eit)->getVertex()[0]->getXCoord(), (*eit)->getVertex()[0]->getYCoord(), (*eit)->getVertex()[0]->getZCoord());
			glVertex3d((*eit)->getVertex()[1]->getXCoord(), (*eit)->getVertex()[1]->getYCoord(), (*eit)->getVertex()[1]->getZCoord());
			glEnd();
		}
//		for(int j=0; j<3; j++)
//		{
//			cout<<"number of faces:"<<listOfCells[i].getFaces().size()<<endl;
//			glColor4f(1.0,0.0,0.0,1.0);
//			glBegin(GL_LINES);
//			glVertex3d(listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[0]->getXCoord(), listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[0]->getYCoord(), listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[0]->getZCoord());
//			glVertex3d(listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[1]->getXCoord(), listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[1]->getYCoord(), listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[1]->getZCoord());
//			cout<<"edge-->X,Y,Z:"<<listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[0]->getXCoord()<<","<< listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[0]->getYCoord()<<","<< listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[0]->getZCoord()<<endl;
//			cout<<"vert2->X,Y,Z:"<<listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[1]->getXCoord()<<","<< listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[1]->getYCoord()<<","<< listOfCells[i].getFaces()[faceToShow].getEdges()[j]->getVertex()[1]->getZCoord()<<endl;
//			glEnd();
//		}
	}
}

int Solid::listOfCellsSize()
{
	return listOfCells.size();
}
/*
void Solid::sortVertices(Solid::axisToSort axis, deque<Vertex> inList, deque<Vertex> outList)
{
	for(int i=0; i<inList.size(); i++)
		cout<<inList[i].getXCoord()<<",";
	cout.flush();
	cout<<endl<<"Sorted List"<<endl;
	sort(inList.begin(),inList.end(),sortVertexX);
	for(int i=0; i<inList.size(); i++)
		cout<<inList[i].getXCoord()<<",";
	cout.flush();
}*/



/// Plc class

deque<Vertex>& Plc::getPlcVertices()
{
	return listOfVertices;
}

void Plc::parsePolyFile (char* fileName)
{
	ifstream infile(fileName);
	string stringLine;
	int stringSize,listType=0;
	bool foundDig=false,nextLine=false;
	char tempChar;
	int i;
	if(infile.bad())
		cout<<"bad";
	if(infile.fail())
		cout<<"failed";
	if(infile.is_open())
		cout<<"is open";
	while(infile.good())
	{
		stringstream line(stringstream::in | stringstream::out);
		getline(infile, stringLine);
		line<<stringLine;
		line.flush();
		//cout<<line.str()<<endl;
		//cout.flush();
		stringSize = stringLine.size();
		for(i=0;i<stringSize;i++)
		{
			tempChar = line.peek();
			if(isdigit(tempChar))
			{
				foundDig = true;
				break;
			}
			if(tempChar=='#' || tempChar=='\0')
			{
				nextLine = true;
				break;
			}
		}
		if(foundDig)
		{

			parseTypeData(line,infile,listType);
			listType++;
			foundDig = false;
		}
	}
	cout<<"file processed";
}

void Plc::parseTypeData(stringstream& line, ifstream& infile, int listType)
{
	if(listType == 0)
	{
		int nodeCount, dims, attrsNum;//vertexNum;
		double x,y,z;
		bool boundary,boundaryMarker;
		Vertex *newVertex;
		string stringLine;
		line>>nodeCount>>dims>>attrsNum>>boundary;
		//vertexLines.seekg(0,ios_base::end);
		for(int i=0; infile.good() && i<nodeCount ; i++)
		{
			int vertexNum;
			stringstream vertexLines(stringstream::in | stringstream::out);
			getline(infile, stringLine);
			vertexLines<<stringLine;
			//vertexLines.flush();
			if(boundary)
				vertexLines>>vertexNum>>x>>y>>z>>boundaryMarker;
			else
				vertexLines>>vertexNum>>x>>y>>z;
			newVertex = new Vertex(x,y,z,boundaryMarker);
			listOfVertices.push_back(*newVertex);
		}
		//cout<<listOfVertices.size()<<endl;
		//cout.flush();
		//std::cout<<nodeCount<<","<<dims<<","<<attrsNum<<","<<attrsNum<<endl;
	}
	else if(listType==1)
	{
		int facetCount, polygonCount, holeCount, vertexCount;
		deque<int> polygonVertices(10);
		bool hasBoundaryMarker, facetBoundaryMarker;
		PlcFacet *newFacet;
		string stringLine;
		line>>facetCount>>hasBoundaryMarker;
		for(int i=0;i<facetCount;i++)
		{
			stringstream polygonParams(stringstream::in | stringstream::out);
			getline(infile,stringLine);
			polygonParams<<stringLine;
			if(hasBoundaryMarker)
				polygonParams>>polygonCount>>holeCount>>facetBoundaryMarker;
			else
				polygonParams>>polygonCount>>holeCount;
			newFacet = new PlcFacet();
			for(int j=0;j<polygonCount;j++)
			{
				stringstream corners(stringstream::in | stringstream::out);
				getline(infile,stringLine);
				corners<<stringLine;
				corners>>vertexCount;
				polygonVertices.resize(vertexCount);
				for(int k=0;k<vertexCount;k++)
					corners>>polygonVertices[k];
				//cout<<polygonVertices.size();
				//cout.flush();
				newFacet->setNewPolygon(polygonVertices,listOfVertices);
			}
			listOfFacets.push_back(*newFacet);
		}
	}
	else if(listType==2)
	{
		int volumeHoleCount,holeNumber;
		double x,y,z;
		Hole *newHole;
		string stringLine;
		line>>volumeHoleCount;
		for(int i=0; i<volumeHoleCount; i++)
		{
			stringstream holeCoords(stringstream::in | stringstream::out);
			getline(infile,stringLine);
			holeCoords<<stringLine;
			holeCoords>>holeNumber>>x>>y>>z;
			newHole = new Hole(x,y,z);
			listOfHoles.push_back(*newHole);
		}
	}
	else if(listType==3)
	{
		//TODO fill code to process region attributes
	}
}

