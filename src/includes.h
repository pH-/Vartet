/*
 * includes.h
 *
 *  Created on: Jun 6, 2011
 *      Author: phanindrabhagavatula
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#ifdef __APPLE__
	#include <GLUT/glut.h>
	//#include <OpenGL/glext.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
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
#include <tr1/unordered_set>

enum axisToSort { X, Y, Z};


#include "tetstruc.h"

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
	cout<<"Fids:"<<intarray[0]<<","<<intarray[1]<<","<<intarray[2]<<endl;
}

#endif /* INCLUDES_H_ */
