/*
 * dewall.cpp
 *
 *  Created on: Jun 16, 2011
 *      Author: phanindrabhagavatula
 */

#include "includes.h"
int firstCell=0;
int lastCell;
int faceToShow=0;
bool showCircle=false;
bool wire=false;
bool debug=true;
int cellCounter=0;
extern Solid model;

bool sortVertexList(deque<Vertex>::pointer p1, deque<Vertex>::pointer p2, double alphaValue, int axisToUse)
{
	return abs(alphaValue-p1->getCoord((axisToSort)axisToUse)) <= abs(alphaValue-p2->getCoord((axisToSort)axisToUse));
}
class sorter
{
public:
	sorter(double alphaVal, int axisVal) : alpha(alphaVal), axis(axisVal) {}
	bool operator()(deque<Vertex>::pointer p1, deque<Vertex>::pointer p2)
	{
		return sortVertexList(p1,p2,alpha,axis);
	}
private:
	double alpha;
	int axis;
};


//void Solid::dewall(axisToSort axis, deque<Vertex>& vertices, map<string,deque<Face>::pointer>& activeFaceList, deque<deque<deque<deque<deque<Vertex>::pointer > > > >& grid)
void Solid::dewall(map<string,deque<Face>::pointer>& activeFaceList, kdtree *kdTree, kdtree *treeRoot)
{
	bool jump=true;
	//int medianIndex = vertices.size() / 2, i = 0;
	double alphaPlane=0;
	map<string,deque<Face>::pointer> aflalpha;
	map<string,deque<Face>::pointer> afllow;
	map<string,deque<Face>::pointer> aflhigh;
	deque<deque<Vertex>::pointer > candidateVertices;
	double searchBoxFirstSimplex[2][3];
	bool endFirstSimplexSearchLoop=false;
	Cell* newCell;
	int axisToUse = kdTree->getRoot()->getaxis();
	alphaPlane = kdTree->getRoot()->getKeyValue();
	for(int i=0; i<3; i++)
	{
		searchBoxFirstSimplex[0][i] = -numeric_limits<double>::max();
		searchBoxFirstSimplex[1][i] =  numeric_limits<double>::max();
	}
	searchBoxFirstSimplex[0][axisToUse] = kdTree->getRoot()->getLeftChild()->getHighRegion()[axisToUse];
	//searchBoxFirstSimplex[1][axisToUse] = kdTree->getRoot()->getRightChild()->getLowRegion()[axisToUse];
	searchBoxFirstSimplex[1][axisToUse] = alphaPlane;

	cout<<"alpha Plane:"<<kdTree->getRoot()->getaxis()<<"->"<<alphaPlane<<endl;
	while (activeFaceList.size() == 0 && !endFirstSimplexSearchLoop)
	{
		kdTree->searchNodes(candidateVertices,kdTree->getRoot(),searchBoxFirstSimplex);
		cout<<"size to look for:"<<candidateVertices.size()<<endl;
		if(candidateVertices.size())
		{
			sort(candidateVertices.begin(), candidateVertices.end(),sorter(alphaPlane, axisToUse));
			for(unsigned int i=0; i<candidateVertices.size(); i++)
			{
				newCell = new Cell();
				if (newCell->getVertexListSize() <=4) {
					newCell->addVertex(candidateVertices[i]);
				}
				listOfCells.push_back(*newCell);
				makeCell(listOfCells.back(),kdTree, treeRoot, alphaPlane);
				if(!listOfCells.back().getFaceListSize())
				{
					listOfCells.pop_back();
					listOfEdges.pop_back();
					continue;
				}
				makeCell(listOfCells.back(),kdTree, treeRoot, alphaPlane);
				cellCounter++;
//				cout<<"first cell face list size"<<listOfCells.back().getFaceListSize()<<endl;
				if(listOfCells.back().getFaceListSize()==4)
				{
					for(int i=0; i<4; i++)
						activeFaceList.insert(pair<string,deque<Face>::pointer>(listOfCells.back().getFaces()[i]->getId(), listOfCells.back().getFaces()[i]));
					cout<<endl<<"current cellid:"<<listOfCells.back().getId()<<endl;
					cout.flush();
					endFirstSimplexSearchLoop=true;
					break;
				}
				else
				{
					listOfCells.pop_back();
					listOfFaces.pop_back();
				}
			}
			if(!endFirstSimplexSearchLoop)
			{
				candidateVertices.clear();
				searchBoxFirstSimplex[0][axisToUse]-=abs(candidateVertices[0]->getCoord((axisToSort)axisToUse));
//				searchBoxFirstSimplex[1][axisToUse]+=abs(candidateVertices[0]->getCoord((axisToSort)axisToUse));
			}
		}
		else
		{
			candidateVertices.clear();
			searchBoxFirstSimplex[0][axisToUse] -= abs(kdTree->getRoot()->getLeftChild()->getHighRegion()[axisToUse]);
//			searchBoxFirstSimplex[1][axisToUse] += abs(kdTree->getRoot()->getRightChild()->getLowRegion()[axisToUse]);
		}
	}
	if(activeFaceList.size())
	{
		map<string,deque<Face>::pointer>::iterator afiter;
		for(afiter=activeFaceList.begin(); afiter!=activeFaceList.end(); afiter++)
		{
			if((*afiter).second->getVertices()[0]->getCoord((axisToSort)kdTree->getRoot()->getaxis())<=alphaPlane
					&& (*afiter).second->getVertices()[1]->getCoord((axisToSort)kdTree->getRoot()->getaxis())<=alphaPlane
					&& (*afiter).second->getVertices()[2]->getCoord((axisToSort)kdTree->getRoot()->getaxis())<=alphaPlane)
				afllow.insert(pair<string,deque<Face>::pointer>((*afiter).first, (*afiter).second));
			else if((*afiter).second->getVertices()[0]->getCoord((axisToSort)kdTree->getRoot()->getaxis())>alphaPlane
					&& (*afiter).second->getVertices()[1]->getCoord((axisToSort)kdTree->getRoot()->getaxis())>alphaPlane
					&& (*afiter).second->getVertices()[2]->getCoord((axisToSort)kdTree->getRoot()->getaxis())>alphaPlane)
				aflhigh.insert(pair<string,deque<Face>::pointer>((*afiter).first, (*afiter).second));
			else
				aflalpha.insert(pair<string,deque<Face>::pointer>((*afiter).first, (*afiter).second));

		}

		while(aflalpha.size())
		{
			cout<<"Size of aflalpha:"<<aflalpha.size()<<endl;
			cout.flush();
			//		cout<<"Size of cell list:"<<listOfCells.size()<<endl;
			//		for(map<string,deque<Face>::pointer>::iterator it=aflalpha.begin(); it!=aflalpha.end(); it++)
			//			cout<<"aflalpha contents:"<<(*it).first<<","<<(*it).second<<endl;
			//		cout.flush();
			map<string,deque<Face>::pointer>::iterator aflit;
			map<string,deque<Face>::pointer>::iterator delfaceit;
			multimap<deque<Cell>::pointer, deque<Face>::pointer>::iterator niter;
			deque<deque<Face>::pointer>::iterator faceiter;
			//			for(aflit=aflalpha.begin(); aflit!=aflalpha.end(); )
			while(aflalpha.size())
			{
				aflit=aflalpha.begin();
				if(!(*aflit).first.compare("22,48,50"))
				{
					cout<<"in trouble";
					goto allend;
				}
				if(!(*aflit).second->getNumOfOpenFaces())
				{
					//aflalpha.erase(aflit++);
					aflalpha.erase(aflit);
					continue;
				}
				if((*aflit).second->getNeighCell1() && (*aflit).second->getNeighCell2())
				{
					//					aflalpha.erase(aflit++);
					aflalpha.erase(aflit);
					continue;
				}
				newCell = new Cell();
				listOfCells.push_back(*newCell);
				cout<<"current afl entry:"<<(*aflit).first<<endl;
				if(!(*aflit).first.compare("6,7,12"))
					debug=true;
				if(!(*aflit).first.compare("12,26,52"))
					cout<<"why chosing 27?"<<endl;
				cout.flush();
				listOfCells.back().addFEVs((*aflit).second);
				//			cout<<"size of cell list:"<<listOfCells.size()<<endl;
				cout.flush();
				if(makeCell(listOfCells.back(),kdTree,treeRoot, alphaPlane))
				{
					cellCounter++;
					cout<<"Cell Number:"<<cellCounter<<" cellid:"<<listOfCells.back().getId()<<endl;
					//				if(listOfCells.back().getId()=="17,19,33,40")
					//					cout<<"break here"<<endl;
					cout<<"faces generated  ";
					cout.flush();
					for(faceiter=listOfCells.back().getFaces().begin(); faceiter!=listOfCells.back().getFaces().end(); faceiter++)
					{
						//						if(!(*faceiter)->getId().compare("12,13,26"))
						//							cout<<"why inserting 12/13/26"<<endl;
						cout<<":"<<(*faceiter)->getId();
						cout.flush();
						delfaceit = aflalpha.find((*faceiter)->getId());
						if(delfaceit!=aflalpha.end())
						{
							aflit++;
							//cout<<"face to delete"<<(*delfaceit).second->getId()<<endl;
							aflalpha.erase(delfaceit);
							printMap(aflalpha);
						}
						else if((*faceiter)->getNeighCell1() && (*faceiter)->getNeighCell2())
							continue;
						else if(((*faceiter)->getVertices()[0]->getCoord((axisToSort)kdTree->getRoot()->getaxis()))<=alphaPlane
								&& ((*faceiter)->getVertices()[1]->getCoord((axisToSort)kdTree->getRoot()->getaxis()))<=alphaPlane
								&& ((*faceiter)->getVertices()[2]->getCoord((axisToSort)kdTree->getRoot()->getaxis()))<=alphaPlane)
						{
							afllow.insert(pair<string,deque<Face>::pointer>((*faceiter)->getId(), *faceiter));
						}
						else if(((*faceiter)->getVertices()[0]->getCoord((axisToSort)kdTree->getRoot()->getaxis()))>alphaPlane
								&& ((*faceiter)->getVertices()[1]->getCoord((axisToSort)kdTree->getRoot()->getaxis()))>alphaPlane
								&& ((*faceiter)->getVertices()[2]->getCoord((axisToSort)kdTree->getRoot()->getaxis()))>alphaPlane)
						{
							aflhigh.insert(pair<string,deque<Face>::pointer>((*faceiter)->getId(),*faceiter));
						}
						else
						{
							//							if(!(*faceiter)->getId().compare("12,13,26"))
							//								cout<<"why inserting 12/13/26"<<endl;
							aflalpha.insert(pair<string,deque<Face>::pointer>((*faceiter)->getId(),*faceiter));
						}
					}
					printMap(aflalpha);
					cout<<endl;
				}
				else
				{
					listOfCells.back().delFEVs((*aflit).second);
					listOfCells.pop_back();
					cout<<"Size of cell list after subtraction:"<<listOfCells.size()<<endl;
					aflalpha.erase(aflit++);
					printMap(aflalpha);
				}
				cout.flush();
			}
			cout<<"alpha size:"<<aflalpha.size()<<endl;
			cout<<"Number of cells"<<listOfCells.size()<<endl;
			cout<<"Number of faces"<<listOfFaces.size()<<endl;
			cout<<"Number of edges"<<listOfEdges.size()<<endl;
			map<string,deque<Face>::pointer>::iterator fmapit;
			//		for(unsigned int i=0; i<listOfCells.size(); i++)
			//		{
			//			if(listOfCells[i].getVertexListSize()==3)
			//				makeCell(listOfCells[i],grid, alphaPlane);
			//		}
		}
	}
	allend:
	cout<<"aflalphalist:"<<aflalpha.size()<<endl;
	cout<<"afllowlist:"<<afllow.size()<<endl;
	cout<<"aflhighlist:"<<aflhigh.size()<<endl;
	cout.flush();
	//	if(!kdTree->getRoot()->getLeftChild()->getaxis())
	//		return;
	if(jump && afllow.size() ) //&& !kdTree->getRoot()->getLeftChild()->isLeaf()
	{
		kdtree *kdsubTree=new kdtree();
		kdsubTree->setRoot(kdTree->getRoot()->getLeftChild());
		dewall(afllow, kdsubTree, treeRoot);
	}
	cout<<"aflalphalist:"<<aflalpha.size()<<endl;
	cout<<"afllowlist:"<<afllow.size()<<endl;
	cout<<"aflhighlist:"<<aflhigh.size()<<endl;
	cout.flush();
	if(jump && aflhigh.size()) //!kdTree->getRoot()->getRightChild()->isLeaf()
	{
		kdtree *kdsubTree=new kdtree();
		kdsubTree->setRoot(kdTree->getRoot()->getRightChild());
		dewall(aflhigh, kdsubTree, treeRoot);
	}

}

bool Solid::makeCell(deque<Cell>::reference cell, kdtree* kdTree, kdtree* treeRoot,  double alphaPlane)
{
	Vertex* winnerVertex;
	int *centerCoords;
	unsigned int stepSize = 0;
	centerCoords = (int*)malloc(sizeof(int)*4);
//	unsigned short int layerlimitx=1, layerlimity=1, layerlimitz=1;
//	unsigned short int layerlimitX=1, layerlimitY=1, layerlimitZ=1;
//	bool endSearch=false;
//	int layer=0;
	if(cell.getVertexListSize()==1)
	{
		winnerVertex=NULL;
		Edge*	newEdge = NULL;
//		double minSqDist=-1;
//		double tempSqDist=0;
		stepSize = 1;
		centerCoords=(int*)cell.getVertices()[0]->sparePtr;
		//cout<<endl<<centerCoords[0]<<","<<centerCoords[1]<<","<<centerCoords[2]<<endl;
		//cout.flush();
		winnerVertex=kdTree->nearestNeighbour(cell.getVertices()[0]);
		cout<<"winner vertex coord to compare:"<<winnerVertex->getCoord((axisToSort)kdTree->getRoot()->getaxis())<<endl;
		if(!winnerVertex || winnerVertex->getCoord((axisToSort)kdTree->getRoot()->getaxis())<=alphaPlane)
			return false;
		cout<<"winner vertex :"<<winnerVertex->getId()<<endl;
		cell.addVertex(winnerVertex);
		newEdge = new Edge();
		listOfEdges.push_back(*newEdge);
		listOfEdges.back().setVertices(cell.getVertices()[0],cell.getVertices()[1]);
		cell.addEdge(&listOfEdges.back());
	}
	if(cell.getVertexListSize()==2 || cell.getVertexListSize()==3)
	{
		winnerVertex=NULL;
		Edge*	newEdge1 = NULL;
		Edge*   newEdge2 = NULL;
		Edge*   newEdge3 = NULL;
		Face*   newFace  = NULL;
		double initSearchAreaBox[2][3];
		double centerRadius[4];
		//double minRadius=0;
		double stepSize = 1.0;
		//trippleBool tstSphrReturn;
		//double tempRadius;
		bool endLoop = false;
		bool expandSearchVolume=true, firstCandidateWinner=false;
		deque<deque<Vertex>::pointer > candidateVertices;
		//deque<Vertex>::pointer candidateWinnerVertex;
		initSearchAreaBox[0][0]= (cell.getVertices()[0]->getXCoord()<cell.getVertices()[1]->getXCoord())?cell.getVertices()[0]->getXCoord():cell.getVertices()[1]->getXCoord();
		initSearchAreaBox[1][0]= (cell.getVertices()[0]->getXCoord()>cell.getVertices()[1]->getXCoord())?cell.getVertices()[0]->getXCoord():cell.getVertices()[1]->getXCoord();
		initSearchAreaBox[0][1]= (cell.getVertices()[0]->getYCoord()<cell.getVertices()[1]->getYCoord())?cell.getVertices()[0]->getYCoord():cell.getVertices()[1]->getYCoord();
		initSearchAreaBox[1][1]= (cell.getVertices()[0]->getYCoord()>cell.getVertices()[1]->getYCoord())?cell.getVertices()[0]->getYCoord():cell.getVertices()[1]->getYCoord();
		initSearchAreaBox[0][2]= (cell.getVertices()[0]->getZCoord()<cell.getVertices()[1]->getZCoord())?cell.getVertices()[0]->getZCoord():cell.getVertices()[1]->getZCoord();
		initSearchAreaBox[1][2]= (cell.getVertices()[0]->getZCoord()>cell.getVertices()[1]->getZCoord())?cell.getVertices()[0]->getZCoord():cell.getVertices()[1]->getZCoord();
		if(cell.getVertexListSize()==3)
		{
			initSearchAreaBox[0][0]= (initSearchAreaBox[0][0]<cell.getVertices()[2]->getXCoord())?initSearchAreaBox[0][0]:cell.getVertices()[2]->getXCoord();
			initSearchAreaBox[1][0]= (initSearchAreaBox[1][0]>cell.getVertices()[2]->getXCoord())?initSearchAreaBox[1][0]:cell.getVertices()[2]->getXCoord();
			initSearchAreaBox[0][1]= (initSearchAreaBox[0][1]<cell.getVertices()[2]->getYCoord())?initSearchAreaBox[0][1]:cell.getVertices()[2]->getYCoord();
			initSearchAreaBox[1][1]= (initSearchAreaBox[1][1]>cell.getVertices()[2]->getYCoord())?initSearchAreaBox[1][1]:cell.getVertices()[2]->getYCoord();
			initSearchAreaBox[0][2]= (initSearchAreaBox[0][2]<cell.getVertices()[2]->getZCoord())?initSearchAreaBox[0][2]:cell.getVertices()[2]->getZCoord();
			initSearchAreaBox[1][2]= (initSearchAreaBox[1][2]>cell.getVertices()[2]->getZCoord())?initSearchAreaBox[1][2]:cell.getVertices()[2]->getZCoord();
		}
		while(!winnerVertex && !endLoop)
		{
			//cout<<"list1 size:"<<candidateVertices.size()<<endl;
			kdTree->searchNodes(candidateVertices,kdTree->getRoot(),initSearchAreaBox);
			if(debug)
				cout<<"list1 size:"<<candidateVertices.size()<<endl;
			if(kdTree->getRoot()->getsubTreeSize()==candidateVertices.size())
				expandSearchVolume = false;
			cout.flush();
			if(candidateVertices.size())
			{
				for(unsigned int i=0; i<candidateVertices.size(); i++)
				{
					if(debug)
						cout<<"\ttested vertex"<<candidateVertices[i]->getId()<<endl;
					if((cell.getVertexListSize()==3 && !cell.check3DOrientationAdaptive(*candidateVertices[i]))
					 ||(cell.getVertexListSize()==2 && !cell.check2DOrientationAdaptive(*candidateVertices[i]))
					 ||(!candidateVertices[i]->getNumOfOpenFaces()))
						continue;
//					cout<<"cell vertices:"<<cell.getVertexListSize()<<endl;
//					cout.flush();
					if(!firstCandidateWinner)
					{
						winnerVertex=candidateVertices[i];
						firstCandidateWinner=true;
						endLoop=true;
						continue;
					}
					if(((cell.getVertexListSize()==2) && cell.testCircumCircleAdaptive(*winnerVertex, *candidateVertices[i])>0)
					|| ((cell.getVertexListSize()==3) && cell.testCircumSphereAdaptive(*winnerVertex, *candidateVertices[i])>0))
					{
						winnerVertex=candidateVertices[i];
					}
				}
				if(winnerVertex && cell.getVertexListSize()==3)
				{
					cell.testCircumSphere(*winnerVertex, centerRadius);
					cell.setCircumCenter(centerRadius);
					cell.setCircumRadius(centerRadius[3]);
					candidateVertices.clear();
					//						cout<<"list1 size after clear:"<<candidateVertices.size()<<endl;
					cout.flush();
					kdTree->searchNodes(candidateVertices,treeRoot->getRoot(),abs(centerRadius[3]),centerRadius);
					if(debug)
						cout<<"list2 size:"<<candidateVertices.size()<<endl;
					//						cout.flush();
					if(candidateVertices.size())
					{
						for(unsigned int j=0; j<candidateVertices.size(); j++)
						{
							if(debug)
								cout<<"vertex tested in final test"<<candidateVertices[j]->getId()<<endl;
							if(cell.testCircumSphereAdaptive(*winnerVertex, *candidateVertices[j])>0)
							{
								winnerVertex=NULL;
								break;
							}
						}
					}
				}

				//}
			}
			if(!winnerVertex)
			{
				if(!expandSearchVolume)
					break;
				for(int m=0; m<2; m++)
					for(int n=0; n<3; n++)
						initSearchAreaBox[m][n] += pow(-1.0, m+1)*stepSize;
				stepSize++;
				candidateVertices.clear();
			}
		}
		if(!winnerVertex)
			return false;
		if(winnerVertex)
		{
			if(cell.getVertexListSize()==2)
			{
				cell.addVertex(winnerVertex);
				newEdge1 = new Edge();
				newEdge2 = new Edge();
				string facid;
				stringstream tempStream1;
				int idarray[3];
				listOfEdges.push_back(*newEdge1);
				listOfEdges.back().setVertices(cell.getVertices()[1],cell.getVertices()[2]);
				cell.addEdge(&listOfEdges.back());
				listOfEdges.push_back(*newEdge2);
				listOfEdges.back().setVertices(cell.getVertices()[0],cell.getVertices()[2]);
				cell.addEdge(&listOfEdges.back());
				//cell.getEdges()[1].setVertices(&cell.getVertices()[1],&cell.getVertices()[2]);
				//cell.getEdges()[2].setVertices(&cell.getVertices()[0],&cell.getVertices()[2]);
				newFace  = new Face();
				listOfFaces.push_back(*newFace);
				listOfFaces.back().addVertices(cell.getVertices()[0],cell.getVertices()[1],cell.getVertices()[2]);
				//TODO
				idarray[0]=cell.getVertices()[0]->getId();
				idarray[1]=cell.getVertices()[1]->getId();
				idarray[2]=cell.getVertices()[2]->getId();
				sort3ints(idarray);
				tempStream1<<idarray[0]<<','<<idarray[1]<<','<<idarray[2];
				facid=tempStream1.str();
				listOfFaces.back().setId(facid);
				faceMap.insert(pair<string,deque<Face>::pointer>(facid,&listOfFaces.back()));
				listOfFaces.back().addEdges(cell.getEdges()[0],cell.getEdges()[1],cell.getEdges()[2]);
				cell.addFace(&listOfFaces.back());
				//cell.getFaces()[0].addVertices(&cell.getVertices()[0],&cell.getVertices()[1],&cell.getVertices()[2]);
				//cell.getFaces()[0].addEdges(&cell.getEdges()[0],&cell.getEdges()[1],&cell.getEdges()[2]);
				cell.getFaces()[0]->incrNumOfOpenFaces();
			}
			else if(cell.getVertexListSize()==3)
			{
				cell.addVertex(winnerVertex);
				int newFaceCount=0;
//				newEdge1 = new Edge();
//				newEdge2 = new Edge();
//				newEdge3 = new Edge();
				map<string,deque<Face>::pointer>::iterator fmapit;
				int idarray[3];
				int cellIdArray[4];
				string facid;
				stringstream tempStream1, tempStream2, tempStream3, cellidstream;
//				listOfEdges.push_back(*newEdge1);
//				listOfEdges.back().setVertices(cell.getVertices()[0],cell.getVertices()[3]);
//				cell.addEdge(&listOfEdges.back());
//				listOfEdges.push_back(*newEdge2);
//				listOfEdges.back().setVertices(cell.getVertices()[1],cell.getVertices()[3]);
//				cell.addEdge(&listOfEdges.back());
//				listOfEdges.push_back(*newEdge3);
//				listOfEdges.back().setVertices(cell.getVertices()[2],cell.getVertices()[3]);
//				cell.addEdge(&listOfEdges.back());
				//			cell.getEdges()[3].setVertices(&cell.getVertices()[0],&cell.getVertices()[3]);
				//			cell.getEdges()[4].setVertices(&cell.getVertices()[1],&cell.getVertices()[3]);
				//			cell.getEdges()[5].setVertices(&cell.getVertices()[2],&cell.getVertices()[3]);
				idarray[0]=cell.getVertices()[0]->getId();
				idarray[1]=cell.getVertices()[1]->getId();
				idarray[2]=cell.getVertices()[3]->getId();
				sort3ints(idarray);
				tempStream1<<idarray[0]<<','<<idarray[1]<<','<<idarray[2];
				facid=tempStream1.str();
				cell.getFaces().back()->decrNumOfOpenFaces();
				//			if(facid=="17,32,33")
				//				cout<<"found culprit"<<endl;
				fmapit=faceMap.find(facid);
				if(fmapit==faceMap.end())
				{
					Face* newFace1 = new Face();
					listOfFaces.push_back(*newFace1);
					listOfFaces.back().addVertices(cell.getVertices()[0],cell.getVertices()[1],cell.getVertices()[3]);
					listOfFaces.back().setId(facid);
					faceMap.insert(pair<string,deque<Face>::pointer>(facid,&listOfFaces.back()));
					cell.addFace(&listOfFaces.back());
					cell.getFaces().back()->incrNumOfOpenFaces();
					newFaceCount++;
				}
				else
				{
					if(cell.addFace((*fmapit).second))
						(*fmapit).second->decrNumOfOpenFaces();
					else
					{
						winnerVertex->decrNumOfOpenFaces();
						winnerVertex=NULL;
						return false;
					}
				}
				//tempStream<<cell.getVertices()[1]->getId()<<','<<cell.getVertices()[2]->getId()<<','<<cell.getVertices()[3]->getId();
				idarray[0]=cell.getVertices()[1]->getId();
				idarray[1]=cell.getVertices()[2]->getId();
				idarray[2]=cell.getVertices()[3]->getId();
				sort3ints(idarray);
				tempStream2<<idarray[0]<<','<<idarray[1]<<','<<idarray[2];
				facid=tempStream2.str();
				//			if(facid=="17,32,33")
				//				cout<<"found culprit"<<endl;
				fmapit=faceMap.find(facid);
				if(fmapit==faceMap.end())
				{
					Face* newFace2 = new Face();
					listOfFaces.push_back(*newFace2);
					listOfFaces.back().addVertices(cell.getVertices()[1],cell.getVertices()[2],cell.getVertices()[3]);
					listOfFaces.back().setId(facid);
					faceMap.insert(pair<string,deque<Face>::pointer>(facid,&listOfFaces.back()));
					cell.addFace(&listOfFaces.back());
					cell.getFaces().back()->incrNumOfOpenFaces();
					newFaceCount++;

				}
				else
				{
					if(cell.addFace((*fmapit).second))
						(*fmapit).second->decrNumOfOpenFaces();
					else
					{
						while(newFaceCount)
						{
							listOfFaces.pop_back();
							newFaceCount--;
						}
						winnerVertex->decrNumOfOpenFaces();
						winnerVertex=NULL;
						return false;
					}
				}
				//tempStream<<cell.getVertices()[2]->getId()<<','<<cell.getVertices()[0]->getId()<<','<<cell.getVertices()[3]->getId();
				idarray[0]=cell.getVertices()[2]->getId();
				idarray[1]=cell.getVertices()[0]->getId();
				idarray[2]=cell.getVertices()[3]->getId();
				sort3ints(idarray);
				tempStream3<<idarray[0]<<','<<idarray[1]<<','<<idarray[2];
				facid=tempStream3.str();
				//			if(facid=="17,32,33")
				//				cout<<"found culprit"<<endl;
				fmapit=faceMap.find(facid);
				if(fmapit==faceMap.end())
				{
					Face* newFace3 = new Face();
					listOfFaces.push_back(*newFace3);
					listOfFaces.back().addVertices(cell.getVertices()[2],cell.getVertices()[0],cell.getVertices()[3]);
					listOfFaces.back().setId(facid);
					faceMap.insert(pair<string,deque<Face>::pointer>(facid,&listOfFaces.back()));
					cell.addFace(&listOfFaces.back());
					cell.getFaces().back()->incrNumOfOpenFaces();
				}
				else
				{
					if(cell.addFace((*fmapit).second))
						(*fmapit).second->decrNumOfOpenFaces();
					else
					{
						while(newFaceCount)
						{
							listOfFaces.pop_back();
							newFaceCount--;
						}
						winnerVertex->decrNumOfOpenFaces();
						winnerVertex=NULL;
						return false;
					}
				}
				//			cell.getFaces()[1].addVertices(&cell.getVertices()[0],&cell.getVertices()[1],&cell.getVertices()[3]);
				//			cell.getFaces()[2].addVertices(&cell.getVertices()[1],&cell.getVertices()[2],&cell.getVertices()[3]);
				//			cell.getFaces()[3].addVertices(&cell.getVertices()[2],&cell.getVertices()[0],&cell.getVertices()[3]);

				newEdge1 = new Edge();
				newEdge2 = new Edge();
				newEdge3 = new Edge();
				listOfEdges.push_back(*newEdge1);
				listOfEdges.back().setVertices(cell.getVertices()[0],cell.getVertices()[3]);
				cell.addEdge(&listOfEdges.back());
				listOfEdges.push_back(*newEdge2);
				listOfEdges.back().setVertices(cell.getVertices()[1],cell.getVertices()[3]);
				cell.addEdge(&listOfEdges.back());
				listOfEdges.push_back(*newEdge3);
				listOfEdges.back().setVertices(cell.getVertices()[2],cell.getVertices()[3]);
				cell.addEdge(&listOfEdges.back());

				cell.getFaces()[1]->addEdges(cell.getEdges()[0],cell.getEdges()[4],cell.getEdges()[3]);
				cell.getFaces()[2]->addEdges(cell.getEdges()[1],cell.getEdges()[5],cell.getEdges()[4]);
				cell.getFaces()[3]->addEdges(cell.getEdges()[2],cell.getEdges()[3],cell.getEdges()[5]);

				cell.getFaces()[0]->addOppositeVertex(cell.getVertices()[3]);
				cell.getFaces()[1]->addOppositeVertex(cell.getVertices()[2]);
				cell.getFaces()[2]->addOppositeVertex(cell.getVertices()[0]);
				cell.getFaces()[3]->addOppositeVertex(cell.getVertices()[1]);

				for(int i=0; i<4; i++)
					cellIdArray[i]=cell.getVertices()[i]->getId();
				sort4ints(cellIdArray);
				cellidstream<<cellIdArray[0]<<','<<cellIdArray[1]<<','<<cellIdArray[2]<<','<<cellIdArray[3];
				cell.setId(cellidstream.str());
			}
		}
	}
//	model.drawEdges();
//	glutSwapBuffers();
	return true;
}
