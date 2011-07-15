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
bool showCircle=true;
bool wire=false;

//struct classcomp {
//  bool operator() (const string f1id, const string f2id)
//  {
//	  if(f1id.compare(f2id)>=0)
//		  return true;
//	  else
//		  return false;
//  }
//};

void Solid::dewall(axisToSort axis, deque<Vertex>& vertices, map<string,deque<Face>::pointer>& activeFaceList, deque<deque<deque<deque<deque<Vertex>::pointer > > > >& grid)
{
	bool jump=false;
	int medianIndex = vertices.size() / 2, i = 0;
	double alphaPlane=0;
	map<string,deque<Face>::pointer> aflalpha;
	map<string,deque<Face>::pointer> afllow;
	map<string,deque<Face>::pointer> aflhigh;
	deque<Vertex> verticesLow;
	deque<Vertex> verticesHigh;
	deque<Vertex>::iterator vit;
	//Edge* newEdge;
	Cell* newCell;
	for (vit = vertices.begin(); vit != vertices.end(); i++,vit++) {
		if (i < medianIndex)
			verticesLow.push_back(*vit);
		else if (i > medianIndex)
			verticesHigh.push_back(*vit);
	}
	if(floor(vertices[medianIndex].getCoord(axis)) != floor(vertices[medianIndex+1].getCoord(axis)))
		alphaPlane = floor(vertices[medianIndex+1].getCoord(axis));
	else
		alphaPlane = vertices[medianIndex+1].getCoord(axis) + ((vertices[medianIndex+1].getCoord(axis) - vertices[medianIndex].getCoord(axis))/2);
//	cout<<"alpha Plane:"<<alphaPlane<<endl;
	if (activeFaceList.size() == 0)
	{
		newCell = new Cell();
		if (newCell->getVertexListSize() <=4) {
			newCell->addVertex(&listOfVertices[vertices[medianIndex].getId()]);
		}
		listOfCells.push_back(*newCell);
		makeCell(listOfCells.back(),grid, alphaPlane);
		makeCell(listOfCells.back(),grid, alphaPlane);
//		cout << "vertex1" << newCell->getVertices()[0].getXCoord()<<endl;
//		cout<<"Edges1, v1, xcoord:"<<listOfCells.back().getEdges()[0].getVertex()[0]->getXCoord()<<endl;
//		cout<<"Edges1, v2, xcoord:"<<listOfCells.back().getEdges()[0].getVertex()[1]->getXCoord()<<endl;
//
//		cout<<"Edges1, v1, xcoord:"<<listOfCells.back().getEdges()[1].getVertex()[0]->getXCoord()<<endl;
//		cout<<"Edges1, v2, xcoord:"<<listOfCells.back().getEdges()[1].getVertex()[1]->getXCoord()<<endl;
//
//		cout<<"Edges1, v1, xcoord:"<<listOfCells.back().getEdges()[2].getVertex()[0]->getXCoord()<<endl;
//		cout<<"Edges1, v2, xcoord:"<<listOfCells.back().getEdges()[2].getVertex()[1]->getXCoord()<<endl;
//
//		cout.flush();
		for(int i=0; i<4;i++)
		{
			if((listOfCells.back().getFaces()[i]->getVertices()[0]->getXCoord())<alphaPlane
			&& (listOfCells.back().getFaces()[i]->getVertices()[1]->getXCoord())<alphaPlane
			&& (listOfCells.back().getFaces()[i]->getVertices()[2]->getXCoord())<alphaPlane)
			{
				afllow.insert(pair<string,deque<Face>::pointer>(listOfCells.back().getFaces()[i]->getId(), listOfCells.back().getFaces()[i]));
			}
			else if((listOfCells.back().getFaces()[i]->getVertices()[0]->getXCoord())>alphaPlane
				 && (listOfCells.back().getFaces()[i]->getVertices()[1]->getXCoord())>alphaPlane
				 && (listOfCells.back().getFaces()[i]->getVertices()[2]->getXCoord())>alphaPlane)
			{
				aflhigh.insert(pair<string,deque<Face>::pointer>(listOfCells.back().getFaces()[i]->getId(), listOfCells.back().getFaces()[i]));
			}
			else
				aflalpha.insert(pair<string,deque<Face>::pointer>(listOfCells.back().getFaces()[i]->getId(), listOfCells.back().getFaces()[i]));
		}
		cout<<endl<<"current cellid:"<<listOfCells.back().getId()<<endl;
//		cout.flush();
	}
	while(aflalpha.size())
	{
//		cout<<"Size of aflalpha:"<<aflalpha.size()<<endl;
//		cout<<"Size of cell list:"<<listOfCells.size()<<endl;
//		for(map<string,deque<Face>::pointer>::iterator it=aflalpha.begin(); it!=aflalpha.end(); it++)
//			cout<<"aflalpha contents:"<<(*it).first<<","<<(*it).second<<endl;
//		cout.flush();
		map<string,deque<Face>::pointer>::iterator aflit;
		map<string,deque<Face>::pointer>::iterator delfaceit;
		multimap<deque<Cell>::pointer, deque<Face>::pointer>::iterator niter;
		deque<deque<Face>::pointer>::iterator faceiter;
		for(aflit=aflalpha.begin(); aflit!=aflalpha.end(); )
		{
			if(!(*aflit).first.compare("31,44,49"))
			{
				cout<<"in trouble";
				goto allend;
			}
			if((*aflit).second->getNeighCell1() && (*aflit).second->getNeighCell2())
			{
				aflalpha.erase(aflit++);
				continue;
			}
			newCell = new Cell();
			listOfCells.push_back(*newCell);
			cout<<"current afl entry:"<<(*aflit).first<<endl;
			listOfCells.back().addFEVs((*aflit).second);
//			cout<<"size of cell list:"<<listOfCells.size()<<endl;
			cout.flush();
			if(makeCell(listOfCells.back(),grid,alphaPlane))
			{
				//cout<<"current cellid:"<<listOfCells.back().getId()<<endl;
//				if(listOfCells.back().getId()=="17,19,33,40")
//					cout<<"break here"<<endl;
				cout<<"faces generated  ";
				for(faceiter=listOfCells.back().getFaces().begin(); faceiter!=listOfCells.back().getFaces().end(); faceiter++)
				{
					cout<<":"<<(*faceiter)->getId();
					delfaceit = aflalpha.find((*faceiter)->getId());
					if(delfaceit!=aflalpha.end())
					{
						aflit++;
						//cout<<"face to delete"<<(*delfaceit).second->getId()<<endl;
						aflalpha.erase(delfaceit);
					}
					else if((*faceiter)->getNeighCell1() && (*faceiter)->getNeighCell2())
						continue;
					else if(((*faceiter)->getVertices()[0]->getXCoord())<alphaPlane
							&& ((*faceiter)->getVertices()[1]->getXCoord())<alphaPlane
							&& ((*faceiter)->getVertices()[2]->getXCoord())<alphaPlane)
					{
						afllow.insert(pair<string,deque<Face>::pointer>((*faceiter)->getId(), *faceiter));
					}
					else if(((*faceiter)->getVertices()[0]->getXCoord())>alphaPlane
							&& ((*faceiter)->getVertices()[1]->getXCoord())>alphaPlane
							&& ((*faceiter)->getVertices()[2]->getXCoord())>alphaPlane)
					{
						aflhigh.insert(pair<string,deque<Face>::pointer>((*faceiter)->getId(),*faceiter));
					}
					else
						aflalpha.insert(pair<string,deque<Face>::pointer>((*faceiter)->getId(),*faceiter));
				}
				cout<<endl;
			}
			else
			{
				listOfCells.back().delFEVs((*aflit).second);
				listOfCells.pop_back();
				cout<<"Size of cell list after subtraction:"<<listOfCells.size()<<endl;
				aflalpha.erase(aflit++);
			}
			cout.flush();
		}
		//cout<<"alpha size:"<<aflalpha.size()<<endl;
		//cout<<"Number of cells"<<listOfCells.size()<<endl;
		//cout<<"Number of faces"<<listOfFaces.size()<<endl;
//		map<string,deque<Face>::pointer>::iterator fmapit;
//		for(unsigned int i=0; i<listOfCells.size(); i++)
//		{
//			if(listOfCells[i].getVertexListSize()==3)
//				makeCell(listOfCells[i],grid, alphaPlane);
//		}
	}
allend:
	cout<<"done";
	cout<<"aflalphalist:"<<aflalpha.size()<<endl;
	cout<<"afllowlist:"<<afllow.size()<<endl;
	cout<<"aflhighlist:"<<aflhigh.size()<<endl;
	cout.flush();
	if(!jump)
	{
		dewall((axisToSort)((axis+1)%3), verticesLow, afllow, grid);
		dewall((axisToSort)((axis+1)%3), verticesHigh, aflhigh, grid);
	}

}

bool Solid::makeCell(deque<Cell>::reference cell, deque<deque<deque<deque<deque<Vertex>::pointer > > > >& grid, double alphaPlane)
{
	Vertex* winnerVertex;
	int *centerCoords;
	unsigned int stepSize = 0;
	centerCoords = (int*)malloc(sizeof(int)*4);
	if(cell.getVertexListSize()==1)
	{
		winnerVertex=NULL;
		Edge*	newEdge = NULL;
		double minSqDist=-1;
		double tempSqDist=0;
		stepSize = 1;
		centerCoords=(int*)cell.getVertices()[0]->sparePtr;
		//cout<<endl<<centerCoords[0]<<","<<centerCoords[1]<<","<<centerCoords[2]<<endl;
		//cout.flush();
		while(winnerVertex==NULL)
		{
			for(unsigned int i = centerCoords[0]-stepSize; i<grid.size() && i<=centerCoords[0]+stepSize; i++)
				for(unsigned int j=centerCoords[1]-stepSize; j<grid[i].size() && j<=centerCoords[1]+stepSize ;j++)
					for(unsigned int k=centerCoords[2]-stepSize; k<grid[i][j].size() && k<=centerCoords[2]+stepSize; k++)
					{
						if((i-(centerCoords[0]-stepSize) >0 && j-(centerCoords[1]-stepSize)>0 && k-(centerCoords[2]-stepSize)>0)
						 &&(((centerCoords[0]+stepSize)-i > 0) && ((centerCoords[1]+stepSize)-j > 0) && ((centerCoords[2]+stepSize)-k > 0)))
							continue;
						if(grid[i][j][k].size()==0)
							continue;
						else
						{
							for(unsigned int indexer=0; indexer<grid[i][j][k].size(); indexer++)
							{
								if(grid[i][j][k][indexer]->getXCoord()<alphaPlane)
									continue;
								if(minSqDist<0)
								{
									minSqDist=cell.getVertices()[0]->getSqDistance(*grid[i][j][k][indexer]);
									winnerVertex=grid[i][j][k][indexer];
								}
								else
								{
									tempSqDist = cell.getVertices()[0]->getSqDistance(*grid[i][j][k][indexer]);
									if(tempSqDist<minSqDist)
									{
										minSqDist=tempSqDist;
										winnerVertex=grid[i][j][k][indexer];
									}
								}
							}

						}

					}
			stepSize++;
		}
		cout<<"winner vertex :"<<winnerVertex->getXCoord()<<endl;
		cell.addVertex(winnerVertex);
		newEdge = new Edge();
		listOfEdges.push_back(*newEdge);
		listOfEdges.back().setVertices(cell.getVertices()[0],cell.getVertices()[1]);
		cell.addEdge(&listOfEdges.back());
		//cell.getEdges()[0].setVertices(&cell.getVertices()[0],&cell.getVertices()[1]);
		//cout<<"Vertex1:"<<cell.getVertices()[0].getXCoord()<<","<<cell.getVertices()[0].getYCoord()<<","<<cell.getVertices()[0].getZCoord()<<endl;
		//cout<<"Vertex2:"<<cell.getVertices()[1].getXCoord()<<","<<cell.getVertices()[1].getYCoord()<<","<<cell.getVertices()[1].getZCoord()<<endl;

	}
	if(cell.getVertexListSize()==2 || cell.getVertexListSize()==3)
	{
		winnerVertex=NULL;
		Edge*	newEdge1 = NULL;
		Edge*   newEdge2 = NULL;
		Edge*   newEdge3 = NULL;
		Face*   newFace  = NULL;
		unsigned int bboxMin[3], bboxMax[3];
		unsigned int *gridCoord1,*gridCoord2, *gridCoord3;
		unsigned short int layerlimitx=1, layerlimity=1, layerlimitz=1;
		unsigned short int layerlimitX=1, layerlimitY=1, layerlimitZ=1;
		int layer=0;
		stepSize =0;
		double minRadius=0;
		double tempRadius;
		double centerRadius[4];
		bool   twoVertices, endSearch=false;
		gridCoord1 = (unsigned int*)malloc(sizeof(unsigned int)*4);
		gridCoord2 = (unsigned int*)malloc(sizeof(unsigned int)*4);
		gridCoord3 = (unsigned int*)malloc(sizeof(unsigned int)*4);

		//cout<<"Vertex1:"<<cell.getVertices()[0].getXCoord()<<","<<cell.getVertices()[0].getYCoord()<<","<<cell.getVertices()[0].getZCoord()<<endl;
		//cout<<"Vertex2:"<<cell.getVertices()[1].getXCoord()<<","<<cell.getVertices()[1].getYCoord()<<","<<cell.getVertices()[1].getZCoord()<<endl;
		gridCoord1=(unsigned int*)cell.getVertices()[0]->sparePtr;
		gridCoord2=(unsigned int*)cell.getVertices()[1]->sparePtr;

		bboxMin[0]= (gridCoord1[0]<gridCoord2[0])?gridCoord1[0]:gridCoord2[0];
		bboxMax[0]= (gridCoord1[0]>gridCoord2[0])?gridCoord1[0]:gridCoord2[0];
		bboxMin[1]= (gridCoord1[1]<gridCoord2[1])?gridCoord1[1]:gridCoord2[1];
		bboxMax[1]= (gridCoord1[1]>gridCoord2[1])?gridCoord1[1]:gridCoord2[1];
		bboxMin[2]= (gridCoord1[2]<gridCoord2[2])?gridCoord1[2]:gridCoord2[2];
		bboxMax[2]= (gridCoord1[2]>gridCoord2[2])?gridCoord1[2]:gridCoord2[2];

		if(cell.getVertexListSize()==2)
			twoVertices=true;
		else
			twoVertices=false;

		if(!twoVertices)
		{
			gridCoord3=(unsigned int*)cell.getVertices()[2]->sparePtr;
			bboxMin[0]= (bboxMin[0]<gridCoord3[0])?bboxMin[0]:gridCoord3[0];
			bboxMax[0]= (bboxMax[0]>gridCoord3[0])?bboxMax[0]:gridCoord3[0];
			bboxMin[1]= (bboxMin[1]<gridCoord3[1])?bboxMin[1]:gridCoord3[1];
			bboxMax[1]= (bboxMax[1]>gridCoord3[1])?bboxMax[1]:gridCoord3[1];
			bboxMin[2]= (bboxMin[2]<gridCoord3[2])?bboxMin[2]:gridCoord3[2];
			bboxMax[2]= (bboxMax[2]>gridCoord3[2])?bboxMax[2]:gridCoord3[2];
		}

		while(winnerVertex==NULL && !endSearch)
		{
//			cout<<"\t\tbbox dims:"<<bboxMin[0]<<","<<bboxMin[1]<<","<<bboxMin[2]<<":XX:"<<bboxMax[0]<<","<<bboxMax[1]<<","<<bboxMax[2]<<endl;
//			cout<<"\t\tbbox dims:"<<((bboxMin[0]<stepSize)?0:bboxMin[0]-stepSize)<<","<<((bboxMin[1]<stepSize)?0:bboxMin[1]-stepSize)<<","<<((bboxMin[2]<stepSize)?0:bboxMin[2]-stepSize)<<":XX:"<<bboxMax[0]+stepSize<<","<<bboxMax[1]+stepSize<<","<<bboxMax[2]+stepSize<<endl;
			for(unsigned int i = (bboxMin[0]<stepSize)?0:bboxMin[0]-stepSize; i<grid.size() && i<=bboxMax[0]+stepSize; i++)
			{
				for(unsigned int j=(bboxMin[1]<stepSize)?0:bboxMin[1]-stepSize; j<grid[i].size() && j<=bboxMax[1]+stepSize ;j++)
					for(unsigned int k=(bboxMin[2]<stepSize)?0:bboxMin[2]-stepSize; k<grid[i][j].size() && k<=bboxMax[2]+stepSize; k++)
					{
						//cout<<"\t\tgrid pos:"<<i<<":"<<j<<":"<<k<<endl;
						if(layer&& (i-((bboxMin[0]<stepSize)?0:bboxMin[0]-stepSize) >=layerlimitx && j-((bboxMin[1]<stepSize)?0:bboxMin[1]-stepSize)>=layerlimity && k-((bboxMin[2]<stepSize)?0:bboxMin[2]-stepSize)>=layerlimitz)
								&&(((bboxMax[0]+stepSize)-i >= layerlimitX) && ((bboxMax[1]+stepSize)-j >=layerlimitY) && ((bboxMax[2]+stepSize)-k >=layerlimitZ)))
						{
							//cout<<"outta here"<<endl;
							continue;
						}
						if(grid[i][j][k].size()==0)
						{
							//cout<<"nth herE:"<<endl;
							continue;
						}
						else
						{
							for(unsigned int indexer=0; indexer<grid[i][j][k].size(); indexer++)
							{
								cout<<"\ttested vertex"<<grid[i][j][k][indexer]->getId()<<endl;
								if(grid[i][j][k][indexer]->getId()==cell.getVertices()[0]->getId()
								|| grid[i][j][k][indexer]->getId()==cell.getVertices()[1]->getId())
									continue;
								 if(cell.getVertexListSize()==3 &&
									grid[i][j][k][indexer]->getId()==cell.getVertices()[2]->getId())
									continue;
								if(cell.getNeighListSize()>0 && !cell.checkOrientation(*grid[i][j][k][indexer]))
									continue;
								if((cell.getVertexListSize()==2 && cell.testCircumCircle(*grid[i][j][k][indexer],centerRadius))
								 ||(cell.getVertexListSize()==3 && (cell.testCircumSphere(*grid[i][j][k][indexer],centerRadius)==true_val)))
								{
									if(minRadius==0)
									{
										minRadius=centerRadius[3];
										winnerVertex=grid[i][j][k][indexer];
										cell.setCircumCenter(centerRadius);
										cell.setCircumRadius((centerRadius[3]>0)?centerRadius[3]:centerRadius[3]*-1);
									}
									else
									{
										tempRadius = centerRadius[3];
										if(tempRadius<minRadius)
										{
											minRadius=tempRadius;
											winnerVertex=grid[i][j][k][indexer];
											cell.setCircumCenter(centerRadius);
											cell.setCircumRadius((centerRadius[3]>0)?centerRadius[3]:centerRadius[3]*-1);
										}
									}
								}

							}
							if(winnerVertex)
							{
								confirmWinnerVertex(cell,winnerVertex,grid);
								if(winnerVertex)
								{
									cell.addVertex(winnerVertex);
									goto breakAllLoops;
								}
								else
									endSearch=true;
							}
						}
					}
			}
			if(!endSearch)
			{
				layer++;
				stepSize++;
				if(bboxMin[0]<stepSize)
					layerlimitx=0;
				if(bboxMin[1]<stepSize)
					layerlimity=0;
				if(bboxMin[2]<stepSize)
					layerlimitz=0;
				if((bboxMax[0]+stepSize) >grid.size())
					layerlimitX=0;
				if((bboxMax[1]+stepSize) > grid[0].size())
					layerlimitY=0;
				if((bboxMax[2]+stepSize) > grid[0][0].size())
					layerlimitZ=0;
				if((bboxMin[0]<stepSize) && (bboxMin[1]<stepSize) && (bboxMin[2]<stepSize)
						&&((bboxMax[0]+stepSize) >grid.size()) &&((bboxMax[1]+stepSize) > grid[0].size()) &&((bboxMax[2]+stepSize) > grid[0][0].size()))
					endSearch=true;
			}

		}
		breakAllLoops:
		if(!winnerVertex)
			return false;

		if(twoVertices && winnerVertex)
		{
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
			cell.getFaces()[0]->incrNumOfOpenFaces(grid);
		}
		if(!twoVertices && winnerVertex)
		{
			newEdge1 = new Edge();
			newEdge2 = new Edge();
			newEdge3 = new Edge();
			map<string,deque<Face>::pointer>::iterator fmapit;
			int idarray[3];
			int cellIdArray[4];
			string facid;
			stringstream tempStream1, tempStream2, tempStream3, cellidstream;
			listOfEdges.push_back(*newEdge1);
			listOfEdges.back().setVertices(cell.getVertices()[0],cell.getVertices()[3]);
			cell.addEdge(&listOfEdges.back());
			listOfEdges.push_back(*newEdge2);
			listOfEdges.back().setVertices(cell.getVertices()[1],cell.getVertices()[3]);
			cell.addEdge(&listOfEdges.back());
			listOfEdges.push_back(*newEdge3);
			listOfEdges.back().setVertices(cell.getVertices()[2],cell.getVertices()[3]);
			cell.addEdge(&listOfEdges.back());
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
			}
			else
			{
				cell.addFace((*fmapit).second);
				(*fmapit).second->decrNumOfOpenFaces();
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

			}
			else
			{
				cell.addFace((*fmapit).second);
				(*fmapit).second->decrNumOfOpenFaces();
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
				cell.addFace((*fmapit).second);
				(*fmapit).second->decrNumOfOpenFaces();
			}
//			cell.getFaces()[1].addVertices(&cell.getVertices()[0],&cell.getVertices()[1],&cell.getVertices()[3]);
//			cell.getFaces()[2].addVertices(&cell.getVertices()[1],&cell.getVertices()[2],&cell.getVertices()[3]);
//			cell.getFaces()[3].addVertices(&cell.getVertices()[2],&cell.getVertices()[0],&cell.getVertices()[3]);

			cell.getFaces()[1]->addEdges(cell.getEdges()[0],cell.getEdges()[4],cell.getEdges()[3]);
			cell.getFaces()[2]->addEdges(cell.getEdges()[1],cell.getEdges()[5],cell.getEdges()[4]);
			cell.getFaces()[3]->addEdges(cell.getEdges()[2],cell.getEdges()[3],cell.getEdges()[5]);

//			cell.getFaces()[1]->incrNumOfOpenFaces();
//			cell.getFaces()[2]->incrNumOfOpenFaces();
//			cell.getFaces()[3]->incrNumOfOpenFaces();

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
	return true;
}

void Solid::confirmWinnerVertex(deque<Cell>::reference cell,Vertex*& winnerVertex,deque<deque<deque<deque<deque<Vertex>::pointer > > > >& grid)
{
	unsigned int stepSize = ceil(cell.getCircumRadius());
	double minRadius = cell.getCircumRadius();
	double tempRadius = cell.getCircumRadius();
	double newCenterRadius[4];
	trippleBool tstSphrReturn;
	unsigned int centerGrid[3];
	int *vertexGridCoords;
	bool possibleFail=false;
	vertexGridCoords = (int*)malloc(sizeof(int)*4);
	vertexGridCoords = (int*)cell.getVertices()[0]->sparePtr;
	centerGrid[0]=vertexGridCoords[0]+(floor(cell.getCircumCenter()[0])-floor(cell.getVertices()[0]->getXCoord()));
	centerGrid[1]=vertexGridCoords[1]+(floor(cell.getCircumCenter()[1])-floor(cell.getVertices()[0]->getYCoord()));
	centerGrid[2]=vertexGridCoords[2]+(floor(cell.getCircumCenter()[2])-floor(cell.getVertices()[0]->getZCoord()));

	for(unsigned int i = (centerGrid[0]<stepSize)?0:centerGrid[0]-stepSize; i<grid.size() && i<=centerGrid[0]+stepSize; i++)
		for(unsigned int j=(centerGrid[1]<stepSize)?0:centerGrid[1]-stepSize; j<grid[i].size() && j<=centerGrid[1]+stepSize ;j++)
			for(unsigned int k=(centerGrid[2]<stepSize)?0:centerGrid[2]-stepSize; k<grid[i][j].size() && k<=centerGrid[2]+stepSize; k++)
			{
				if(grid[i][j][k].size()==0)
					continue;
				else
				{
					for(unsigned int indexer=0; indexer<grid[i][j][k].size(); indexer++)
					{
						cout<<"rechk\ttested vertex"<<grid[i][j][k][indexer]->getId()<<endl;
						if( (cell.getVertexListSize()==2 && cell.testCircumCircle(*grid[i][j][k][indexer],newCenterRadius))
						  ||(cell.getVertexListSize()==3 && (tstSphrReturn=cell.testCircumSphere(*grid[i][j][k][indexer],newCenterRadius))))
						{
							tempRadius = newCenterRadius[3];
							if(tempRadius<minRadius)
							{
								if( (cell.getNeighListSize()>0 && !cell.checkOrientation(*grid[i][j][k][indexer]))
								 || (tstSphrReturn==midstate_val))
								{
									possibleFail=true;
									minRadius=tempRadius;
								}
								else
								{
									possibleFail=false;
									minRadius=tempRadius;
									winnerVertex=grid[i][j][k][indexer];
									cell.setCircumCenter(newCenterRadius);
									cell.setCircumRadius((newCenterRadius[3]>0)?newCenterRadius[3]:newCenterRadius[3]*-1);
								}
							}
						}
					}
				}
			}
	if(possibleFail)
		winnerVertex=NULL;
}

