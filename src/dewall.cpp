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

struct classcomp {
  bool operator() (const int f1id, const int f2id)
  {return f1id<f2id;}
};

void Solid::dewall(axisToSort axis, deque<Vertex>& vertices, map<int,deque<Face>::pointer,classcomp>& activeFaceList, deque<deque<deque<deque<deque<Vertex>::pointer > > > >& grid)
{
	int medianIndex = vertices.size() / 2, i = 0;
	double alphaPlane=0;
	map<int,deque<Face>::pointer,classcomp> aflalpha;
	map<int,deque<Face>::pointer,classcomp> afllow;
	map<int,deque<Face>::pointer,classcomp> aflhigh;
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

	if (activeFaceList.size() == 0)
	{
		newCell = new Cell();
		if (newCell->getVertexListSize() <=4) {
			newCell->addVertex(vertices[medianIndex]);
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
			if((listOfCells.back().getFaces()[i].getVertices()[0]->getXCoord())<alphaPlane
			&& (listOfCells.back().getFaces()[i].getVertices()[1]->getXCoord())<alphaPlane
			&& (listOfCells.back().getFaces()[i].getVertices()[2]->getXCoord())<alphaPlane)
			{
				afllow.insert(pair<int,deque<Face>::pointer>(listOfCells.back().getFaces()[i].id, &listOfCells.back().getFaces()[i]));
			}
			else if((listOfCells.back().getFaces()[i].getVertices()[0]->getXCoord())>alphaPlane
				 && (listOfCells.back().getFaces()[i].getVertices()[1]->getXCoord())>alphaPlane
				 && (listOfCells.back().getFaces()[i].getVertices()[2]->getXCoord())>alphaPlane)
			{
				aflhigh.insert(pair<int,deque<Face>::pointer>(listOfCells.back().getFaces()[i].id, &listOfCells.back().getFaces()[i]));
			}
			else
				aflalpha.insert(pair<int,deque<Face>::pointer>(listOfCells.back().getFaces()[i].id, &listOfCells.back().getFaces()[i]));
		}
		cout<<"aflalpha:"<<aflalpha.size()<<endl;
		cout<<"afllow:"<<afllow.size()<<endl;
		cout<<"aflhigh"<<aflhigh.size()<<endl;
		cout.flush();
	}
	while(aflalpha.size())
	{
		map<int,deque<Face>::pointer,classcomp>::iterator aflit;
		multimap<deque<Cell>::pointer, deque<Face>::pointer>::iterator niter;
		for(aflit=aflalpha.begin(); aflit!=aflalpha.end(); )
		{
			newCell = new Cell();
			listOfCells.push_back(*newCell);
			listOfCells.back().addFEVs(*(*aflit).second);
			makeCell(listOfCells.back(),grid,alphaPlane);
			for(niter=listOfCells.back().getNeighbours().begin(); niter!=listOfCells.back().getNeighbours().end(); niter++)
			{
				aflalpha.erase((*((*niter).first->getNeighbours().find(&listOfCells.back()))).second->id);
			}
		}
//		for(unsigned int i=0; i<listOfCells.size(); i++)
//		{
//			if(listOfCells[i].getVertexListSize()==3)
//				makeCell(listOfCells[i],grid, alphaPlane);
//		}
	}
}

void Solid::makeCell(deque<Cell>::reference cell, deque<deque<deque<deque<deque<Vertex>::pointer > > > >& grid, double alphaPlane)
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
		centerCoords=(int*)cell.getVertices()[0].sparePtr;
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
									minSqDist=cell.getVertices()[0].getSqDistance(*grid[i][j][k][indexer]);
									winnerVertex=grid[i][j][k][indexer];
								}
								else
								{
									tempSqDist = cell.getVertices()[0].getSqDistance(*grid[i][j][k][indexer]);
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
		cell.addVertex(*winnerVertex);
		newEdge = new Edge();
		cell.addEdge(*newEdge);
		cell.getEdges()[0].setVertices(&cell.getVertices()[0],&cell.getVertices()[1]);
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
		int layer=0;
		stepSize =0;
		double minRadius=-1;
		double tempRadius;
		double centerRadius[4];
		bool   twoVertices, endSearch=true;
		gridCoord1 = (unsigned int*)malloc(sizeof(unsigned int)*4);
		gridCoord2 = (unsigned int*)malloc(sizeof(unsigned int)*4);
		gridCoord3 = (unsigned int*)malloc(sizeof(unsigned int)*4);

		//cout<<"Vertex1:"<<cell.getVertices()[0].getXCoord()<<","<<cell.getVertices()[0].getYCoord()<<","<<cell.getVertices()[0].getZCoord()<<endl;
		//cout<<"Vertex2:"<<cell.getVertices()[1].getXCoord()<<","<<cell.getVertices()[1].getYCoord()<<","<<cell.getVertices()[1].getZCoord()<<endl;
		gridCoord1=(unsigned int*)cell.getVertices()[0].sparePtr;
		gridCoord2=(unsigned int*)cell.getVertices()[1].sparePtr;

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
			gridCoord3=(unsigned int*)cell.getVertices()[2].sparePtr;
			bboxMin[0]= (bboxMin[0]<gridCoord3[0])?bboxMin[0]:gridCoord3[0];
			bboxMax[0]= (bboxMax[0]>gridCoord3[0])?bboxMax[0]:gridCoord3[0];
			bboxMin[1]= (bboxMin[1]<gridCoord3[1])?bboxMin[1]:gridCoord3[1];
			bboxMax[1]= (bboxMax[1]>gridCoord3[1])?bboxMax[1]:gridCoord3[1];
			bboxMin[2]= (bboxMin[2]<gridCoord3[2])?bboxMin[2]:gridCoord3[2];
			bboxMax[2]= (bboxMax[2]>gridCoord3[2])?bboxMax[2]:gridCoord3[2];
		}

		while(winnerVertex==NULL && endSearch)
		{
			endSearch=false;
			for(unsigned int i = bboxMin[0]-stepSize; i<grid.size() && i<=bboxMax[0]+stepSize; i++)
			{
				for(unsigned int j=bboxMin[1]-stepSize; j<grid[i].size() && j<=bboxMax[1]+stepSize ;j++)
					for(unsigned int k=bboxMin[2]-stepSize; k<grid[i][j].size() && k<=bboxMax[2]+stepSize; k++)
					{
						if(layer&& (i-(bboxMin[0]-stepSize) >0 && j-(bboxMin[1]-stepSize)>0 && k-(bboxMin[2]-stepSize)>0)
								&&(((bboxMax[0]+stepSize)-i > 0) && ((bboxMax[1]+stepSize)-j > 0) && ((bboxMax[2]+stepSize)-k > 0)))
							continue;
						if(grid[i][j][k].size()==0)
							continue;
						else
						{
							for(unsigned int indexer=0; indexer<grid[i][j][k].size(); indexer++)
							{
								if(cell.getNeighListSize()>0 && !cell.checkOrientation(*grid[i][j][k][indexer]))
									continue;
								if((cell.getVertexListSize()==2 && cell.testCircumCircle(*grid[i][j][k][indexer],centerRadius))
								 ||(cell.getVertexListSize()==3 && cell.testCircumSphere(*grid[i][j][k][indexer],centerRadius)))
								{
									if(minRadius<0)
									{
										minRadius=centerRadius[3];
										winnerVertex=grid[i][j][k][indexer];
									}
									else
									{
										tempRadius = centerRadius[3];
										if(tempRadius<minRadius)
										{
											minRadius=tempRadius;
											winnerVertex=grid[i][j][k][indexer];
										}
									}
								}

							}
							if(winnerVertex)
								goto breakAllLoops;
						}
					}
				endSearch=true;
			}
			layer++;
			stepSize++;
		}
		breakAllLoops:
		if(winnerVertex)
		{
			confirmWinnerVertex(cell,winnerVertex,centerRadius,grid);
			cell.addVertex(*winnerVertex);
		}

		if(twoVertices && winnerVertex)
		{
			newEdge1 = new Edge();
			newEdge2 = new Edge();
			cell.addEdge(*newEdge1);
			cell.addEdge(*newEdge2);
			cell.getEdges()[1].setVertices(&cell.getVertices()[1],&cell.getVertices()[2]);
			cell.getEdges()[2].setVertices(&cell.getVertices()[0],&cell.getVertices()[2]);
			newFace  = new Face();
			cell.addFace(*newFace);
			cell.getFaces()[0].addVertices(&cell.getVertices()[0],&cell.getVertices()[1],&cell.getVertices()[2]);
			cell.getFaces()[0].addEdges(&cell.getEdges()[0],&cell.getEdges()[1],&cell.getEdges()[2]);
			cell.getFaces()[0].incrNumOfOpenFaces(grid);
		}
		if(!twoVertices && winnerVertex)
		{
			newEdge1 = new Edge();
			newEdge2 = new Edge();
			newEdge3 = new Edge();
			Face* newFace1 = new Face();
			Face* newFace2 = new Face();
			Face* newFace3 = new Face();
			cell.addEdge(*newEdge1);
			cell.addEdge(*newEdge2);
			cell.addEdge(*newEdge3);
			cell.getEdges()[3].setVertices(&cell.getVertices()[0],&cell.getVertices()[3]);
			cell.getEdges()[4].setVertices(&cell.getVertices()[1],&cell.getVertices()[3]);
			cell.getEdges()[5].setVertices(&cell.getVertices()[2],&cell.getVertices()[3]);
			cell.addFace(*newFace1);
			cell.addFace(*newFace2);
			cell.addFace(*newFace3);
			cell.getFaces()[1].addVertices(&cell.getVertices()[0],&cell.getVertices()[1],&cell.getVertices()[3]);
			cell.getFaces()[2].addVertices(&cell.getVertices()[1],&cell.getVertices()[2],&cell.getVertices()[3]);
			cell.getFaces()[3].addVertices(&cell.getVertices()[2],&cell.getVertices()[0],&cell.getVertices()[3]);

			cell.getFaces()[1].addEdges(&cell.getEdges()[0],&cell.getEdges()[4],&cell.getEdges()[3]);
			cell.getFaces()[2].addEdges(&cell.getEdges()[1],&cell.getEdges()[5],&cell.getEdges()[4]);
			cell.getFaces()[3].addEdges(&cell.getEdges()[2],&cell.getEdges()[3],&cell.getEdges()[5]);

			cell.getFaces()[1].incrNumOfOpenFaces(grid);
			cell.getFaces()[2].incrNumOfOpenFaces(grid);
			cell.getFaces()[3].incrNumOfOpenFaces(grid);

			cell.getFaces()[0].addOppositeVertex(&cell.getVertices()[3]);
			cell.getFaces()[1].addOppositeVertex(&cell.getVertices()[2]);
			cell.getFaces()[2].addOppositeVertex(&cell.getVertices()[0]);
			cell.getFaces()[3].addOppositeVertex(&cell.getVertices()[1]);
		}
	}

}

void Solid::confirmWinnerVertex(deque<Cell>::reference cell,Vertex*& winnerVertex, double centerRadius[4],deque<deque<deque<deque<deque<Vertex>::pointer > > > >& grid)
{
	unsigned int stepSize = floor(centerRadius[3]);
	double minRadius = centerRadius[3];
	double tempRadius = centerRadius[3];
	double newCenterRadius[4];
	int centerGrid[3];
	int *vertexGridCoords;
	vertexGridCoords = (int*)malloc(sizeof(int)*4);
	vertexGridCoords = (int*)cell.getVertices()[0].sparePtr;
	centerGrid[0]=vertexGridCoords[0]+(floor(centerRadius[0])-floor(cell.getVertices()[0].getXCoord()));
	centerGrid[1]=vertexGridCoords[1]+(floor(centerRadius[1])-floor(cell.getVertices()[0].getYCoord()));
	centerGrid[2]=vertexGridCoords[2]+(floor(centerRadius[2])-floor(cell.getVertices()[0].getZCoord()));

	for(unsigned int i = centerGrid[0]-stepSize; i<grid.size() && i<=centerGrid[0]+stepSize; i++)
		for(unsigned int j=centerGrid[1]-stepSize; j<grid[i].size() && j<=centerGrid[1]+stepSize ;j++)
			for(unsigned int k=centerGrid[2]-stepSize; k<grid[i][j].size() && k<=centerGrid[2]+stepSize; k++)
			{
				if(grid[i][j][k].size()==0)
					continue;
				else
				{
					for(unsigned int indexer=0; indexer<grid[i][j][k].size(); indexer++)
					{
						if(cell.getNeighListSize()>0 && !cell.checkOrientation(*grid[i][j][k][indexer]))
							continue;
						if( (cell.getVertexListSize()==2 && cell.testCircumCircle(*grid[i][j][k][indexer],newCenterRadius))
						  ||(cell.getVertexListSize()==3 && cell.testCircumSphere(*grid[i][j][k][indexer],newCenterRadius)))
						{
							tempRadius = newCenterRadius[3];
							if(tempRadius<minRadius)
							{
								minRadius=tempRadius;
								winnerVertex=grid[i][j][k][indexer];
							}
						}
					}
				}
			}
}

