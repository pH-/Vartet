/*
 * quickhull.cpp
 *
 *  Created on: Sep 30, 2011
 *      Author: phanindrabhagavatula
 */
#include "includes.h"
using namespace qhull;
//#define Print
//vertex functions
void convexHull4d::printMap()
{
	map<long,facet>::iterator mit;
	vector<long> faceVertices;
	cout<<"Printing map"<<endl;
	ofstream outfile("output.off");
	outfile<<verticesOfHull.size()<<" vertices"<<"\r\n"<<tempFacets.size()<<" tets"<<"\r\n";
	int i=0;
	for(map<long,vertex4d>::iterator vit= verticesOfHull.begin(); vit!= verticesOfHull.end(); vit++)
	{
		outfile<<vit->second.getCoords()[0]<<" "<<vit->second.getCoords()[1]<<" "<<vit->second.getCoords()[2]<<"\r\n";
	}
	for(map<long,ridge>::iterator rit = tempRidges.begin(); rit!=tempRidges.end(); rit++)
	{
//		outfile<<"4 ";
//		i=0;
//		for(deque<long>::iterator vit = rit->second.getVertexList().begin(); vit!= rit->second.getVertexList().end(); vit++,i++)
//		{
//			if(i<2)
//				outfile<<*vit<<" ";
//			else
//				outfile<<*vit<<endl;
//		}
	}
	for(mit=tempFacets.begin(); mit!=tempFacets.end(); mit++,i++)
	{
		outfile<<"4 ";
		outfile<< mit->second.getVertexIndices()[0] <<" "<<mit->second.getVertexIndices()[1]<<" "<<mit->second.getVertexIndices()[2]<<" "<<mit->second.getVertexIndices()[3]<<"\r\n";
	}
	outfile.close();
	i=0;
	for(mit=tempFacets.begin(); mit!=tempFacets.end(); mit++,i++)
	{
#ifdef Print
		cout<<"facet Id "<< i+1<<":"<<mit->first<<":";
		for(deque<long>::iterator bit = mit->second.getBucketVertices().begin(); bit!=mit->second.getBucketVertices().end(); bit++)
		{
			cout<<*bit<<",";
		}
		cout<<"\t Ridge ids: ";
#endif
		for(deque<long>::iterator rit = mit->second.getFacetRidges().begin(); rit!=mit->second.getFacetRidges().end(); rit++)
		{
#ifdef Print
			cout<<*rit<<",";
#endif
			for(deque<long>::iterator vit = tempRidges.find(*rit)->second.getVertexList().begin(); vit!= tempRidges.find(*rit)->second.getVertexList().end(); vit++)
			{
				if(find(faceVertices.begin(), faceVertices.end(), *vit)==faceVertices.end())
					faceVertices.push_back(*vit);
			}
		}
#ifdef Print
		cout<<"\t Vertex ids: ";
#endif
		cout<<faceVertices[0]<<","<<faceVertices[1]<<","<<faceVertices[2]<<","<<faceVertices[3]<<";";
		faceVertices.clear();
#ifdef Print
		cout<<"\t Normal:"<<mit->second.getNormal()[0]<<","<<mit->second.getNormal()[1]<<","<<mit->second.getNormal()[2]<<","<<mit->second.getNormal()[3];
#endif
		cout<<endl;
//		if(i%1000==0)
//			cout<<"take a break";
	}
}
vertex4d::vertex4d(double x, double y, double z, double w)
{
	coords4d[0]=x;
	coords4d[1]=y;
	coords4d[2]=z;
	coords4d[3]=w;
	bucketStatus = false;
}
double* vertex4d::getCoords()
{
	return coords4d;
}

bool vertex4d::isAbove(facet* f)
{
	return DOT4D(this->getCoords(),f->getNormal())+f->getOffset() > 0;
}
void vertex4d::setAssignedToBucket(bool newValue)
{
	bucketStatus = newValue;
}
bool vertex4d::getBucketAssignmentStatus()
{
	return bucketStatus;
}

deque<long>& vertex4d::getVisibleFacets()
{
	return visibleFacets;
}
long vertex4d::getId()
{
	return vertexId;
}
void vertex4d::insertVisibleFacet(long f)
{
	if(find(visibleFacets.begin(), visibleFacets.end(), f)==visibleFacets.end())
		visibleFacets.push_back(f);
}
void vertex4d::setVertexId(long id)
{
	vertexId = id;
}

//ridge functions
ridge::ridge()
{
	neighFacet1=0l;
	neighFacet2=0l;
	ridgeId=0l;
}
long ridge::getNeighbour1()
{
	return neighFacet1;
}

long ridge::getNeighbour2()
{
	return neighFacet2;
}

deque<long>& ridge::getVertexList()
{
	return ridgeVertices;
}
long ridge::getId()
{
	return ridgeId;
}
void ridge::setNeighbour1(long f)
{
	neighFacet1 = f;
}

void ridge::setNeighbour2(long f)
{
	neighFacet2 = f;
}

void ridge::insertVertex(long v)
{
	ridgeVertices.push_back(v);
}

void ridge::setId(long id)
{
	ridgeId = id;
}
//void ridge::setId(const vector<long>* vertexIds)
//{
//	long chksum =1;
//	for(unsigned int i=0; i<(*vertexIds).size(); i++)
//	{
//		long eachNum=(*vertexIds)[i];
//		chksum = (31*chksum+eachNum)%1000003;
//	}
//	ridgeId = chksum;
//}
//facet functions
facet::facet()
{
	furthestPoint=0;
	id=0;
	offset=0;
}
double* facet::getNormal()
{
	return normal;
}

double facet::getOffset()
{
	return offset;
}

deque<long >& facet::getBucketVertices()
{
	return bucket;
}

deque<long>& facet::getFacetRidges()
{
	return facetRidges;
}
deque<long>& facet::getVertexIndices()
{
	return vertexIndices;
}
long facet::getId()
{
	return id;
}
long facet::getFurthestPoint()
{
	return furthestPoint;
}
void facet::setNormal(double normalVector[4])
{
	for(int i=0; i<4; i++)
		normal[i]=normalVector[i];
}

//void facet::setOffset(double offsetVector[4])
//{
//	ASSIGN4D(offset,offsetVector);
//}

void facet::putInBucket(long v)
{
	bucket.push_back(v);
}

void facet::insertRidge(long r)
{
	facetRidges.push_back(r);
}

void facet::calcNormOffset(const deque<vertex4d>::pointer v[4], double* testVertex)
{
	double crossProdMatrix[3][4];
	double tempVector[4];
	for(int i=0; i<3; i++)
	{
		MINUS4D(crossProdMatrix[i],v[i+1]->getCoords(),v[0]->getCoords());
	}
	double determinantMatrix[3][3];
	double crossProduct[4];
	for(int k=0; k<4; k++)
	{
		for(int i=0,j=0; i<4; i++,j++)
		{
			if(i==k)
			{
				j--;
				continue;
			}
			determinantMatrix[0][j]=crossProdMatrix[0][i];
			determinantMatrix[1][j]=crossProdMatrix[1][i];
			determinantMatrix[2][j]=crossProdMatrix[2][i];
		}
		crossProduct[k]= pow((-1.0),k)*determinant3x3(determinantMatrix);
	}
	double normalLen = vector4dLen(crossProduct);
	MINUS4D(tempVector,testVertex,v[0]->getCoords());
	double direction=DOT4D(crossProduct, tempVector);
	direction = direction<0?1:-1;
	if(direction==-1)
	{
		vertexIndices.push_back(v[0]->getId());
		vertexIndices.push_back(v[1]->getId());
		vertexIndices.push_back(v[2]->getId());
		vertexIndices.push_back(v[3]->getId());
	}
	else
	{
		vertexIndices.push_back(v[0]->getId());
		vertexIndices.push_back(v[1]->getId());
		vertexIndices.push_back(v[3]->getId());
		vertexIndices.push_back(v[2]->getId());
	}
	MULT4D(normal,crossProduct,direction/normalLen);
	offset=-1*DOT4D(normal,v[0]->getCoords());
}


void facet::setId(long idValue)
{
	id=idValue;
}

void facet::setFurthestPoint(long v)
{
	furthestPoint = v;
}

//convexHull functions

map<long,facet>& convexHull4d::getFacetList()
{
	return tempFacets;
}

map<long,ridge>& convexHull4d::getRidgeList()
{
	return tempRidges;
}

map<long,vertex4d>& convexHull4d::getVertexList()
{
	return verticesOfHull;
}

void convexHull4d::insertVertex(vertex4d v)
{
	verticesOfHull.insert(pair<long,vertex4d>(v.getId(),v));
}

void convexHull4d::populateVerticesList(deque<Vertex> origList)
{
	for(unsigned int i=0; i<origList.size(); i++)
	{
		double coordDim4 = pow(origList[i].getXCoord(),2) + pow(origList[i].getYCoord(),2) + pow(origList[i].getZCoord(),2);
		vertex4d *newVertex = new vertex4d(origList[i].getXCoord(), origList[i].getYCoord(), origList[i].getZCoord(), coordDim4);
		newVertex->setVertexId(i);
		insertVertex(*newVertex);
	}
}

void convexHull4d::makeInitSimplex()
{
	deque<vertex4d>::pointer simplexVertices[5];
	facet					*newFacets[5];
//	deque<ridge>::pointer 	 newRidges[10];
//	for(int i=0; i<5; i++)
//			tempHull.insert((pair<string,deque<facet>>)(newFacets[i]->getId, *newFacets[i]));
	map<long,vertex4d>::iterator v4dit;
	int i;
	for(v4dit = verticesOfHull.begin(),i=0;  i<5; v4dit++,i++)
	{
		v4dit->second.setAssignedToBucket(true);
		simplexVertices[i]=&v4dit->second;
	}
//	for(int i=0; i<5; i++)
//	{
//		facet *newFacet = new facet();
//		facetsOfHull.push_back(*newFacet);
//		newFacets[i] = &facetsOfHull.back();
//	}
//	for(int i=0; i<10; i++)
//	{
////		ridge *newRidge = new ridge();
////		ridgesOfHull.push_back(*newRidge);
////		newRidges[i] = &ridgesOfHull.back();
//	}
	deque<vertex4d>::pointer facetVertices[4];
	for(int i=0; i<5; i++)
	{
		vector<long> vid(4);
		vid.clear();
		for(int j=0,k=0; j<5; j++,k++)
		{
			if(j==i)
			{
				k--;
				continue;
			}
			facetVertices[k]=simplexVertices[j];
			vid.push_back(j);
		}
//		facetId=;
//		facetId.erase(facetId.size()-1,1);

		facet *newFacet = new facet();
		sort(vid.begin(), vid.end());
		newFacet->setId(generateId(&vid));
		tempFacets.insert(pair<long,facet>(newFacet->getId(), *newFacet));
		newFacets[i]=&tempFacets.find(newFacet->getId())->second;
		newFacets[i]->calcNormOffset(facetVertices, simplexVertices[i]->getCoords());
	}
	for(int i=0; i<10; i++)
	{
		ridge *newRidge = new ridge();
		vector<long> vertexIds;
		if(i<4)
		{
			newRidge->setNeighbour1(newFacets[0]->getId());
			newRidge->setNeighbour2(newFacets[i+1]->getId());
			for(int j=1; j<5; j++)
			{
				if(j==i+1)
					continue;
				newRidge->insertVertex(simplexVertices[j]->getId());
				vertexIds.push_back(j);
			}
		}
		else if(i<7)
		{
			newRidge->setNeighbour1(newFacets[1]->getId());
			newRidge->setNeighbour2(newFacets[i-2]->getId());
			for(int j=0; j<5; j++)
			{
				if(j==i-2 || j==1)
					continue;
				newRidge->insertVertex(simplexVertices[j]->getId());
				vertexIds.push_back(j);
			}
		}
		else if(i<9)
		{
			newRidge->setNeighbour1(newFacets[2]->getId());
			newRidge->setNeighbour2(newFacets[i-4]->getId());
			for(int j=0; j<5; j++)
			{
				if(j==i-4 || j==2)
					continue;
				newRidge->insertVertex(simplexVertices[j]->getId());
				vertexIds.push_back(j);
			}
		}
		else
		{
			newRidge->setNeighbour1(newFacets[3]->getId());
			newRidge->setNeighbour2(newFacets[i-5]->getId());
			for(int j=0; j<5; j++)
			{
				if(j==i-5 || j==3)
					continue;
				newRidge->insertVertex(simplexVertices[j]->getId());
				vertexIds.push_back(j);
			}
		}
		sort(vertexIds.begin(), vertexIds.end());
		newRidge->setId(generateId(&vertexIds));
		tempFacets.find(newRidge->getNeighbour1())->second.insertRidge(newRidge->getId());
		tempFacets.find(newRidge->getNeighbour2())->second.insertRidge(newRidge->getId());
		tempRidges.insert(pair<long,ridge>(newRidge->getId(),*newRidge));
	}
}
void convexHull4d::calcCentroid()
{
	map<long,vertex4d>::iterator vit;
	map<long,ridge>::iterator rit;
	ZERO4D(centroid);
	int j=5;
	for(vit = verticesOfHull.begin(); j--; vit++)
	{
		for(int i=0; i<4; i++)
			centroid[i]+=vit->second.getCoords()[i]/5;
	}
}
void convexHull4d::makeHull()
{
	calcCentroid();
	makeInitSimplex();
	initBucketing();
	map<long,facet>::iterator thit;
	for(thit = tempFacets.begin(); thit!=tempFacets.end(); thit++)
	{
		setFurthestPoint((*thit).second.getId());
	}
	unsigned int endCounter=0;
	vector<long> facetIdList;
	for(thit = tempFacets.begin(); thit!=tempFacets.end(); thit++)
	{
		if(thit->second.getBucketVertices().size())
			facetIdList.push_back((*thit).first);
	}
	vector<long>::iterator currFacet;
	deque<long>::iterator fit;
	deque<long > horizonRidges;
	deque<long>::iterator hrit;
	deque<long> discardedFacets;
	deque<long > bucketLessVertices;
	deque<long>::iterator blvit;
	double testNormal[4];

	while(true)
	{
		currFacet = facetIdList.begin();
		if(!facetIdList.size())
			break;
//		thit=tempFacets.begin();
		if(tempFacets.find(*currFacet)==tempFacets.end() || tempFacets.find(*currFacet)->second.getBucketVertices().size()==0)
		{
			facetIdList.erase(currFacet);
			continue;
		}
//		for(unsigned int i=0; i<facetIdList.size(); i++)
//		{
//			cout<<"facetIds in tempList:"<<facetIdList[i]<<endl;
//		}
//		printMap();
		ASSIGN4D(testNormal,tempFacets.find(*currFacet)->second.getNormal());
		endCounter=0;
		vertex4d *furthest =&( verticesOfHull.find(tempFacets.find(*currFacet)->second.getFurthestPoint())->second );
//		cout<<verticesOfHull.find(tempFacets.find(*currFacet)->second.getFurthestPoint())->second.getId()<<endl;
		furthest->insertVisibleFacet(*currFacet);
		facetIdList.erase(currFacet);
//set Visible facets
		setVisibleFacetsForVertex(furthest->getId());
		for(fit = furthest->getVisibleFacets().begin(); fit!=furthest->getVisibleFacets().end(); fit++)
		{
			discardedFacets.push_back(*fit);
//			cout<<"facets visible:"<<(*fit)<<endl;
		}
// get horizon ridges
		getHorizonRidges(horizonRidges,furthest);
//		for(hrit = horizonRidges.begin(); hrit!= horizonRidges.end(); hrit++)
//		{
////			cout<<"Horizon Ridge  "<<(*hrit)<<":";
//			for(deque<long>::iterator vit = tempRidges.find(*hrit)->second.getVertexList().begin(); vit != tempRidges.find(*hrit)->second.getVertexList().end(); vit++)
//			{
//				cout<<*vit<<",";
//			}
//			cout<<endl;
//		}
//		for(fit = discardedFacets.begin(); fit!=discardedFacets.end(); fit++)
//		{
//			cout<<"discarded facets:"<<(*fit)<<endl;
//		}
//delete discarded facets
		deleteDiscardedFacets(discardedFacets, furthest, bucketLessVertices, horizonRidges);
//create new facets
//		cout<<"size of horizon ridgeS:"<<horizonRidges.size()<<endl;
		createNewFacets(horizonRidges, furthest, facetIdList, bucketLessVertices, testNormal);

		horizonRidges.clear();
//		cout<<"size of horizon ridgeS after clearing:"<<horizonRidges.size()<<endl;

		discardedFacets.clear();
		bucketLessVertices.clear();
	}
	finalCleaning();
//	for(map<long,vertex4d>::iterator v4dit = getVertexList().begin(); v4dit!=getVertexList().end(); v4dit++)
//	{
//		cout<<"coords  "<<v4dit->second.getId()<<":"<<v4dit->second.getCoords()[0]<<","<<v4dit->second.getCoords()[1]<<","<<v4dit->second.getCoords()[2]<<endl;
//	}
	cout<<"final output"<<endl;
	printMap();
}
void convexHull4d::createNewFacets(deque<long>& horizonRidges, vertex4d* furthest, vector<long>& facetIdList, deque<long>& bucketLessVertices, double testNormal[4])
{
//	cout<<"size of horizon ridgeS when creating:"<<horizonRidges.size()<<endl;
	for(deque<long>::iterator hrit=horizonRidges.begin(); hrit!= horizonRidges.end(); hrit++)
	{
		vector<long> vertexIndices;
		vector<long> ridgeIdsForFacet(4);
//			cout<<"\tsomevertices"<<endl;
//			cout<<tempRidges.find((*hrit))->second.getVertexList()[0]<<endl;
//			cout<<tempRidges.find((*hrit))->second.getVertexList()[1]<<endl;
//			cout<<tempRidges.find((*hrit))->second.getVertexList()[2]<<endl;
		vertexIndices.push_back(tempRidges.find((*hrit))->second.getVertexList()[0]);
		vertexIndices.push_back(tempRidges.find((*hrit))->second.getVertexList()[1]);
		vertexIndices.push_back(tempRidges.find((*hrit))->second.getVertexList()[2]);
		vertexIndices.push_back(furthest->getId());
		sort(vertexIndices.begin(), vertexIndices.end());
//		cout<<"Vertices of Facet:";
//		for(unsigned int i=0; i<vertexIndices.size(); i++)
//		{
//			cout<<vertexIndices[i]<<",";
//		}
//		cout<<endl;
		long facetId = generateId(&vertexIndices);
		vertexIndices.clear();
		facet *newFacet = new facet();
		newFacet->setId(facetId);
		tempFacets.insert(pair<long,facet>(facetId, *newFacet));
		newFacet=&tempFacets.find(facetId)->second;
		if(tempRidges.find((*hrit))->second.getNeighbour1())
		{
			tempRidges.find((*hrit))->second.setNeighbour2(newFacet->getId());
			newFacet->insertRidge(*hrit);
		}
		else
		{
			tempRidges.find((*hrit))->second.setNeighbour1(newFacet->getId());
			newFacet->insertRidge(*hrit);
		}
		for(int i=0; i<3; i++)
		{
			for(int j=0,k=0; k<2; j++,k++)
			{
				if(j==i)
				{
					k--;
					continue;
				}
				vertexIndices.push_back(tempRidges.find((*hrit))->second.getVertexList()[j]);
			}
			vertexIndices.push_back(furthest->getId());
			sort(vertexIndices.begin(),vertexIndices.end());
			ridge *newRidge;
//			newRidge->setId(&vertexIndices);
			long tempId = generateId(&vertexIndices);
//			cout<<"newRidge id:"<<tempId<<endl<<"Vertices of Ridge:";
//			for(unsigned int i=0; i<vertexIndices.size(); i++)
//			{
//				cout<<vertexIndices[i]<<",";
//			}
//			cout<<endl;

			if(tempRidges.find(tempId)==tempRidges.end())
			{
				newRidge = new ridge();
				newRidge->setId(tempId);
				tempRidges.insert(pair<long,ridge>(tempId,*newRidge));
				newRidge = &tempRidges.find(tempId)->second;
				for(vector<long>::iterator viter = vertexIndices.begin(); viter!=vertexIndices.end(); viter++)
				{
					newRidge->getVertexList().push_back(*viter);
				}
			}
			else
			{
//				free(newRidge);
				newRidge = &tempRidges.find(tempId)->second;
			}
//			newRidge = &tempRidges.find(newRidge->getId())->second;
			vertexIndices.clear();
			if(newRidge->getNeighbour1()==0)
				newRidge->setNeighbour1(newFacet->getId());
			else
				newRidge->setNeighbour2(newFacet->getId());
			newFacet->insertRidge(newRidge->getId());
		}
		deque<vertex4d>::pointer v[4];
		v[0]= &(verticesOfHull.find(furthest->getId())->second);
		for(int i=1; i<4; i++)
		{
			v[i]=&(verticesOfHull.find(tempRidges.find(*hrit)->second.getVertexList()[i-1])->second);
		}
		newFacet->calcNormOffset(v, centroid);
		facetIdList.push_back(newFacet->getId());
		for(deque<long>::iterator blvit=bucketLessVertices.begin(); blvit!=bucketLessVertices.end(); blvit++)
		{
			if(!verticesOfHull.find(*blvit)->second.getBucketAssignmentStatus() &&verticesOfHull.find(*blvit)->second.isAbove(newFacet))
			{
				newFacet->putInBucket((*blvit));
				verticesOfHull.find(*blvit)->second.insertVisibleFacet(newFacet->getId());
				verticesOfHull.find(*blvit)->second.setAssignedToBucket(true);
//					bucketLessVertices.erase(blvit);
			}
		}
		setFurthestPoint(newFacet->getId());
	}
	for(deque<long>::iterator blvit=bucketLessVertices.begin(); blvit!=bucketLessVertices.end(); blvit++)
	{
		if(verticesOfHull.find(*blvit)->second.getBucketAssignmentStatus()==false)
		{
			verticesOfHull.find(*blvit)->second.getVisibleFacets().clear();
			verticesOfHull.find(*blvit)->second.setAssignedToBucket(true);
		}
	}
}
void convexHull4d::deleteDiscardedFacets(deque<long>& discardedFacets, vertex4d* furthest, deque<long>& bucketLessVertices, deque<long>& horizonRidges)
{
	for(deque<long>::iterator fit = discardedFacets.begin(); fit!=discardedFacets.end(); fit++)
	{
		deque<long >::iterator rit;
		deque<long >::iterator buckIt;
		for(buckIt=tempFacets.find((*fit))->second.getBucketVertices().begin(); buckIt!=tempFacets.find((*fit))->second.getBucketVertices().end(); buckIt++)
		{
			if(*buckIt!=furthest->getId())
			{
				verticesOfHull.find(*buckIt)->second.setAssignedToBucket(false);
				bucketLessVertices.push_back((*buckIt));
				verticesOfHull.find(*buckIt)->second.getVisibleFacets().clear();
			}
		}
		for(rit = tempFacets.find((*fit))->second.getFacetRidges().begin(); rit!=tempFacets.find((*fit))->second.getFacetRidges().end(); rit++)
		{
//				cout<<"ridge to delete:"<<*rit<<endl;
			if(find(horizonRidges.begin(), horizonRidges.end(), (*rit))==horizonRidges.end() && tempRidges.find(*rit)!=tempRidges.end())
			{
				tempRidges.find((*rit))->second.getVertexList().clear();
				tempRidges.find((*rit))->second.setNeighbour1(0);
				tempRidges.find((*rit))->second.setNeighbour2(0);
				tempRidges.erase((*rit));
			}
		}
//			for(buckIt= tempFacets.find(*fit)->second.getBucketVertices().begin() ; buckIt!=tempFacets.find(*fit)->second.getBucketVertices().end(); buckIt++)
//			{
//				if(find(bucketLessVertices.begin(),bucketLessVertices.end(),(*buckIt))!=bucketLessVertices.end())
//					bucketLessVertices.push_back(*buckIt);
//			}
		tempFacets.erase((*fit));
	}
}
void convexHull4d::getHorizonRidges(deque<long>& horizonRidges, vertex4d* furthest)
{
	for(deque<long>::iterator fit = furthest->getVisibleFacets().begin(); fit!=furthest->getVisibleFacets().end(); fit++)
	{
		deque<long>::iterator rit;
		for(rit = tempFacets.find((*fit))->second.getFacetRidges().begin() ; rit!=tempFacets.find((*fit))->second.getFacetRidges().end(); rit++)
		{
			bool isFirstNeighbour = tempRidges.find((*rit))->second.getNeighbour1()==(*fit);
			long neigh;
			if(isFirstNeighbour)
				neigh = tempRidges.find((*rit))->second.getNeighbour2();
			else
				neigh = tempRidges.find((*rit))->second.getNeighbour1();
			if(find(furthest->getVisibleFacets().begin(),furthest->getVisibleFacets().end(),neigh)==furthest->getVisibleFacets().end()
			   && find(horizonRidges.begin(), horizonRidges.end(),*rit)==horizonRidges.end())
			{
				horizonRidges.push_back(*rit);
				if(isFirstNeighbour)
					tempRidges.find((*rit))->second.setNeighbour1(0);
				else
					tempRidges.find((*rit))->second.setNeighbour2(0);
			}
		}
	}
}
void convexHull4d::setVisibleFacetsForVertex(long vertexId)
{
	for(deque<long>::iterator fit = verticesOfHull.find(vertexId)->second.getVisibleFacets().begin(); fit!=verticesOfHull.find(vertexId)->second.getVisibleFacets().end(); fit++)
	{
//		cout<<"curr num of visible facets:"<<verticesOfHull.find(vertexId)->second.getVisibleFacets().size()<<endl;
		deque<long>::iterator rit;
		for(rit = tempFacets.find((*fit))->second.getFacetRidges().begin() ; rit!=tempFacets.find((*fit))->second.getFacetRidges().end(); rit++)
		{
			long neigh = tempRidges.find((*rit))->second.getNeighbour1()==(*fit)?tempRidges.find((*rit))->second.getNeighbour2():tempRidges.find((*rit))->second.getNeighbour1();
//			if(neigh==955364)
//				cout<<"chek here"<<endl;
			if(verticesOfHull.find(vertexId)->second.isAbove(&tempFacets.find(neigh)->second) &&
			   find(verticesOfHull.find(vertexId)->second.getVisibleFacets().begin(), verticesOfHull.find(vertexId)->second.getVisibleFacets().end(),(neigh))==verticesOfHull.find(vertexId)->second.getVisibleFacets().end())
			{
				verticesOfHull.find(vertexId)->second.insertVisibleFacet(neigh);
//				discardedFacet.push_back(neigh);
			}
		}
	}
}
void convexHull4d::setFurthestPoint(long facetId)
{
	deque<long>::iterator bucketIt;
	double maxDist = numeric_limits<double>::min();
	double dist;
	for(bucketIt = tempFacets.find(facetId)->second.getBucketVertices().begin(); bucketIt!=tempFacets.find(facetId)->second.getBucketVertices().end(); bucketIt++)
	{
		dist = DOT4D( verticesOfHull.find((*bucketIt))->second.getCoords(),tempFacets.find(facetId)->second.getNormal())+tempFacets.find(facetId)->second.getOffset();
		if(dist>maxDist)
		{
			maxDist = dist;
			tempFacets.find(facetId)->second.setFurthestPoint(*bucketIt);
		}
	}
}

void convexHull4d::finalCleaning()
{
	map<long,facet>::iterator facetIter;
	deque<long> discardedFacet;
	deque<long>::iterator fit;
	double refVector[4] = {0,0,0,-1};
	for(facetIter = tempFacets.begin(); facetIter!=tempFacets.end(); facetIter++)
	{
//		double *normal = facetIter->second.getNormal();
		if(DOT(facetIter->second.getNormal(),refVector)<0)
		{
			cout<<"found an opposite facet."<<endl;
			discardedFacet.push_back(facetIter->first);
		}
	}
	for(fit = discardedFacet.begin(); fit!=discardedFacet.end(); fit++)
	{
		deque<long >::iterator rit;
		deque<long >::iterator buckIt;
		for(buckIt=tempFacets.find((*fit))->second.getBucketVertices().begin(); buckIt!=tempFacets.find((*fit))->second.getBucketVertices().end(); buckIt++)
		{
			verticesOfHull.find(*buckIt)->second.setAssignedToBucket(false);
		}
		for(rit = tempFacets.find((*fit))->second.getFacetRidges().begin(); rit!=tempFacets.find((*fit))->second.getFacetRidges().end(); rit++)
		{
			if(tempRidges.find(*rit)==tempRidges.end())
				continue;
//			cout<<"ridge to delete:"<<*rit<<endl;
			bool isFirstNeighbour = tempRidges.find((*rit))->second.getNeighbour1()==(*fit);
			long neigh;
			if(isFirstNeighbour)
				neigh = tempRidges.find((*rit))->second.getNeighbour2();
			else
				neigh = tempRidges.find((*rit))->second.getNeighbour1();
			if(find(discardedFacet.begin(), discardedFacet.end(), neigh)==discardedFacet.end() && tempRidges.find(*rit)!=tempRidges.end())
			{
				tempRidges.find((*rit))->second.getVertexList().clear();
				tempRidges.find((*rit))->second.setNeighbour1(0);
				tempRidges.find((*rit))->second.setNeighbour2(0);
				tempRidges.erase((*rit));
			}
		}
		tempFacets.erase((*fit));
	}
}
void convexHull4d::initBucketing()
{
	map<long,vertex4d>::iterator v4dit;
	map<long,facet>::iterator tempHullit;
//	cout<<"numfacets:"<<tempFacets.size()<<endl;
	if(tempFacets.size()>4)
	{
		for(v4dit = verticesOfHull.begin();  v4dit!=verticesOfHull.end(); v4dit++)
		{
//			cout<<"vertex assignment status"<<v4dit->second.getBucketAssignmentStatus()<<endl;
			for(tempHullit=tempFacets.begin(); !v4dit->second.getBucketAssignmentStatus() && tempHullit!=tempFacets.end(); tempHullit++)
			{
				if(v4dit->second.isAbove(&((*tempHullit).second)))
				{
					tempHullit->second.putInBucket(v4dit->first);
					break;
				}
			}
		}
	}
}
long convexHull4d::generateId(const vector<long>* vertexIds)
{
	long id;
	string stringId;
	stringstream stringIdStream;
	for(unsigned int i=0; i<(*vertexIds).size(); i++)
	{
		stringIdStream<<(*vertexIds)[i];
		if(i<(*vertexIds).size()-1)
			stringIdStream<<',';
	}
	stringId=stringIdStream.str();
	id=convexHull4d::idCounter;
	if(idMap.find(stringId)==idMap.end())
		idMap.insert(pair<string,long>(stringId,convexHull4d::idCounter));
	else
		id=idMap.find(stringId)->second;
	convexHull4d::idCounter++;
	return id;
}
