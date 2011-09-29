//============================================================================
// Name        : Vartet.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "includes.h"
using namespace std;
extern int firstCell;
extern int lastCell;
extern int faceToShow;
extern bool showCircle;
extern bool wire;

Solid model;
double camMotionStep=0.5;
double eyeX=0, eyeY=0, eyeZ=0;
double lookatx=0, lookaty=0, lookatz=0;
void init(void)
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	lastCell=model.listOfCellsSize();

}

void drawAxes()
{
	glPushMatrix();
	glTranslated(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,1.0);
	glVertex3d(0,0,0);
	glVertex3d(20.0,0,0);
	glColor3f(0.0,1.0,0.0);
	glVertex3d(0,0,0);
	glVertex3d(0,20.0,0);
	glColor3f(0.0,0.0,1.0);
	glVertex3d(0,0,0);
	glVertex3d(0,0,20.0);
	glEnd();
	glPopMatrix();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(8+eyeX,8+eyeY,8+eyeZ, lookatx,lookaty,lookatz, 0,1,0);
	model.drawEdges();
	drawAxes();
	glColor4f(1.0,1.0,0.0,1.0);
	glutSwapBuffers();
}



void reshape(int w, int h)
{
/*
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (50.0, (GLdouble)w/(GLdouble)h, 3.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
	/*glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	   glMatrixMode (GL_PROJECTION);
	   glLoadIdentity();
	   if (w <= h)
	      gluOrtho2D (0, 500, 0*(GLfloat)h/(GLfloat)w,
	         500*(GLfloat)h/(GLfloat)w);
	   else
	      gluOrtho2D (0*(GLfloat)w/(GLfloat)h,
	         500*(GLfloat)w/(GLfloat)h, 0, 500);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();*/
	glViewport(0,0,w,h);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	gluPerspective (55.0, (GLdouble)w/(GLdouble)h, 1.0, 3000.0);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
			exit(0);
			break;
		case 'x':
			eyeX-=camMotionStep;
			glutPostRedisplay();
			break;
		case 'X':
			eyeX+=camMotionStep;
			glutPostRedisplay();
			break;
		case 'u':
			lookatx-=camMotionStep;
			glutPostRedisplay();
			break;
		case 'U':
			lookatx+=camMotionStep;
			glutPostRedisplay();
			break;
		case 'Y':
			eyeY+=camMotionStep;
			glutPostRedisplay();
			break;
		case 'y':
			eyeY-=camMotionStep;
			glutPostRedisplay();
			break;
		case 'v':
			lookaty-=camMotionStep;
			glutPostRedisplay();
			break;
		case 'V':
			lookaty+=camMotionStep;
			glutPostRedisplay();
			break;
		case 'z':
			eyeZ-=camMotionStep;
			glutPostRedisplay();
			break;
		case 'Z':
			eyeZ+=camMotionStep;
			glutPostRedisplay();
			break;
		case 'w':
			lookatz-=camMotionStep;
			glutPostRedisplay();
			break;
		case 'W':
			lookatz+=camMotionStep;
			glutPostRedisplay();
			break;
		case '0':
			firstCell=0;
			lastCell=model.listOfCellsSize();
			glutPostRedisplay();
			break;
		case '1':
			if(firstCell<model.listOfCellsSize()-1)
				firstCell++;
			else
				firstCell=0;
			lastCell=firstCell+1;
			glutPostRedisplay();
			break;
		case '2':
			firstCell=0;
			if(lastCell<firstCell)
				lastCell=firstCell;
			else if(lastCell<model.listOfCellsSize())
				lastCell++;
			else
				lastCell=firstCell;
			glutPostRedisplay();
			break;
		case '3':
			faceToShow=(faceToShow+1)%54;
			glutPostRedisplay();
			break;
		case '4':
			if(firstCell>0)
				firstCell--;
			glutPostRedisplay();
			break;
		case 'c':
			showCircle=!showCircle;
			glutPostRedisplay();
			break;
		case 's':
			wire=!wire;
			glutPostRedisplay();
			break;
//		case 'd':
//		case 'D':
//			model.delaunize();
//			glutPostRedisplay();
//			break;
		default:
			break;
	}
}

/*void mouse(int button, int state, int x, int y)
{
   switch (button) {

      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            float newPt[1][2];
            convertCoord(x,y);
            if(numPoints>2)
            {
            	float tempRad=distance(newPt[0],points[numPoints]);
            	drawCircle(tempRad,newPt[0][0],newPt[0][1]);
            }
            theflag=1;
            glutPostRedisplay();
         }
         if (state == GLUT_UP) {
        	 	 	 convertCoord(x,y);
        	 	 	 points[numPoints][0]=newPt[0][0];
        	 	 	 points[numPoints][1]=newPt[0][1];
        	 	 	 theflag=0;
                     glutPostRedisplay();
                     numPoints++;
                  }
         break;
      default:
         break;
   }
}*/


int main (int argc, char** argv)
{
	Plc inputModel;
	cout<<argv[1];
	inputModel.parsePolyFile(argv[1]);
//	model.populateVertices(inputModel.getPlcVertices());
	model.populateVerticesRandom();
	exactinit();
	model.delaunize();
//TODO below line is not req.. its present for debugging..
	srand(time(0));
//TODO delete code above
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow("Variational Tetrahedralization");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc (keyboard);
	init();
	//glutMouseFunc(mouse);
	//glutMotionFunc(activemouse);
	//glutPassiveMotionFunc(passivemouse);
	glutMainLoop();
	return 0;

}

