/*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*
 *	SIMPPAINT
 *	~~~~~~~~~
 *	By: Alexander Guerrero
 *	Created: 08/10/2015 (dd/mm/yyyy)
 *
 * 	This is SimpPaint, a simple version of MS paint, created for
 *  Assignment 1 in SWFR ENG 3GC3.
 *	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/

#include "GraphicsMathLib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>    
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

std::string drawMode = "point"; 
Colour colour = Colour();
int windowSizeX  = 500;
int windowSizeY = 500;

// storing data for 2 mouse clicks
int clicks; 
int x1; int y1;
int x2; int y2;

Colour colourArr[8] = { Colour(1.0, 1.0, 1.0),  // 0 white
						Colour(0.0, 0.0, 0.0),  // 1 black
						Colour(1.0, 0.0, 0.0),  // 2 red
						Colour(1.0, 0.5, 0.0),  // 3 orange
						Colour(1.0, 1.0, 0.0),  // 4 yellow 
						Colour(0.0, 1.0, 0.0),  // 5 green
						Colour(0.0, 0.0, 1.0),  // 6 blue
						Colour(1.0, 0.0, 1.0) };// 7 purple


void displayBackground()
{	/* 
	 * Sets entire screen to white
	 */
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, windowSizeY);
		glVertex2f(windowSizeX, windowSizeY);
		glVertex2f(windowSizeY, 0.0);
	glEnd();
	glFlush();
}


void drawPoint(int x, int y)
{
	glColor3f(colour.r, colour.g, colour.b);
	glPointSize(5);

	glBegin(GL_POINTS);
		glVertex2f(x, y);
	glEnd();

	glFlush(); 
}


void drawLine(int x1, int y1, int x2, int y2)
{	// check point & adjust points for Bresenham's
	int tmp;
	if(x1 > x2)
	{	// swap (x1, y1) & (x2, y2)
		tmp = x1;
		x1 = x2;
		x2 = tmp;

		tmp = y1;
		y1 = y2;
		y2 = tmp;
	} 

	/*
	 * Bresenham's line algorithm
	 */
	int dx = x2 - x1;
	int dy = y2 - y1;

	if(y1 > y2){dy = -dy;} // for negative slope

	int flag;
	if(dy > dx)
	{ // swap dx & dy
		tmp = dx; 
		dx = dy; 
		dy = tmp;
		flag = 1;
	}

	int d = 2*dy - dx;

	int incrE = dy*2;
	int incrNE = 2*dy - 2*dx;

	int x = x1;
	int y = y1;

	// draw initial point
	glColor3f(colour.r, colour.g, colour.b);
	glPointSize(3);
	glBegin(GL_POINTS);
		glVertex2f(x, y);
	glEnd();

	while (x < x2)
	{
		if(d <= 0 && flag == 1 && y1>y2)
		{
			d += incrE;
			y--;
		} else if(d <= 0 && flag == 1){
			d += incrE;
			y++;
		} else if(d <= 0){
			d += incrE;
			x++;
		} else if(y1 > y2) {
			d += incrNE;
			x++;
			y--;
		} else {
			d += incrNE;
			x++;
			y++;
		}
		glBegin(GL_POINTS);
			glVertex2f(x, y);
		glEnd();
	}

	if(y1 > y2)
	{
		for(y; y > y2; y--)
		{
			glBegin(GL_POINTS);
				glVertex2f(x, y);
			glEnd();
		}	
	} else { 
		for(y; y < y2; y++)
		{
			glBegin(GL_POINTS);
				glVertex2f(x, y);
			glEnd();
		}
	}
}


void drawRectangle(int x1, int y1, int x3, int y3)
{/* start with this and change depending on second clk, (x3,y3)
  *		(x1,y1)				(x2,y2)
  *		*......................*
  *		:					   :
  *		:					   :
  *		*......................*
  *		(x4,y4)				(x3,y3)
  */

	// (x2, y2)
	int x2 = x3; int y2 = y1;

	// (x4, y4)
	int x4 = x1; int y4 = y3;

	int dx = x3 - x1;
	int dy = y3 - y1;

	if(dx == 0 || dy == 0)
	{
		drawLine(x1, y1, x3, y3);
	} else {
		// draws horizonal lines
		drawLine(x1, y1, x2, y2);
		drawLine(x4, y4, x3, y3);
		// draws vertical lines
		drawLine(x1, y1, x4, y4);
		drawLine(x2, y2, x3, y3);
		// fill with colour
		if(dx > 0)
		{
			for(int i = x1+1; i<x3; i++)
			{
				x4 += 1;
				drawLine(i, y1, x4, y4);
			}
		} else {
			for(int i = x1-1; i>x3; i--)
			{
				x4 -= 1;
				drawLine(i, y1, x4, y4);			
			} 
		}
	}
}


void drawCircle(int x1, int y1, int x2, int y2)
{	// create vector from 2 points and calc radius of circle (aka mag.)
	float dx = x2 - x1;
	float dy = y2 - y1;

	if(dx == 0){dx = 1;} 
	else if(dy == 0){dy = 1;}

	Vec2D circVec = Vec2D(dx, dy, 0);
	float radiusf = circVec.calcMag();

	/*
	 * Midpoint circle algorithm
	 * https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	 */

	// loop fills cirlce with colour, draws progressively smaller circles
	for(int i = (int)radiusf; i>=0; i--)
	{ 
		int x = i;
		int y = 0;
		int decisionOver2 = 1 - x;

		glColor3f(colour.r, colour.g, colour.b);
		glPointSize(3);

		while(y <= x)
		{
			glBegin(GL_POINTS);
		    	glVertex2f(x + x1, y + y1);  // Octant 1
		    	glVertex2f(y + x1, x + y1);  // Octant 2
				glVertex2f(-x + x1, y + y1); // Octant 4
		 		glVertex2f(-y + x1, x + y1); // Octant 3
		 	  	glVertex2f(-x + x1, -y + y1);// Octant 5
		 		glVertex2f(-y + x1, -x + y1);// Octant 6
		 	  	glVertex2f(x + x1, -y + y1); // Octant 8
		 	  	glVertex2f(y + x1, -x + y1); // Octant 7
		 	glEnd();
	    	y++;

	    	if (decisionOver2<=0)
	    	{
	    		// Change in decision criterion for y -> y+1
	    		decisionOver2 += 2 * y + 1;   
	    	} else {
	     		x--;
	     		// Change for y -> y+1, x -> x-1
	      		decisionOver2 += 2 * (y - x) + 1;
	    	}
	  	}
	}
}


void mouse(int btn, int state, int x, int y)
{	
	while (GLUT_LEFT_BUTTON == btn)
	{	// draw different shapes depending on drawMode
		
		if(state == GLUT_DOWN && drawMode == "point")
		{
			drawPoint(x,y);

		} else if(state == GLUT_DOWN && drawMode == "line"){

			if(clicks == 0)
			{
				x1 = x;
				y1 = y;
			} else if(clicks == 1){
				x2 = x;
				y2 = y;
				drawLine(x1, y1, x2, y2);
				clicks = -1; // reset clicks, -1 because the release
			} 				 // of left mouse increments clicks
			break;

		} else if(state == GLUT_DOWN && drawMode == "rectangle"){

			if(clicks == 0)
			{
				x1 = x;
				y1 = y;
			} else if(clicks == 1){
				x2 = x;
				y2 = y;
				drawRectangle(x1, y1, x2, y2);
				clicks = -1;
			}
			break;

		} else if(state == GLUT_DOWN && drawMode == "circle"){
			if(clicks == 0)
			{
				x1 = x;
				y1 = y;
			} else if(clicks == 1){
				x2 = x;
				y2 = y;
				drawCircle(x1, y1, x2, y2);
				clicks = -1;
			} 
			break;
		} else if(state == GLUT_UP && drawMode != "point"){
			clicks += 1;
			glFlush(); // draw to screen on left button release
		}

	break;		
	}
}


void motion(int x,int y)
{
	if(drawMode == "point")
	{
		drawPoint(x,y);
	}
}


void keyboard(unsigned char key, int x, int y)
{	
	int num; // for random colour

	switch(key)
	{
		case 27: //escape key with fall through
		case 'q':
			exit(0);
			break;

		case '1': // white
			colour = colourArr[0];
			break;

		case '2': // black
			colour = colourArr[1];
			break;	

		case '3': // red
			colour = colourArr[2]; 
			break;

		case '4': // orange
			colour = colourArr[3];
			break;

		case '5': // yellow
			colour = colourArr[4];
			break;

		case '6': // green
			colour = colourArr[5];
			break;

		case '7':	// blue
			colour = colourArr[6];
			break;
		
		case '8':	// purple
			colour = colourArr[7];
			break;

		case '9': // random colour
			num = rand() % 8;
			colour = colourArr[num];
			break;

		case 'p':
			drawMode = "point";
			break;

		case 'l':
			drawMode = "line";
			clicks = 0;
			break;

		case 'r':
			drawMode = "rectangle";
			clicks = 0;
			break;

		case 'c':
			drawMode = "circle";
			clicks = 0;
			break;

		default:
			break;
	}
}


void mainMenu(int value)
{
	switch(value)
	{
		case 0: // clear screen
			displayBackground();
			break;

		case 1: 
			exit(0);
			break;
	}
}


void colourSubMenu(int value)
{
	switch(value){
		case 0: // white
			colour = colourArr[0];
			break;

		case 1: // black
			colour = colourArr[1];
			break;	

		case 2: // red
			colour = colourArr[2]; 
			break;

		case 3: // orange
			colour = colourArr[3];
			break;

		case 4: // yellow
			colour = colourArr[4];
			break;

		case 5: // green
			colour = colourArr[5];
			break;

		case 6:	// blue
			colour = colourArr[6];
			break;
		
		case 7:	// purple
			colour = colourArr[7];
			break;

		case 8: // random colour
			int num = rand() % 8;
			colour = colourArr[num];
			break;
	}
}


void shapeSubMenu(int value)
{
	switch(value){
		case 0: 
			drawMode = "point";
			break;

		case 1: 
			drawMode = "line";
			clicks = 0;
			break;

		case 2: 
			drawMode = "rectangle";
			clicks = 0;
			break;

		case 3:	
			drawMode = "circle";
			clicks = 0;
			break;
	}
}

// for menu on right click
void initMenu()
{
	int mainMenuId = glutCreateMenu(mainMenu);
	int colourSubMenuId = glutCreateMenu(colourSubMenu);
	int shapeSubMenuId = glutCreateMenu(shapeSubMenu);

	// add colour sub-menu options
	glutSetMenu(colourSubMenuId);
	glutAddMenuEntry("White", 0);
	glutAddMenuEntry("Black", 1);
	glutAddMenuEntry("Red", 2);
	glutAddMenuEntry("Orange", 3);
	glutAddMenuEntry("Yellow", 4);
	glutAddMenuEntry("Green", 5);
	glutAddMenuEntry("Blue", 6);
	glutAddMenuEntry("Purple", 7);
	glutAddMenuEntry("Random", 8);

	// add colour sub-menu options
	glutSetMenu(shapeSubMenuId);
	glutAddMenuEntry("Point", 0);
	glutAddMenuEntry("Line", 1);
	glutAddMenuEntry("Rectangle", 2);
	glutAddMenuEntry("Circle", 3);

	// add main menu options
	glutSetMenu(mainMenuId);
	glutAddSubMenu("Colour", colourSubMenuId);
	glutAddSubMenu("Shapes", shapeSubMenuId);
	glutAddMenuEntry("Clear", 0);
	glutAddMenuEntry("Quit", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void glutCallbacks()
{
	glutDisplayFunc(displayBackground);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
}


int main(int argc, char** argv)
{	// print intro/controls
	printf("\n\n....................................................\n");
	printf("\t\tWelcome to SimpPaint!\n\t\tBy: Alex Guerrero\n");
	printf("....................................................\n");
	printf("\t\t~~CONTROLS~~\nMouse:\n- left click to draw\n");
	printf("--> 1 click for points\n--> 2 clicks for lines, ");
	printf("rectangles and cirlces\n");
	printf("- right click to change settings\n\nKeyboard:\n\t");
	printf("Colour\n\t1 -> white\t4 -> black\t7 -> red\n\t");
	printf("2 -> orange\t5 -> yellow\t8 -> green\n\t");
	printf("3 -> blue\t6 -> purple\t9 -> random\n\n\t");
	printf("Shapes\n\tp -> point\tr -> rectangle\n\t");
	printf("l -> line\tc -> circle \n\n\t");
	printf("Other\n\tq/esc -> quit\n\n");
	printf("Have fun creating! :D\n");

	glutInit(&argc, argv);	
	glutInitWindowSize(windowSizeX, windowSizeY);
	glutCreateWindow("Simple Paint");
	
	// register callbacks
	glutCallbacks();

	initMenu();

	// make coordinate system of window match GLUT
	gluOrtho2D(0, windowSizeX, windowSizeY, 0);

	srand (time(NULL)); // makes rand() produce "more random" numbers
						// reference: http://www.cplusplus.com/reference/cstdlib/srand/

	glutMainLoop();		//starts the event loop

	return(0);				
}