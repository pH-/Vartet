/*
 * tetstruc.cpp
 *
 *  Created on: Jun 12, 2011
 *      Author: phanindrabhagavatula
 */

#include "includes.h"
using namespace std;
//extern int firstCell;
//extern int lastCell;
//extern int faceToShow;
//extern bool showCircle;
//extern bool wire;
int counter =0;

class convexHull4d;
///Global Functions

bool sortVertexX(Vertex v1, Vertex v2) { return (v1.getCoords()[0]<v2.getCoords()[0]);}
bool sortVertexY(Vertex v1, Vertex v2) { return (v1.getCoords()[1]<v2.getCoords()[1]);}
bool sortVertexZ(Vertex v1, Vertex v2) { return (v1.getCoords()[2]<v2.getCoords()[2]);}

/// Coordinates Class
//Coordinates::Coordinates(double x, double y, double z)
//{
//	coords[0]=x;
//	coords[1]=y;
//	coords[2]=z;
//}
//
//void Coordinates::setCoords(double x, double y, double z)
//{
//	coords[0]=x;
//	coords[1]=y;
//	coords[2]=z;
//}
//
//double Coordinates::getXCoord()
//{
//	return coords[0];
//}
//
//double Coordinates::getYCoord()
//{
//	return coords[1];
//}
//
//double Coordinates::getZCoord()
//{
//	return coords[2];
//}

/// Vertex Class

Vertex::Vertex(double x, double y, double z)
{
//	vertexCoord = new Coordinates(x,y,z);
	coords[0]=x;
	coords[1]=y;
	coords[2]=z;
//	oppositeFace = NULL;
}

double Vertex::getCoord(axisToSort axis)
{
	if(axis==X)
		return coords[0];
	else if(axis==Y)
		return coords[1];
	else
		return coords[2];
}

double Vertex::getXCoord()
{
	return coords[0];
}

double Vertex::getYCoord()
{
	return coords[1];
}

double Vertex::getZCoord()
{
	return coords[2];
}

double Vertex::getSqDistance(Vertex& v2)
{
	return pow((getCoords()[0]-v2.getCoords()[0]),2) + pow((getCoords()[1]-v2.getCoords()[1]),2) +pow((getCoords()[2]-v2.getCoords()[2]),2);
}

double Vertex::getSqDistance(double coords[3])
{
	return pow((getCoords()[0]-coords[0]),2) + pow((getCoords()[1]-coords[1]),2) +pow((getCoords()[2]-coords[2]),2);
}

//bool Vertex::sameAs(Vertex& v)
//{
//	return(getXCoord()==v.getXCoord() && getYCoord()==v.getYCoord() && getZCoord()==v.getZCoord());
//}

//void Vertex::setOppFace(deque<Face>::pointer face)
//{
//	oppositeFace = face;
//}

//void Vertex::incrNumOfOpenFaces()
//{
//	if(numOfOpenFaces<0)
//		numOfOpenFaces=1;
//	else
//		numOfOpenFaces++;
//}
//
//void Vertex::decrNumOfOpenFaces()
//{
//	if(numOfOpenFaces>0)
//		numOfOpenFaces--;
//	else
//		cout<<"flag error here"<<endl;
//}
//
//int Vertex::getNumOfOpenFaces()
//{
//	return numOfOpenFaces;
//}

///Plc Hole class

Hole::Hole(double x, double y, double z)
{
//	holeCoords = new Coordinates(x,y,z);
	holeCoords[0]=x;
	holeCoords[1]=y;
	holeCoords[2]=z;
}

///Edge class

Edge::Edge(long v1, long v2)
{
	vertices.push_back(v1);
	vertices.push_back(v2);
}

void Edge::insertVertex(long v)
{
	vertices.push_back(v);
}

deque<long>& Edge::getVertices()
{
	return vertices;
}

//bool Edge::testCircumCircle(deque<Vertex>::pointer v, double centerRadius[4])
//{
//	double sideaSq, sidebSq, sidecSq;
//	if(vertices[0]->getId() == v->getId())
//		return false;
//	if(vertices[1]->getId() == v->getId())
//		return false;
//	sideaSq = vertices[1]->getSqDistance(*v);
//	sidebSq = vertices[0]->getSqDistance(*v);
//	sidecSq = vertices[0]->getSqDistance(*vertices[1]);
//	for(int i=0; i<3; i++)
//		centerRadius[i]=((vertices[0]->getCoord((axisToSort)i)*(sideaSq*(sidebSq+sidecSq-sideaSq)))+
//				         (vertices[1]->getCoord((axisToSort)i)*(sidebSq*(sidecSq+sideaSq-sidebSq)))+
//				         (v->getCoord((axisToSort)i)			  *(sidecSq*(sideaSq+sidebSq-sidecSq))))
//				        /(2*(sideaSq*sidebSq+sidebSq*sidecSq+sidecSq*sideaSq)-(sideaSq*sideaSq+sidebSq*sidebSq+sidecSq*sidecSq));
//	centerRadius[3]=sqrt(pow((v->getXCoord()-centerRadius[0]),2) + pow((v->getYCoord()-centerRadius[1]),2) +pow((v->getZCoord()-centerRadius[2]),2));
//	return true;
//}
//
//double Edge::testCircumCircleAdaptive(Vertex& v1, Vertex& v2)
//{
//	double p[2][3], c[3], d[3];
//	for(int i=0; i<2; i++)
//	{
//		for(int j=0; j<3; j++)
//		{
//			p[i][j]=vertices[i]->getCoord((axisToSort)j);
//		}
//	}
//	for(int i=0; i<3; i++)
//	{
//		c[i]=v1.getCoord((axisToSort)i);
//		d[i]=v2.getCoord((axisToSort)i);
//	}
//	if(orient3d(p[0],p[1],c,d))
//		return false;
//	else
//	{
//		double centerRadius[4];
//		testCircumCircle(&v1,centerRadius);
//		return(v2.getSqDistance(centerRadius) < centerRadius[3]);
//	}
//}
//
//bool Edge::check2DOrientationAdaptive(Vertex& v2)
//{
//	double p[2][3], c[3];
//	for(int i=0; i<2; i++)
//	{
//		for(int j=0; j<3; j++)
//		{
//			p[i][j]=vertices[i]->getCoord((axisToSort)j);
//		}
//	}
//	for(int i=0; i<3; i++)
//		c[i]=v2.getCoord((axisToSort)i);
//
//	return ((((p[0][0]-p[1][0]) - (p[0][2]-p[1][2])) * ((c[1]-p[1][1])-(c[2]-p[1][2]))) - (((c[0]-p[1][0]) - (c[2]-p[1][2])) * ((p[0][1]-p[1][1])-(p[0][2]-p[1][2]))));
//}
///Plc Edge class

PlcEdge::PlcEdge(Vertex *vertex1, Vertex *vertex2)
{
	vertices[0]=vertex1;
	vertices[1]=vertex2;
}

///Face class

//Face::Face():oppositeVertex1(NULL),oppositeVertex2(NULL),cell1(NULL),cell2(NULL){}
Face::Face(Base2dSimplex* obj)
{
	this->iD = obj->getId();
	this->neigh3dSimplex1 = obj->getNeighbour1();
	this->neigh3dSimplex2 = obj->getNeighbour2();
//	for(int i=0; i<3; i++)
//		this->vertices[i] = obj->getVertexList()[i];
}
Face::Face(Face* obj)
{
	this->iD = obj->getId();
	this->neigh3dSimplex1 = obj->getNeighbour1();
	this->neigh3dSimplex2 = obj->getNeighbour2();
	for(int i=0; i<3; i++)
		this->vertices[i] = obj->getVertexList()[i];
}
//void Face::setId(long vid)
//{
//	iD=vid;
//}

//string Face::getId()
//{
//	return iD;
//}
//void Face::addVertices(deque<Vertex>::pointer v1, deque<Vertex>::pointer v2, deque<Vertex>::pointer v3)
//{
//	vertices[0]=v1;
//	vertices[1]=v2;
//	vertices[2]=v3;
//}

void Face::addEdges(long e1, long e2, long e3)
{
	edges[0]=e1;
	edges[1]=e2;
	edges[2]=e3;
}

//void Face::addOppositeVertex(deque<Vertex>::pointer v)
//{
//	if(oppositeVertex1)
//		oppositeVertex2=v;
//	else
//		oppositeVertex1=v;
////	v->setOppFace(this);
//}

void Face::addNormalDir(double normal[3])
{
	normDir[0]=normal[0];
	normDir[1]=normal[1];
	normDir[2]=normal[2];
}

double* Face::getNormal()
{
	return normDir;
}
//bool Face::testCircumSphere(Vertex& v, double centerRadius[4])
//{
////	double a[4][4], dx[4][4], dy[4][4], dz[4][4], c[4][4];
////	double deta, detdx, detdy, detdz, detc;
////	for(int i=0;i<3;i++)
////	{
////		dx[i][0]=dy[i][0]=dz[i][0]=c[i][0]=pow(vertices[i]->getCoord(X),2)+pow(vertices[i]->getCoord(Y),2)+pow(vertices[i]->getCoord(Z),2);
////		a[i][0]=vertices[i]->getCoord(X);
////
////		dx[i][1]=a[i][1]=vertices[i]->getCoord(Y);
////		dy[i][1]=vertices[i]->getCoord(X);
////		dz[i][1]=c[i][1]=vertices[i]->getCoord(X);
////
////		dx[i][2]=a[i][2]=vertices[i]->getCoord(Z);
////		dy[i][2]=vertices[i]->getCoord(Z);
////		dz[i][2]=c[i][2]=vertices[i]->getCoord(Y);
////
////		a[i][3]=1;
////		dx[i][3]=1;
////		dy[i][3]=1;
////		dz[i][3]=1;
////		c[i][3]=vertices[i]->getCoord(Z);
////	}
////	dx[3][0]=dy[3][0]=dz[3][0]=c[3][0]=pow(v.getCoord(X),2)+pow(v.getCoord(Y),2)+pow(v.getCoord(Z),2);
////	a[3][0]=v.getCoord(X);
////
////	dx[3][1]=a[3][1]=v.getCoord(Y);
////	dy[3][1]=v.getCoord(X);
////	dz[3][1]=c[3][1]=v.getCoord(X);
////
////	dx[3][2]=a[3][2]=v.getCoord(Z);
////	dy[3][2]=v.getCoord(Z);
////	dz[3][2]=c[3][2]=v.getCoord(Y);
////
////	a[3][3]=1;
////	dx[3][3]=1;
////	dy[3][3]=1;
////	dz[3][3]=1;
////	c[3][3]=v.getCoord(Z);
////	deta  = determinant4x4(a);
////	detdx = determinant4x4(dx);
////	detdy = -1*determinant4x4(dy);
////	detdz = determinant4x4(dz);
////	detc  = determinant4x4(c);
////	if(deta)
////	{
////		centerRadius[0]=detdx/2*deta;
////		centerRadius[1]=detdy/2*deta;
////		centerRadius[2]=detdz/2*deta;
////		centerRadius[3]=sqrt(detdx*detdx+ detdy*detdy+ detdz*detdz - 4*deta*detc)/(2*abs(deta));
////		return true;
////	}
////	else
////		return false;
//	double a[3], bma[3], cma[3], dma[3], tempcrs1[3], tempcrs2[3], tempcrs3[3], radiusVector[3], dinomMatrix[3][3];
//	double lensqdma, lensqcma, lensqbma, detdinomMatrix;
//	for(int i=0; i<3; i++)
//	{
//		a[i]  =vertices[0]->getCoord((axisToSort)i);
//		bma[i]=vertices[1]->getCoord((axisToSort)i)-a[i];
//		cma[i]=vertices[2]->getCoord((axisToSort)i)-a[i];
//		dma[i]=v.getCoord((axisToSort)i)-a[i];
//		dinomMatrix[0][i]=bma[i];
//		dinomMatrix[1][i]=cma[i];
//		dinomMatrix[2][i]=dma[i];
//	}
//	detdinomMatrix=determinant3x3(dinomMatrix);
//	if(detdinomMatrix)
//	{
//		CROSS(tempcrs1,bma,cma);
//		CROSS(tempcrs2,dma,bma);
//		CROSS(tempcrs3,cma,dma);
//		lensqdma=vector3dLenSq(dma);
//		lensqcma=vector3dLenSq(cma);
//		lensqbma=vector3dLenSq(bma);
//		MULT(tempcrs1,tempcrs1,lensqdma);
//		MULT(tempcrs2,tempcrs2,lensqcma);
//		MULT(tempcrs3,tempcrs3,lensqbma);
//		ADD(radiusVector,tempcrs1,tempcrs2);
//		ADD(radiusVector,radiusVector,tempcrs3);
//		centerRadius[3]=sqrt(vector3dLenSq(radiusVector));
//		centerRadius[3]/=abs(2*detdinomMatrix);
//		MULT(radiusVector, radiusVector, (1/(2*detdinomMatrix)));
//		ADD(centerRadius,a,radiusVector);
//		return true;
//	}
//	else
//		return false;
//
//}
//double Face::testCircumSphereAdaptive(Vertex& v1, Vertex& v2)
//{
//	double p[3][3], d[3],e[3];
//	for(int i=0; i<3; i++)
//	{
//		for(int j=0; j<3; j++)
//		{
//			p[i][j]=vertices[i]->getCoord((axisToSort)j);
//		}
//	}
//	for(int i=0; i<3; i++)
//	{
//		d[i]=v1.getCoord((axisToSort)i);
//		e[i]=v2.getCoord((axisToSort)i);
//	}
//	return insphere(p[0],p[1],p[2],d,e);
//}

//deque<Vertex>::pointer* Face::getVertices()
//{
//	return vertices;
//}

deque<long>& Face::getEdges()
{
	return edges;
}

//bool Face::setNeighbourCell(deque<Cell>::pointer pCell)
//{
//	if(!cell1)
//		cell1=pCell;
//	else if(!cell2)
//		cell2=pCell;
//	else
//	{
//		cout<<"ERROR while setting neighbour"<<endl;
//		return false;
//	}
//	return true;
//}
//
//void Face::setNeighCell1(deque<Cell>::pointer cellPtr)
//{
//	cell1=cellPtr;
//}
//
//void Face::setNeighCell2(deque<Cell>::pointer cellPtr)
//{
//	cell2=cellPtr;
//}

//void Face::incrNumOfOpenFaces()
//{
//	for(int i=0; i<3; i++)
//	{
//		vertices[i]->incrNumOfOpenFaces();
//		//gridCoord=(unsigned int*)vertices[i]->sparePtr;
//		//grid[gridCoord[0]][gridCoord[1]][gridCoord[2]][gridCoord[3]]->incrNumOfOpenFaces();
//	}
//}

//void Face::decrNumOfOpenFaces()
//{
//	for(int i=0; i<3; i++)
//	{
//		vertices[i]->decrNumOfOpenFaces();
//		//gridCoord=(unsigned int*)vertices[i]->sparePtr;
//		//grid[gridCoord[0]][gridCoord[1]][gridCoord[2]][gridCoord[3]]->incrNumOfOpenFaces();
//	}
//}

//int Face::getNumOfOpenFaces()
//{
//	return (vertices[0]->getNumOfOpenFaces() && vertices[1]->getNumOfOpenFaces() && vertices[2]->getNumOfOpenFaces());
//}
//deque<Cell>::pointer Face::getNeighCell1()
//{
//	return cell1;
//}
//
//deque<Cell>::pointer Face::getNeighCell2()
//{
//	return cell2;
//}
//
//deque<Vertex>::pointer Face::getOppositeVertex1()
//{
//	return oppositeVertex1;
//}
//
//deque<Vertex>::pointer Face::getOppositeVertex2()
//{
//	return oppositeVertex2;
//}
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
Cell::Cell(Base3dSimplex* obj)
{
	this->iD = obj->getId();

}
Cell::Cell(Cell* obj)
{
	this->iD = obj->getId();
	for(int i=0; i<4; i++)
	{
		this->vertexIndices[i] = obj->getVertexIndices()[i];
		this->contained2dSimplexIndices[i] = obj->getContaining2dSimplices()[i];
	}

}

//Cell::Cell(int size)
//{
//	vertices.resize(size);
//	faces.resize(size);
//	//neighbours.resize(size);
//	edges.resize(size);
//}

//void Cell::setId(string cellId)
//{
//	id = cellId;
//}

//string Cell::getId()
//{
//	return id;
//}

//void Cell::insertVertex(deque<Vertex>::pointer vertex)
//{
//	vertices.push_back(vertex);
//}

void Cell::addEdge(long edgeId)
{
	edges.push_back(edgeId);
}

deque<long>& Cell::getEdges()
{
	return edges;
}

//deque<deque<Face>::pointer>& Cell::getFaces()
//{
//	return faces;
//}

//multimap<deque<Cell>::pointer, deque<Face>::pointer>& Cell::getNeighbours()
//{
//	return neighbours;
//}

//bool Cell::testCircumCircle(deque<Vertex>::pointer v, double centerRadius[4])
//{
//	return edges[0]->testCircumCircle(v,centerRadius);
//}
//
//double Cell::testCircumCircleAdaptive(Vertex& v1, Vertex& v2 )
//{
//	return edges[0]->testCircumCircleAdaptive(v1,v2);
//}
//
//trippleBool Cell::testCircumSphere(Vertex& v, double centerRadius[4])
//{
//	if(faces[0]->testCircumSphere(v,centerRadius))
//	{
//		Vertex ctr(centerRadius[0],centerRadius[1],centerRadius[2], false);
//		if(faces.back()->getOppositeVertex1() || faces.back()->getOppositeVertex2())
//		{
//			if(faces.back()->getOppositeVertex1())
//			{
//				if(faces.back()->getOppositeVertex1()->getSqDistance(ctr)<=centerRadius[3]*centerRadius[3])
////					return false_val;
//					return midstate_val;
////				if(faces.back()->getOppositeVertex1()->getSqDistance(ctr)==centerRadius[3]*centerRadius[3])
////					return midstate_val;
//			}
//			else
//			{
//				if(faces.back()->getOppositeVertex2()->getSqDistance(ctr)<=centerRadius[3]*centerRadius[3])
////					return false_val;
//					return midstate_val;
////				if(faces.back()->getOppositeVertex2()->getSqDistance(ctr)==centerRadius[3]*centerRadius[3])
////					return midstate_val;
//			}
//			//if(!this->checkOrientation(ctr))
//			if(!this->check3DOrientationAdaptive(ctr))
//			{
//				centerRadius[3]*=-1;
//			}
//		}
//		return true_val;
//	}
//	else
//		return false_val;
//}
//double Cell::testCircumSphereAdaptive(Vertex& v1, Vertex& v2)
//{
//	return faces[0]->testCircumSphereAdaptive(v1,v2);
//}
//void Cell::addFEVs(deque<Face>::pointer face)
//{
////	if(face->getId()=="17,32,33")
////		cout<<"detect culprit"<<endl;
//	for(int i=0; i<3; i++)
//	{
//		vertices.push_back(face->getVertices()[i]);
//		edges.push_back(face->getEdges()[i]);
//	}
//	addFace(face);
//}

//void Cell::delFEVs(deque<Face>::pointer face)
//{
//	vertices.clear();
//	edges.clear();
//	if(face->getNeighCell1()==this)
//		face->setNeighCell1(NULL);
//	else
//		face->setNeighCell2(NULL);
//}
//bool Cell::checkOrientation(Vertex& v2)
//{
//	double matrixp1[3][3];
//	double matrixp2[3][3];
//	Vertex *oppVertex;
//	if(faces.back()->getNeighCell1()==this)
//		oppVertex=faces.back()->getOppositeVertex2();
//	else if(faces.back()->getNeighCell2()==this)
//		oppVertex=faces.back()->getOppositeVertex1();
//	else
//		cout<<"ERROR ERROR::"<<endl;
//	for(int i=0; i<3; i++)
//		for(int j=0; j<3; j++)
//		{
//			matrixp2[i][j] = (faces.back()->getVertices()[i]->getCoord((axisToSort)j)) - (oppVertex->getCoord((axisToSort)j));
//			matrixp1[i][j] = (faces.back()->getVertices()[i]->getCoord((axisToSort)j)) - (v2.getCoord((axisToSort)j));
//		}
//	if(determinant3x3(matrixp1) * determinant3x3(matrixp2)<0)
//		return true;
//	else
//		return false;
//}
//
//bool Cell::check3DOrientationAdaptive(Vertex& v2)
//{
//	Vertex *oppVertex;
//	for(int i=0; i<3; i++)
//	{
//		if(faces.back()->getVertices()[i]->getId()==v2.getId())
//			return false;
//	}
//	if(this->getNeighListSize()>0)
//	{
//		if(faces.back()->getNeighCell1()==this)
//			oppVertex=faces.back()->getOppositeVertex2();
//		else if(faces.back()->getNeighCell2()==this)
//			oppVertex=faces.back()->getOppositeVertex1();
//		else
//			cout<<"possible ERROR ERROR::"<<endl;
//	}
//	else
//	{
//		double p[3][3], d1[3];
//		for(int i=0; i<3; i++)
//		{
//			for(int j=0; j<3; j++)
//			{
//				p[i][j] = faces.back()->getVertices()[i]->getCoord((axisToSort)j);
//			}
//		}
//		for(int i=0;i<3; i++)
//		{
//			d1[i] = v2.getCoord((axisToSort)i);
//		}
//		return orient3d(p[0],p[1],p[2],d1);
//	}
//	double p[3][3], d1[3], d2[3];
//	for(int i=0; i<3; i++)
//	{
//		for(int j=0; j<3; j++)
//		{
//			p[i][j] = faces.back()->getVertices()[i]->getCoord((axisToSort)j);
//		}
//	}
//	for(int i=0;i<3; i++)
//	{
//		d1[i] = v2.getCoord((axisToSort)i);
//		d2[i] = oppVertex->getCoord((axisToSort)i);
//	}
//	if(orient3d(p[0],p[1],p[2],d1)*orient3d(p[0],p[1],p[2],d2)<0)
//		return true;
//	else
//		return false;
//}
//bool Cell::check2DOrientationAdaptive(Vertex& v2)
//{
//	for(int i =0; i<2; i++)
//	{
//		if(edges.back()->getVertex()[i]->getId() == v2.getId())
//			return false;
//	}
//	return edges[0]->check2DOrientationAdaptive(v2);
//}
void Cell::setCircumCenter(double circumCenter[3])
{
	for(int i=0; i<3; i++)
		circumCtr[i]=circumCenter[i];
}

void Cell::setCircumRadius(double radius)
{
	circumRadius=radius;
}

void Cell::setDihedralAngles(Face *f1, Face *f2, Face *f3, Face *f4)
{
	string stringId;
	stringstream stringIdStream;
	double angle;
	double n1[3],n2[3],n3[3],n4[3];
	MULT(n1,f1->getNormal(),faceNormalDirectionSign.find(f1->getId())->second);
	MULT(n2,f2->getNormal(),faceNormalDirectionSign.find(f2->getId())->second);
	MULT(n3,f3->getNormal(),faceNormalDirectionSign.find(f3->getId())->second);
	MULT(n4,f4->getNormal(),faceNormalDirectionSign.find(f4->getId())->second);

	stringIdStream<<f1->getId()<<","<<f2->getId();
	angle = DOT(n1,n2);
	dihedralAngles.insert(pair<string,double>(stringIdStream.str(),acos(angle)));

	stringIdStream<<f1->getId()<<","<<f3->getId();
	angle = DOT(n1,n3);
	dihedralAngles.insert(pair<string,double>(stringIdStream.str(),acos(angle)));

	stringIdStream<<f1->getId()<<","<<f4->getId();
	angle = DOT(n1,n4);
	dihedralAngles.insert(pair<string,double>(stringIdStream.str(),acos(angle)));

	stringIdStream<<f2->getId()<<","<<f3->getId();
	angle = DOT(n2,n3);
	dihedralAngles.insert(pair<string,double>(stringIdStream.str(),acos(angle)));

	stringIdStream<<f2->getId()<<","<<f4->getId();
	angle = DOT(n2,n4);
	dihedralAngles.insert(pair<string,double>(stringIdStream.str(),acos(angle)));

	stringIdStream<<f3->getId()<<","<<f4->getId();
	angle = DOT(n3,n4);
	dihedralAngles.insert(pair<string,double>(stringIdStream.str(),acos(angle)));
}

void Cell::setFaceNormalDirectionSign(long id, int val)
{
	faceNormalDirectionSign.insert(pair<long,int>(id,val));
}

double Cell::getCircumRadius()
{
	return circumRadius;
}

double* Cell::getCircumCenter()
{
	return circumCtr;
}

int Cell::getFaceNormalDirectionSign(long id)
{
	return faceNormalDirectionSign.find(id)->second;
}
//void Cell::delVertices()
//{
//	vertices.clear();
//}
///Solid class

//void Solid::populateVertices(deque<Vertex>& vertexList)
//{
//	//cout<<listOfVertices.size();
//	//cout.flush();
//	deque<Vertex>::iterator vit;
//	long i=0;
//	for(vit=vertexList.begin(); vit!=vertexList.end(); vit++)
//	{
//		listOfVertices.insert(pair<long,Vertex>(i++,*vit));
//	}
//}
void Solid::populateVerticesRandom()
{
	double x,y,z;
	cout<<"vertices generated:"<<endl;
	for(long i=0;i<5;i++)
	{
		x= (randnum()-0.5)*20;
		y= (randnum()-0.5)*20;
		z= (randnum()-0.5)*20;
//		if(x*x+y*y+z*z>=10)
//		{
//			i--;
//			continue;
//		}
		Vertex* newVertex = new Vertex(x,y,z);
		newVertex->setVertexId(i);
		listOfVertices.insert(pair<long,Vertex>(i,*newVertex));
		cout<<x<<","<<y<<","<<z<<","<<x*x+y*y+z*z<<";"<<endl;
	}
}
void Solid::delaunize(qhull::convexHull4d *hull4d)
{
	deque<Vertex> vertices;
	deque<Vertex>::iterator vit;
	map<string,deque<Face>::pointer> afl;
	hull4d->populateVerticesList(listOfVertices);
	hull4d->makeHull();
	createSolid(hull4d);
}
void Solid::createSolid(qhull::convexHull4d *hull4d)
{
	for(map<long,qhull::ridge>::iterator rit= hull4d->getRidgeList().begin(); rit!=hull4d->getRidgeList().end(); rit++)
	{
		Face *newFace = new Face((Base2dSimplex*)(&rit->second));
		listOfFaces.insert(pair<long,Face>(newFace->getId(),*newFace));
		Face *insertedNewFace = &listOfFaces.find(newFace->getId())->second;
		double vector1[3], vector2[3], normal[3];
		for(int i=0; i<3; i++)
		{
			insertedNewFace->insertVertex(rit->second.getVertexList()[i]);
		}
		MINUS(vector1,listOfVertices.find(insertedNewFace->getVertexList()[1])->second.getCoords(),listOfVertices.find(insertedNewFace->getVertexList()[0])->second.getCoords());
		MINUS(vector2,listOfVertices.find(insertedNewFace->getVertexList()[2])->second.getCoords(),listOfVertices.find(insertedNewFace->getVertexList()[0])->second.getCoords());
		CROSS(normal,vector1,vector2)
		MULT(normal,normal,1/sqrt(vector3dLenSq(normal)));
		insertedNewFace->addNormalDir(normal);
	}

	for(map<long,qhull::facet>::iterator fit = hull4d->getFacetList().begin(); fit!=hull4d->getFacetList().end(); fit++)
	{
		Cell *newCell = new Cell((Base3dSimplex*)(&fit->second));
		int normalDirSign;
		listOfCells.insert(pair<long,Cell>(newCell->getId(),*newCell));
		Cell *insertedNewCell = &listOfCells.find(newCell->getId())->second;
		for(int i=0; i<4; i++)
		{
			insertedNewCell->insertVertex(fit->second.getVertexIndices()[i]);
			insertedNewCell->insertContaining2dSimplex(fit->second.getContaining2dSimplices()[i]);
		}
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)
			{
				if(find(listOfFaces.find(fit->second.getContaining2dSimplices()[i])->second.getVertexList().begin(),
						listOfFaces.find(fit->second.getContaining2dSimplices()[i])->second.getVertexList().end(),
						insertedNewCell->getVertexIndices()[j])
					  ==listOfFaces.find(fit->second.getContaining2dSimplices()[i])->second.getVertexList().end())
				{
					normalDirSign = testNormalDir(&listOfFaces.find(fit->second.getContaining2dSimplices()[0])->second,
												  &listOfVertices.find(insertedNewCell->getVertexIndices()[j])->second) ? 1:-1;

					insertedNewCell->setFaceNormalDirectionSign(listOfFaces.find(fit->second.getContaining2dSimplices()[0])->first,
																normalDirSign);
					break;
				}
			}
		}
		insertedNewCell->setDihedralAngles(&listOfFaces.find(fit->second.getContaining2dSimplices()[0])->second,
										   &listOfFaces.find(fit->second.getContaining2dSimplices()[1])->second,
										   &listOfFaces.find(fit->second.getContaining2dSimplices()[2])->second,
										   &listOfFaces.find(fit->second.getContaining2dSimplices()[3])->second);
	}
}
void Solid::drawEdges()
{
	deque<deque<Edge>::pointer>::iterator eit;
	//deque<deque<Face>::pointer>::iterator fit;
//	deque<Vertex>::iterator vit;
	map<long,Vertex>::iterator vit;
	map<long,Face>::iterator fit;
//	map<long,qhull::vertex4d>::iterator v4it;
//	map<long,qhull::ridge>::iterator rit;
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
//	for(vit=listOfVertices.begin(); vit!=listOfVertices.end(); vit++)
	for(vit=listOfVertices.begin(); vit!=listOfVertices.end(); vit++)
	{
		glPushMatrix();
		glColor4f(0.0,1.0,0.0,1.0);
//		glTranslated(vit->getXCoord()/resolution, vit->getYCoord()/resolution, vit->getZCoord()/resolution);
		double *vertices;
		vertices = vit->second.getCoords();
		glTranslated(vertices[0]/resolution, vertices[1]/resolution, vertices[2]/resolution);
		glutSolidSphere(0.05,10,10);
		glPopMatrix();
	}
//	cout<<"Number of faces:"<<hull4d->getRidgeList().size()<<endl;
	for(fit = listOfFaces.begin(); fit!=listOfFaces.end(); fit++)
	{

		double vertList[3][3];
		int i=0;
//		cout<<"viewing:"<<rit->second.getId()<<endl;
		for(deque<long>::iterator faceVit= fit->second.getVertexList().begin(); faceVit!= fit->second.getVertexList().end(); i++,faceVit++ )
		{
//			cout<<"curr ridgeVit:"<<*ridgeVit<<endl;
			for(int j=0; j<3; j++)
			{
				vertList[i][j]=listOfVertices.find(*faceVit)->second.getCoords()[j];
			}
		}
		glColor4f(1.0,0.0,0.0,1.0);
//		cout<<"first vertex:"<<vertList[0][0]<<","<<vertList[0][1]<<","<<vertList[0][2]<<endl;
//		cout<<"sec vertex:"<<vertList[1][0]<<","<<vertList[1][1]<<","<<vertList[1][2]<<endl;
//		cout<<"third vertex:"<<vertList[2][0]<<","<<vertList[2][1]<<","<<vertList[2][2]<<endl;
		glBegin(GL_LINES);
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				if(i==j)
					continue;
				glVertex3d(vertList[j][0]/resolution,vertList[j][1]/resolution,vertList[j][2]/resolution);
			}
		}
		glEnd();
	}
//	for(int i=firstCell; i<lastCell; i++)
//	{
//		double *center;
//		glColor4f(1.0,0.0,0.0,1.0);
//		for(eit=listOfCells[i].getEdges().begin(); eit!=listOfCells[i].getEdges().end(); eit++)
//		{
//			glColor4f(1.0,0.0,0.0,1.0);
//			glBegin(GL_LINES);
//			glVertex3d((*eit)->getVertex()[0]->getXCoord()/resolution, (*eit)->getVertex()[0]->getYCoord()/resolution, (*eit)->getVertex()[0]->getZCoord()/resolution);
//			glVertex3d((*eit)->getVertex()[1]->getXCoord()/resolution, (*eit)->getVertex()[1]->getYCoord()/resolution, (*eit)->getVertex()[1]->getZCoord()/resolution);
//			glEnd();
//		}
//		if(showCircle)
//		{
//			center=listOfCells[i].getCircumCenter();
////			if(listOfCells[i].getVertexListSize()==4)
////			{
//				glPushMatrix();
//				glTranslated(center[0]/resolution,center[1]/resolution,center[2]/resolution);
//				if(wire)
//					glutWireSphere(listOfCells[i].getCircumRadius()/resolution,40,40);
//				else
//					glutSolidSphere(listOfCells[i].getCircumRadius()/resolution,40,40);
//				glPopMatrix();
////			}
////			else
////			{
////				glPushMatrix();
////				glBegin(GL_LINE_LOOP);
////				glTranslated(center[0]/resolution,center[1]/resolution,center[2]/resolution);
////				for (int ic=0; ic < 360; ic++)
////				{
////					double degInRad = ic*(3.14159/180);
////					glVertex3d(cos(degInRad)*(listOfCells[i].getCircumRadius()/resolution),center[1],sin(degInRad)*(listOfCells[i].getCircumRadius()/resolution));
////				}
////				glEnd();
////				glPopMatrix();
////			}
//		}
//	}
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
			newVertex = new Vertex(x,y,z);
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
bool Solid::testNormalDir(Face* f,Vertex* v)
{
	double testVector[3];
	MINUS(testVector,listOfVertices.find(f->getVertexList()[0])->second.getCoords(),v->getCoords());
	return DOT(f->getNormal(),testVector)<0;
}
