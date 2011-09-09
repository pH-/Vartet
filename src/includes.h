/*
 * includes.h
 *
 *  Created on: Jun 6, 2011
 *      Author: phanindrabhagavatula
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#ifdef __APPLE__
	#include <GL/glut.h>
	//#include <OpenGL/glext.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
#ifdef __linux__
	#include<GL/glut.h>
	#include<GL/gl.h>
	#include<GL/glu.h>
#endif

#ifdef _WIN32 || _WIN64
	#include<GL/glut.h>
	#include<GL/gl.h>
	#include<GL/glu.h>
#endif

#include <list>
#include <deque>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <set>
#include <limits>
#include <tr1/unordered_set>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

enum axisToSort { X, Y, Z};
enum trippleBool{ false_val, true_val, midstate_val};
#define STEPSIZE 0.1


#include "tetstruc.h"
#include "kdtree.h"

#define CROSS(O,A,B)		 {(O)[0] = (A)[1]*(B)[2]-(A)[2]*(B)[1]; \
							  (O)[1] = (A)[2]*(B)[0]-(A)[0]*(B)[2]; \
							  (O)[2] = (A)[0]*(B)[1]-(A)[1]*(B)[0];}
#define DOT(A,B) ((A)[0]*(B)[0] + (A)[1]*(B)[1]+(A)[2]*(B)[2])

#define ADD(O,A,B)			 {(O)[0] = (A)[0]+(B)[0]; \
							  (O)[1] = (A)[1]+(B)[1]; \
							  (O)[2] = (A)[2]+(B)[2];}

#define MINUS(O,A,B)		 {(O)[0] = (A)[0]-(B)[0]; \
							  (O)[1] = (A)[1]-(B)[1]; \
							  (O)[2] = (A)[2]-(B)[2];}

#define ASSIGN(O,A)			 {(O)[0] = (A)[0]; \
							  (O)[1] = (A)[1]; \
							  (O)[2] = (A)[2];}

#define MULT(O,A,c)			 {(O)[0] = (A)[0]*c; \
							  (O)[1] = (A)[1]*c; \
							  (O)[2] = (A)[2]*c;}

#define ZERO(O)				 {(O)[0]=0.0;\
							  (O)[1]=0.0;\
							  (O)[2]=0.0;}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Geometric predicates                                                      //
//                                                                           //
// Return one of the values +1, 0, and -1 on basic geometric questions such  //
// as the orientation of point sets, in-circle, and in-sphere tests.  They   //
// are basic units for implmenting geometric algorithms.  TetGen uses two 3D //
// geometric predicates: the orientation and in-sphere tests.                //
//                                                                           //
// Orientation test:  let a, b, c be a sequence of 3 non-collinear points in //
// R^3.  They defines a unique hypeplane H.  Let H+ and H- be the two spaces //
// separated by H, which are defined as follows (using the left-hand rule):  //
// make a fist using your left hand in such a way that your fingers follow   //
// the order of a, b and c, then your thumb is pointing to H+.  Given any    //
// point d in R^3, the orientation test returns +1 if d lies in H+, -1 if d  //
// lies in H-, or 0 if d lies on H.                                          //
//                                                                           //
// In-sphere test:  let a, b, c, d be 4 non-coplanar points in R^3.  They    //
// defines a unique circumsphere S.  Given any point e in R^3, the in-sphere //
// test returns +1 if e lies inside S, or -1 if e lies outside S, or 0 if e  //
// lies on S.                                                                //
//                                                                           //
// The following routines use arbitrary precision floating-point arithmetic. //
// They are provided by J. R. Schewchuk in public domain (http://www.cs.cmu. //
// edu/~quake/robust.html). The source code are in "predicates.cxx".         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

double exactinit();
double orient3d(double *pa, double *pb, double *pc, double *pd);
double insphere(double *pa, double *pb, double *pc, double *pd, double *pe);
inline double vector3dLenSq(double vector[3])
{
	return vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2];
}
inline double determinant2x2 (double matrix[2][2])
{
	return matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0];
}


inline double determinant3x3 (double matrix[3][3])
{
	double subMatrix[2][2];
	double determinant=0.0;
	double intermediate;
	for(int i=0;i<3;i++)
	{
		for(int j=0,k=0;j<3;j++,k++)
		{
			if(i==j)
			{
				k--;
				continue;
			}
			subMatrix[0][k]=matrix[1][j];
			subMatrix[1][k]=matrix[2][j];
		}
		intermediate = determinant2x2(subMatrix);
		intermediate*=matrix[0][i];
		intermediate*=pow((-1.0),i);
		determinant+= intermediate;
	}
	if(determinant*pow(10.0,6) <1 && determinant*pow(10.0,6)> -1)
			return 0;
		else
			return determinant;
}
inline double determinant4x4 (double matrix[4][4])
{
	double subMatrix[3][3];
	double determinant=0.0;
	for(int i=0;i<4;i++)
	{
		for(int j=0,k=0;j<4;j++,k++)
		{
			if(i==j)
			{
				k--;
				continue;
			}
			subMatrix[0][k]=matrix[1][j];
			subMatrix[1][k]=matrix[2][j];
			subMatrix[2][k]=matrix[3][j];
		}
		determinant+= pow((-1.0),i)*matrix[0][i]*determinant3x3(subMatrix);
	}
	cout.flush();
	//TODO dirty piece of hook...
	if(determinant*pow(10.0,6) <1 && determinant*pow(10.0,6)> -1)
		return 0;
	else
		return determinant;
}

inline void sort3ints(int intarray[3])
{
	int tempNum;
	if(intarray[0]>intarray[1])
	{
		tempNum=intarray[1];
		intarray[1]=intarray[0];
		intarray[0]=tempNum;
	}
	if(intarray[1]>intarray[2])
	{
		tempNum=intarray[2];
		intarray[2]=intarray[1];
		intarray[1]=tempNum;

		if(intarray[0]>intarray[1])
		{
			tempNum=intarray[1];
			intarray[1]=intarray[0];
			intarray[0]=tempNum;
		}
	}
}

inline void sort4ints(int intarray[4])
{
	int tempNum;
	for(int i=0;i<3;i++)
		if(intarray[i]>intarray[i+1])
		{
			tempNum=intarray[i+1];
			intarray[i+1]=intarray[i];
			intarray[i]=tempNum;
		}
	sort3ints(intarray);
}

inline bool findMedianToRight(deque<deque<Vertex>::pointer >& vertexList,int stride, int& newIndex, int axisToUse)
{
	for(unsigned int i=vertexList.size()/2+stride/2; i<=vertexList.size()/2+stride-1; i++)
		if(vertexList[i+1]->getCoord((axisToSort)axisToUse)!= vertexList[i]->getCoord((axisToSort)axisToUse))
		{
			newIndex=i+1;
			return true;
		}
	return false;
}
inline bool findMedianToLeft(deque<deque<Vertex>::pointer >& vertexList,int stride, int& newIndex, int axisToUse)
{
	for(unsigned int i=vertexList.size()/2-stride/2; i>=vertexList.size()/2-stride+1; i--)
		if(vertexList[i-1]->getCoord((axisToSort)axisToUse)!= vertexList[i]->getCoord((axisToSort)axisToUse))
		{
			newIndex=i;
			return true;
		}
	return false;
}

inline int findMedian(deque<deque<Vertex>::pointer >& vertexList, int axisToUse)
{
//	unsigned int medianIndex= vertexList.size()/2;
	int i=1;
	int newMedianIndex=-1;
	unsigned int j;
	while(1)
	{
		if(findMedianToRight(vertexList,i,newMedianIndex,axisToUse))
			return newMedianIndex;
		else if(findMedianToLeft(vertexList,i,newMedianIndex,axisToUse))
			return newMedianIndex;
		i*=2;
		if(vertexList.size()/2+i > vertexList.size())
			break;
	}
	for(j=vertexList.size()/2+i/2; j<vertexList.size(); j++)
	{
		if(vertexList[j+1]->getCoord((axisToSort)axisToUse)!=vertexList[j]->getCoord((axisToSort)axisToUse))
				break;
	}
	return j+1;
}
inline void printMap(map<string,deque<Face>::pointer>& mapToPrint)
{
	map<string,deque<Face>::pointer>::iterator mapit;
	cout<<"\tmap contents";
	for(mapit=mapToPrint.begin(); mapit!=mapToPrint.end(); mapit++)
	{
		cout<<":"<<(*mapit).first;
	}
	cout<<endl;
	cout.flush();
}
#endif /* INCLUDES_H_ */
