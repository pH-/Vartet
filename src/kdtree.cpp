/*
 * kdtree.cpp
 *
 *  Created on: Jul 21, 2011
 *      Author: phanindrabhagavatula
 */

#include"includes.h"
using namespace std;

bool sortVerticesX(deque<Vertex>::pointer p1, deque<Vertex>::pointer p2) { return (p1->getXCoord()<p2->getXCoord());}
bool sortVerticesY(deque<Vertex>::pointer p1, deque<Vertex>::pointer p2) { return (p1->getYCoord()<p2->getYCoord());}
bool sortVerticesZ(deque<Vertex>::pointer p1, deque<Vertex>::pointer p2) { return (p1->getZCoord()<p2->getZCoord());}

//treeNode Class...
treeNode* treeNode::getLeftChild()
{
	return leftChild;
}

treeNode* treeNode::getRightChild()
{
	return rightChild;
}

double treeNode::getKeyValue()
{
	return keyVal;
}

deque<Vertex>::pointer treeNode::getPtObject()
{
	return pointObj;
}

int treeNode::getaxis()
{
	return axis;
}

unsigned int treeNode::getsubTreeSize()
{
	return subTreeSize;
}

void treeNode::setsubtreeSize(unsigned int size)
{
	subTreeSize=size;
}
void treeNode::setaxis(int axisToChk)
{
	axis = axisToChk;
}

void treeNode::setLeftChild(treeNode* ilc)
{
	leftChild = ilc;
}

void treeNode::setRightChild(treeNode* irc)
{
	rightChild = irc;
}

void treeNode::setKeyValue(double keyValue)
{
	keyVal=keyValue;
}

void treeNode::setRegion(double boundingRegion[2][3])
{
	for(int i=0; i<3; i++)
		regionLow[i]=boundingRegion[0][i];
	for(int i=0; i<3; i++)
		regionHigh[i]=boundingRegion[1][i];
}

void treeNode::setDummy()
{
	dummy = true;
}
void treeNode::attachPtObject(deque<Vertex>::pointer partRef)
{
	pointObj=partRef;
}

bool treeNode::isLeaf()
{
	return(!leftChild && !rightChild);
}

bool treeNode::isDummy()
{
	return dummy;
}

trippleBool treeNode::testRegion(double queryRegion[2][3])
{
//	trippleBool returnVal=false_val;
	char returnBits=0x00;
	for(int j=0; j<3; j++)
	{
		if(regionLow[j]>queryRegion[1][j] || regionHigh[j]<queryRegion[0][j])
			return false_val;
		if(regionLow[j]>=queryRegion[0][j])
		{
			if(j==0)
				returnBits|=0x20;
			else if(j==1)
				returnBits|=0x10;
			else if(j==2)
				returnBits|=0x08;
		}
		if(regionHigh[j]<=queryRegion[1][j])
		{
			if(j==0)
				returnBits|=0x04;
			else if(j==1)
				returnBits|=0x02;
			else if(j==2)
				returnBits|=0x01;
		}
	}
	if((0x24==(0x24 & returnBits)) && (0x12==(0x12 & returnBits)) && (0x09==(0x09 & returnBits)))
		return true_val;
	else
		return midstate_val;
}

//kdtree class...

treeNode* kdtree::getRoot()
{
	return root;
}

void kdtree::setRoot(treeNode* newRoot)
{
	root = newRoot;
}
void kdtree::buildTree(deque<Vertex>& vertexList)
{
	double max = numeric_limits<double>::max();
	double regionBox[][3]={{-max, -max, -max}, {max, max, max}};
	for(unsigned int i=0; i<vertexList.size(); i++)
		listOfVertices.push_back(&vertexList[i]);
	sortList(listOfVertices,0);
	root = buildTreeInternal(listOfVertices,0,regionBox);
	listOfVertices.clear();
	cout<<"tree built:"<<endl;
}

void kdtree::searchNodes(deque<deque<Vertex>::pointer >& nodeList, treeNode *root, double distance, double position[3])
{
	double queryRegion[2][3];
//	trippleBool testResult;
	for(int i=0; i<2; i++)
		for(int j=0; j<3; j++)
			queryRegion[i][j] = position[j]+ pow(-1.0, i+1)*distance;
	searchNodes(nodeList,root,queryRegion);
}

void kdtree::searchNodes(deque<deque<Vertex>::pointer >& nodeList, treeNode *root, double queryRegion[2][3])
{
	trippleBool testResult;
	if(root->isLeaf())
	{
		if( !root->isDummy() && root->getPtObject()->getCoord((axisToSort)root->getaxis())>=queryRegion[0][root->getaxis()]
	    && root->getPtObject()->getCoord((axisToSort)root->getaxis())<=queryRegion[1][root->getaxis()])
			nodeList.push_back(root->getPtObject());
	}
	else
	{
		testResult = root->getLeftChild()->testRegion(queryRegion);
		if(testResult == true_val)
			reportSubTree(nodeList,root->getLeftChild());
		else if(testResult==midstate_val)
			searchNodes(nodeList, root->getLeftChild(), queryRegion);
		testResult = root->getRightChild()->testRegion(queryRegion);
		if(testResult == true_val)
			reportSubTree(nodeList,root->getRightChild());
		else if(testResult==midstate_val)
			searchNodes(nodeList,root->getRightChild(),queryRegion);
	}
}

void kdtree::reportSubTree(deque<deque<Vertex>::pointer >& nodeList, treeNode *subRoot)
{
	if(subRoot->isLeaf())
	{
		if(!subRoot->isDummy())
			nodeList.push_back(subRoot->getPtObject());
	}
	else
	{
		reportSubTree(nodeList,subRoot->getLeftChild());
		reportSubTree(nodeList,subRoot->getRightChild());
	}
}

//treeNode* kdtree::getTreeMin()
//{
//	if(!root->getLeftChild())
//		return root;
//	else
//		return treeMinSubtree(root->getLeftChild());
//}
//
//treeNode* kdtree::getTreeMax()
//{
//	if(root->getRightChild())
//		return root;
//	else
//		return treeMaxSubtree(root->getRightChild());
//}

deque<Vertex>::pointer kdtree::nearestNeighbour(deque<Vertex>::pointer queryPoint)
{
	double max = numeric_limits<double>::max();
	double tempNearest = max;
	treeNode* nnVertex = nearestNeighbourInternal(root->getRightChild(), queryPoint, tempNearest);
	return nnVertex->getPtObject();
}

deque<Vertex>::pointer kdtree::nearestNeighbour(deque<Vertex>::pointer queryPoint, double tempNearest)
{
	treeNode* nnVertex = nearestNeighbourInternal(root->getRightChild(), queryPoint, tempNearest);
	return nnVertex->getPtObject();
}

treeNode* kdtree::buildTreeInternal(deque<deque<Vertex>::pointer >& vertexList, int level, double regionBox[2][3])
{
	treeNode *newNode = new treeNode();
	int axisToUse = level%3;
	newNode->setaxis(axisToUse);
	newNode->setsubtreeSize(vertexList.size());
	deque<deque<Vertex>::pointer > leftSubTreeList;
	deque<deque<Vertex>::pointer > rightSubTreeList;
	if(vertexList.size()==0)
	{
		newNode->setDummy();
	}
	else if(vertexList.size()==1)
	{
		newNode->attachPtObject(vertexList[0]);
		newNode->setRegion(regionBox);
	}
	else
	{
		treeNode *leftChild, *rightChild;
		unsigned int medianIndex;
		double lcRegionBox[2][3], rcRegionBox[2][3];
		memcpy(lcRegionBox, regionBox, 2*sizeof(*regionBox));
		memcpy(rcRegionBox, regionBox, 2*sizeof(*regionBox));
		if(vertexList.size()>2)
			medianIndex = findMedian(vertexList,axisToUse);
		else
			medianIndex = 1;
//		for(unsigned int i=medianIndex; i<vertexList.size(); i++)
//		{
//			if(vertexList[i+1]->getCoord((axisToSort)axisToUse) == vertexList[i]->getCoord((axisToSort)axisToUse))
//				medianIndex=i+1;
//		}
		for(unsigned int i=0; i<medianIndex; i++)
			leftSubTreeList.push_back(vertexList[i]);

		for(unsigned int i=medianIndex; i<vertexList.size(); i++)
			rightSubTreeList.push_back(vertexList[i]);

		//double keyVal = vertexList[medianIndex-1]->getCoord((axisToSort)axisToUse) + (vertexList[medianIndex]->getCoord((axisToSort)axisToUse) - vertexList[medianIndex-1]->getCoord((axisToSort)axisToUse))/2;
		double keyVal = vertexList[medianIndex-1]->getCoord((axisToSort)axisToUse);
		lcRegionBox[1][axisToUse]=vertexList[medianIndex-1]->getCoord((axisToSort)axisToUse);
		rcRegionBox[0][axisToUse]=vertexList[medianIndex]->getCoord((axisToSort)axisToUse);
		newNode->attachPtObject(vertexList[medianIndex-1]);
		newNode->setKeyValue(keyVal);
		newNode->setRegion(regionBox);
		sortList(leftSubTreeList,(axisToUse+1)%3);
		sortList(rightSubTreeList,(axisToUse+1)%3);
		leftChild = buildTreeInternal(leftSubTreeList,level+1,lcRegionBox);
		rightChild = buildTreeInternal(rightSubTreeList, level+1,rcRegionBox);
		newNode->setLeftChild(leftChild);
		newNode->setRightChild(rightChild);
	}
	return newNode;
}

void kdtree::sortList(deque<deque<Vertex>::pointer >& list, int axis)
{
	if(axis==0)
		sort(list.begin(), list.end(),sortVerticesX);
	else if(axis == 1)
		return sort(list.begin(), list.end(),sortVerticesY);
	else
		return sort(list.begin(), list.end(),sortVerticesZ);;
}

//treeNode* kdtree::treeMinSubtree(treeNode* subRoot)
//{
//	if(!subRoot->getLeftChild())
//		return subRoot;
//	else
//		return treeMinSubtree(subRoot->getLeftChild());
//}
//
//treeNode* kdtree::treeMaxSubtree(treeNode* subRoot)
//{
//	if(!subRoot->getRightChild())
//		return subRoot;
//	else
//		return treeMaxSubtree(subRoot->getRightChild());
//}

treeNode* kdtree::nearestNeighbourInternal(treeNode* root, deque<Vertex>::pointer queryPoint, double& bestDist)
{
	treeNode* candidateNearest=NULL;
	treeNode* tempNearestVertex;
	double tempNearestDist;
	if(root->isLeaf())
	{
		if(root->isDummy())
			return candidateNearest;
		tempNearestVertex = root;
		if(tempNearestVertex)
		{
			tempNearestDist = queryPoint->getSqDistance(*tempNearestVertex->getPtObject());

			if(tempNearestDist<bestDist)
			{
				bestDist=tempNearestDist;
				candidateNearest=tempNearestVertex;
			}
		}
		return candidateNearest;
	}
	else if(queryPoint->getCoord((axisToSort)root->getaxis()) < root->getKeyValue())
	{
		tempNearestVertex = nearestNeighbourInternal(root->getLeftChild(), queryPoint, bestDist);

		if(tempNearestVertex)
		{
			tempNearestDist = queryPoint->getSqDistance(*tempNearestVertex->getPtObject());

			if(tempNearestDist<=bestDist)
			{
				bestDist=tempNearestDist;
				candidateNearest=tempNearestVertex;
			}
		}
		if(bestDist > pow((queryPoint->getCoord((axisToSort)root->getaxis()) - root->getKeyValue()),2))
			tempNearestVertex = nearestNeighbourInternal(root->getRightChild(), queryPoint, bestDist);
		else
			return candidateNearest;

		if(tempNearestVertex)
			return tempNearestVertex;
		else
			return candidateNearest;

//		else
//			return candidateNearest;
	}
	else
	{
		tempNearestVertex = nearestNeighbourInternal(root->getRightChild(), queryPoint, bestDist);

		if(tempNearestVertex)
		{
			tempNearestDist = queryPoint->getSqDistance(*tempNearestVertex->getPtObject());

			if(tempNearestDist<=bestDist)
			{
				bestDist=tempNearestDist;
				candidateNearest=tempNearestVertex;
			}
		}
		if(bestDist > pow((queryPoint->getCoord((axisToSort)root->getaxis()) - root->getKeyValue()),2))
			tempNearestVertex = nearestNeighbourInternal(root->getLeftChild(), queryPoint, bestDist);
		else
			return candidateNearest;

		if(tempNearestVertex)
			return tempNearestVertex;
		else
			return candidateNearest;

//		else
//			return candidateNearest;
	}
}

