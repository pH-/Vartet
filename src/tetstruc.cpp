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
extern bool showCircle;
extern bool wire;
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

double Vertex::getSqDistance(double coords[3])
{
	return pow((getXCoord()-coords[0]),2) + pow((getYCoord()-coords[1]),2) +pow((getZCoord()-coords[2]),2);
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

void Vertex::decrNumOfOpenFaces()
{
	if(numOfOpenFaces>0)
		numOfOpenFaces--;
	else
		cout<<"flag error here"<<endl;
}

int Vertex::getNumOfOpenFaces()
{
	return numOfOpenFaces;
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

bool Edge::testCircumCircle(deque<Vertex>::pointer v, double centerRadius[4])
{
	double sideaSq, sidebSq, sidecSq;
	if(vertices[0]->getId() == v->getId())
		return false;
	if(vertices[1]->getId() == v->getId())
		return false;
	sideaSq = vertices[1]->getSqDistance(*v);
	sidebSq = vertices[0]->getSqDistance(*v);
	sidecSq = vertices[0]->getSqDistance(*vertices[1]);
	for(int i=0; i<3; i++)
		centerRadius[i]=((vertices[0]->getCoord((axisToSort)i)*(sideaSq*(sidebSq+sidecSq-sideaSq)))+
				         (vertices[1]->getCoord((axisToSort)i)*(sidebSq*(sidecSq+sideaSq-sidebSq)))+
				         (v->getCoord((axisToSort)i)			  *(sidecSq*(sideaSq+sidebSq-sidecSq))))
				        /(2*(sideaSq*sidebSq+sidebSq*sidecSq+sidecSq*sideaSq)-(sideaSq*sideaSq+sidebSq*sidebSq+sidecSq*sidecSq));
	centerRadius[3]=sqrt(pow((v->getXCoord()-centerRadius[0]),2) + pow((v->getYCoord()-centerRadius[1]),2) +pow((v->getZCoord()-centerRadius[2]),2));
	return true;
}

double Edge::testCircumCircleAdaptive(Vertex& v1, Vertex& v2)
{
	double p[2][3], c[3], d[3];
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<3; j++)
		{
			p[i][j]=vertices[i]->getCoord((axisToSort)j);
		}
	}
	for(int i=0; i<3; i++)
	{
		c[i]=v1.getCoord((axisToSort)i);
		d[i]=v2.getCoord((axisToSort)i);
	}
	if(orient3d(p[0],p[1],c,d))
		return false;
	else
	{
		double centerRadius[4];
		testCircumCircle(&v1,centerRadius);
		return(v2.getSqDistance(centerRadius) < centerRadius[3]);
	}
}

bool Edge::check2DOrientationAdaptive(Vertex& v2)
{
	double p[2][3], c[3];
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<3; j++)
		{
			p[i][j]=vertices[i]->getCoord((axisToSort)j);
		}
	}
	for(int i=0; i<3; i++)
		c[i]=v2.getCoord((axisToSort)i);

	return ((((p[0][0]-p[1][0]) - (p[0][2]-p[1][2])) * ((c[1]-p[1][1])-(c[2]-p[1][2]))) - (((c[0]-p[1][0]) - (c[2]-p[1][2])) * ((p[0][1]-p[1][1])-(p[0][2]-p[1][2]))));
}
///Plc Edge class

PlcEdge::PlcEdge(Vertex *vertex1, Vertex *vertex2)
{
	vertices[0]=vertex1;
	vertices[1]=vertex2;
}

///Face class

Face::Face():oppositeVertex1(NULL),oppositeVertex2(NULL),cell1(NULL),cell2(NULL){}

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
//	double a[4][4], dx[4][4], dy[4][4], dz[4][4], c[4][4];
//	double deta, detdx, detdy, detdz, detc;
//	for(int i=0;i<3;i++)
//	{
//		dx[i][0]=dy[i][0]=dz[i][0]=c[i][0]=pow(vertices[i]->getCoord(X),2)+pow(vertices[i]->getCoord(Y),2)+pow(vertices[i]->getCoord(Z),2);
//		a[i][0]=vertices[i]->getCoord(X);
//
//		dx[i][1]=a[i][1]=vertices[i]->getCoord(Y);
//		dy[i][1]=vertices[i]->getCoord(X);
//		dz[i][1]=c[i][1]=vertices[i]->getCoord(X);
//
//		dx[i][2]=a[i][2]=vertices[i]->getCoord(Z);
//		dy[i][2]=vertices[i]->getCoord(Z);
//		dz[i][2]=c[i][2]=vertices[i]->getCoord(Y);
//
//		a[i][3]=1;
//		dx[i][3]=1;
//		dy[i][3]=1;
//		dz[i][3]=1;
//		c[i][3]=vertices[i]->getCoord(Z);
//	}
//	dx[3][0]=dy[3][0]=dz[3][0]=c[3][0]=pow(v.getCoord(X),2)+pow(v.getCoord(Y),2)+pow(v.getCoord(Z),2);
//	a[3][0]=v.getCoord(X);
//
//	dx[3][1]=a[3][1]=v.getCoord(Y);
//	dy[3][1]=v.getCoord(X);
//	dz[3][1]=c[3][1]=v.getCoord(X);
//
//	dx[3][2]=a[3][2]=v.getCoord(Z);
//	dy[3][2]=v.getCoord(Z);
//	dz[3][2]=c[3][2]=v.getCoord(Y);
//
//	a[3][3]=1;
//	dx[3][3]=1;
//	dy[3][3]=1;
//	dz[3][3]=1;
//	c[3][3]=v.getCoord(Z);
//	deta  = determinant4x4(a);
//	detdx = determinant4x4(dx);
//	detdy = -1*determinant4x4(dy);
//	detdz = determinant4x4(dz);
//	detc  = determinant4x4(c);
//	if(deta)
//	{
//		centerRadius[0]=detdx/2*deta;
//		centerRadius[1]=detdy/2*deta;
//		centerRadius[2]=detdz/2*deta;
//		centerRadius[3]=sqrt(detdx*detdx+ detdy*detdy+ detdz*detdz - 4*deta*detc)/(2*abs(deta));
//		return true;
//	}
//	else
//		return false;
	double a[3], bma[3], cma[3], dma[3], tempcrs1[3], tempcrs2[3], tempcrs3[3], radiusVector[3], dinomMatrix[3][3];
	double lensqdma, lensqcma, lensqbma, detdinomMatrix;
	for(int i=0; i<3; i++)
	{
		a[i]  =vertices[0]->getCoord((axisToSort)i);
		bma[i]=vertices[1]->getCoord((axisToSort)i)-a[i];
		cma[i]=vertices[2]->getCoord((axisToSort)i)-a[i];
		dma[i]=v.getCoord((axisToSort)i)-a[i];
		dinomMatrix[0][i]=bma[i];
		dinomMatrix[1][i]=cma[i];
		dinomMatrix[2][i]=dma[i];
	}
	detdinomMatrix=determinant3x3(dinomMatrix);
	if(detdinomMatrix)
	{
		CROSS(tempcrs1,bma,cma);
		CROSS(tempcrs2,dma,bma);
		CROSS(tempcrs3,cma,dma);
		lensqdma=vector3dLenSq(dma);
		lensqcma=vector3dLenSq(cma);
		lensqbma=vector3dLenSq(bma);
		MULT(tempcrs1,tempcrs1,lensqdma);
		MULT(tempcrs2,tempcrs2,lensqcma);
		MULT(tempcrs3,tempcrs3,lensqbma);
		ADD(radiusVector,tempcrs1,tempcrs2);
		ADD(radiusVector,radiusVector,tempcrs3);
		centerRadius[3]=sqrt(vector3dLenSq(radiusVector));
		centerRadius[3]/=abs(2*detdinomMatrix);
		MULT(radiusVector, radiusVector, (1/(2*detdinomMatrix)));
		ADD(centerRadius,a,radiusVector);
		return true;
	}
	else
		return false;

}
double Face::testCircumSphereAdaptive(Vertex& v1, Vertex& v2)
{
	double p[3][3], d[3],e[3];
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			p[i][j]=vertices[i]->getCoord((axisToSort)j);
		}
	}
	for(int i=0; i<3; i++)
	{
		d[i]=v1.getCoord((axisToSort)i);
		e[i]=v2.getCoord((axisToSort)i);
	}
	return insphere(p[0],p[1],p[2],d,e);
}

deque<Vertex>::pointer* Face::getVertices()
{
	return vertices;
}

deque<Edge>::pointer* Face::getEdges()
{
	return edges;
}

bool Face::setNeighbourCell(deque<Cell>::pointer pCell)
{
	if(!cell1)
		cell1=pCell;
	else if(!cell2)
		cell2=pCell;
	else
	{
		cout<<"ERROR while setting neighbour"<<endl;
		return false;
	}
	return true;
}

void Face::setNeighCell1(deque<Cell>::pointer cellPtr)
{
	cell1=cellPtr;
}

void Face::setNeighCell2(deque<Cell>::pointer cellPtr)
{
	cell2=cellPtr;
}

void Face::incrNumOfOpenFaces()
{
	for(int i=0; i<3; i++)
	{
		vertices[i]->incrNumOfOpenFaces();
		//gridCoord=(unsigned int*)vertices[i]->sparePtr;
		//grid[gridCoord[0]][gridCoord[1]][gridCoord[2]][gridCoord[3]]->incrNumOfOpenFaces();
	}
}

void Face::decrNumOfOpenFaces()
{
	for(int i=0; i<3; i++)
	{
		vertices[i]->decrNumOfOpenFaces();
		//gridCoord=(unsigned int*)vertices[i]->sparePtr;
		//grid[gridCoord[0]][gridCoord[1]][gridCoord[2]][gridCoord[3]]->incrNumOfOpenFaces();
	}
}

int Face::getNumOfOpenFaces()
{
	return (vertices[0]->getNumOfOpenFaces() && vertices[1]->getNumOfOpenFaces() && vertices[2]->getNumOfOpenFaces());
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

void Cell::setId(string cellId)
{
	id = cellId;
}

string Cell::getId()
{
	return id;
}

void Cell::addVertex(deque<Vertex>::pointer vertex)
{
	vertices.push_back(vertex);
}

void Cell::addEdge(deque<Edge>::pointer edge)
{
	edges.push_back(edge);
}

bool Cell::addFace(deque<Face>::pointer face)
{
	faces.push_back(face);
	return faces.back()->setNeighbourCell(this);
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

bool Cell::testCircumCircle(deque<Vertex>::pointer v, double centerRadius[4])
{
	return edges[0]->testCircumCircle(v,centerRadius);
}

double Cell::testCircumCircleAdaptive(Vertex& v1, Vertex& v2 )
{
	return edges[0]->testCircumCircleAdaptive(v1,v2);
}

trippleBool Cell::testCircumSphere(Vertex& v, double centerRadius[4])
{
	if(faces[0]->testCircumSphere(v,centerRadius))
	{
		Vertex ctr(centerRadius[0],centerRadius[1],centerRadius[2], false);
		if(faces.back()->getOppositeVertex1() || faces.back()->getOppositeVertex2())
		{
			if(faces.back()->getOppositeVertex1())
			{
				if(faces.back()->getOppositeVertex1()->getSqDistance(ctr)<=centerRadius[3]*centerRadius[3])
//					return false_val;
					return midstate_val;
//				if(faces.back()->getOppositeVertex1()->getSqDistance(ctr)==centerRadius[3]*centerRadius[3])
//					return midstate_val;
			}
			else
			{
				if(faces.back()->getOppositeVertex2()->getSqDistance(ctr)<=centerRadius[3]*centerRadius[3])
//					return false_val;
					return midstate_val;
//				if(faces.back()->getOppositeVertex2()->getSqDistance(ctr)==centerRadius[3]*centerRadius[3])
//					return midstate_val;
			}
			//if(!this->checkOrientation(ctr))
			if(!this->check3DOrientationAdaptive(ctr))
			{
				centerRadius[3]*=-1;
			}
		}
		return true_val;
	}
	else
		return false_val;
}
double Cell::testCircumSphereAdaptive(Vertex& v1, Vertex& v2)
{
	return faces[0]->testCircumSphereAdaptive(v1,v2);
}
void Cell::addFEVs(deque<Face>::pointer face)
{
//	if(face->getId()=="17,32,33")
//		cout<<"detect culprit"<<endl;
	for(int i=0; i<3; i++)
	{
		vertices.push_back(face->getVertices()[i]);
		edges.push_back(face->getEdges()[i]);
	}
	addFace(face);
}

void Cell::delFEVs(deque<Face>::pointer face)
{
	vertices.clear();
	edges.clear();
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

bool Cell::check3DOrientationAdaptive(Vertex& v2)
{
	Vertex *oppVertex;
	for(int i=0; i<3; i++)
	{
		if(faces.back()->getVertices()[i]->getId()==v2.getId())
			return false;
	}
	if(this->getNeighListSize()>0)
	{
		if(faces.back()->getNeighCell1()==this)
			oppVertex=faces.back()->getOppositeVertex2();
		else if(faces.back()->getNeighCell2()==this)
			oppVertex=faces.back()->getOppositeVertex1();
		else
			cout<<"possible ERROR ERROR::"<<endl;
	}
	else
	{
		double p[3][3], d1[3];
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				p[i][j] = faces.back()->getVertices()[i]->getCoord((axisToSort)j);
			}
		}
		for(int i=0;i<3; i++)
		{
			d1[i] = v2.getCoord((axisToSort)i);
		}
		return orient3d(p[0],p[1],p[2],d1);
	}
	double p[3][3], d1[3], d2[3];
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			p[i][j] = faces.back()->getVertices()[i]->getCoord((axisToSort)j);
		}
	}
	for(int i=0;i<3; i++)
	{
		d1[i] = v2.getCoord((axisToSort)i);
		d2[i] = oppVertex->getCoord((axisToSort)i);
	}
	if(orient3d(p[0],p[1],p[2],d1)*orient3d(p[0],p[1],p[2],d2)<0)
		return true;
	else
		return false;
}
bool Cell::check2DOrientationAdaptive(Vertex& v2)
{
	for(int i =0; i<2; i++)
	{
		if(edges.back()->getVertex()[i]->getId() == v2.getId())
			return false;
	}
	return edges[0]->check2DOrientationAdaptive(v2);
}
void Cell::setCircumCenter(double circumCenter[3])
{
	for(int i=0; i<3; i++)
		circumCtr[i]=circumCenter[i];
}

void Cell::setCircumRadius(double radius)
{
	circumRadius=radius;
}

double Cell::getCircumRadius()
{
	return circumRadius;
}

double* Cell::getCircumCenter()
{
	return circumCtr;
}

void Cell::delVertices()
{
	vertices.clear();
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
void Solid::populateVerticesRandom()
{
	double x,y,z;
	for(int i=0;i<10;i++)
	{
		x= (randnum()-0.5)*20;
		y= (randnum()-0.5)*20;
		z= (randnum()-0.5)*20;
		Vertex* newVertex = new Vertex(x,y,z,0);
		listOfVertices.push_back(*newVertex);
		listOfVertices.back().setId(i);
	}
}
void Solid::delaunize()
{
	deque<Vertex> vertices;
	deque<Vertex>::iterator vit;
	map<string,deque<Face>::pointer> afl;
	kdtree *kdTree = new kdtree();
	kdTree->buildTree(listOfVertices);
	dewall(afl,kdTree,kdTree);
}

void Solid::drawEdges()
{
	deque<deque<Edge>::pointer>::iterator eit;
	//deque<deque<Face>::pointer>::iterator fit;
	deque<Vertex>::iterator vit;
	double resolution=1.0;
//	for(int i=0; i<faceToShow; i++)
//	{
//		glPushMatrix();
//		glColor4f(1.0,1.0,0.0,1.0);
//		glTranslated(listOfVertices[i].getXCoord()/resolution, listOfVertices[i].getYCoord()/resolution,listOfVertices[i].getZCoord()/resolution);
//		glutSolidSphere(0.05,10,10);
//		glPopMatrix();
//	}

//	for(int i=firstCell; i<lastCell; i++)
//	{
//		int j=0;
//
//		for(j=0; j<4; j++)
//		{
//			glColor4f(0.0,1.0/(j+1),1.0,1.0);
//			//cout<<"number of faces:"<<listOfCells[i].getFaces().size()<<endl;
//			glBegin(GL_TRIANGLES);
//			glVertex3d(listOfCells[i].getFaces()[j]->getVertices()[0]->getXCoord(), listOfCells[i].getFaces()[j]->getVertices()[0]->getYCoord(), listOfCells[i].getFaces()[j]->getVertices()[0]->getZCoord());
//			glVertex3d(listOfCells[i].getFaces()[j]->getVertices()[1]->getXCoord(), listOfCells[i].getFaces()[j]->getVertices()[1]->getYCoord(), listOfCells[i].getFaces()[j]->getVertices()[1]->getZCoord());
//			glVertex3d(listOfCells[i].getFaces()[j]->getVertices()[2]->getXCoord(), listOfCells[i].getFaces()[j]->getVertices()[2]->getYCoord(), listOfCells[i].getFaces()[j]->getVertices()[2]->getZCoord());
////			cout<<"v1-->X,Y,Z:"<<listOfCells[i].getFaces()[j]->getVertices()[0]->getXCoord()<<","<< listOfCells[i].getFaces()[j]->getVertices()[0]->getXCoord()<<","<< listOfCells[i].getFaces()[j]->getVertices()[0]->getXCoord()<<endl;
////			cout<<"v2-->X,Y,Z:"<<listOfCells[i].getFaces()[j]->getVertices()[1]->getXCoord()<<","<< listOfCells[i].getFaces()[j]->getVertices()[1]->getXCoord()<<","<< listOfCells[i].getFaces()[j]->getVertices()[1]->getXCoord()<<endl;
////			cout<<"v3-->X,Y,Z:"<<listOfCells[i].getFaces()[j]->getVertices()[2]->getXCoord()<<","<< listOfCells[i].getFaces()[j]->getVertices()[2]->getXCoord()<<","<< listOfCells[i].getFaces()[j]->getVertices()[2]->getXCoord()<<endl;
//			glEnd();
//		}
//
//	}
	for(vit=listOfVertices.begin(); vit!=listOfVertices.end(); vit++)
	{
		glPushMatrix();
		glColor4f(0.0,1.0,0.0,1.0);
		glTranslated(vit->getXCoord()/resolution, vit->getYCoord()/resolution, vit->getZCoord()/resolution);
		glutSolidSphere(0.05,10,10);
		glPopMatrix();
	}
	for(int i=firstCell; i<lastCell; i++)
	{
		double *center;
		glColor4f(1.0,0.0,0.0,1.0);
		for(eit=listOfCells[i].getEdges().begin(); eit!=listOfCells[i].getEdges().end(); eit++)
		{
			glColor4f(1.0,0.0,0.0,1.0);
			glBegin(GL_LINES);
			glVertex3d((*eit)->getVertex()[0]->getXCoord()/resolution, (*eit)->getVertex()[0]->getYCoord()/resolution, (*eit)->getVertex()[0]->getZCoord()/resolution);
			glVertex3d((*eit)->getVertex()[1]->getXCoord()/resolution, (*eit)->getVertex()[1]->getYCoord()/resolution, (*eit)->getVertex()[1]->getZCoord()/resolution);
			glEnd();
		}
		if(showCircle)
		{
			center=listOfCells[i].getCircumCenter();
//			if(listOfCells[i].getVertexListSize()==4)
//			{
				glPushMatrix();
				glTranslated(center[0]/resolution,center[1]/resolution,center[2]/resolution);
				if(wire)
					glutWireSphere(listOfCells[i].getCircumRadius()/resolution,40,40);
				else
					glutSolidSphere(listOfCells[i].getCircumRadius()/resolution,40,40);
				glPopMatrix();
//			}
//			else
//			{
//				glPushMatrix();
//				glBegin(GL_LINE_LOOP);
//				glTranslated(center[0]/resolution,center[1]/resolution,center[2]/resolution);
//				for (int ic=0; ic < 360; ic++)
//				{
//					double degInRad = ic*(3.14159/180);
//					glVertex3d(cos(degInRad)*(listOfCells[i].getCircumRadius()/resolution),center[1],sin(degInRad)*(listOfCells[i].getCircumRadius()/resolution));
//				}
//				glEnd();
//				glPopMatrix();
//			}
		}
	}
}

int Solid::listOfCellsSize()
{
	return listOfCells.size();
}

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

