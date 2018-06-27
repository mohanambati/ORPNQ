//**********************************************************************************************************
// Written by Mohan Sai Ambati
//
// 
//
//
//**********************************************************************************************************

//**********************************************************************************************************
//			Header Files
//**********************************************************************************************************
#include "glut.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
//**********************************************************************************************************
//			Global Variables
//**********************************************************************************************************
#define MAXSEGS 100		/* max # of line segments */
#define IWINWIDTH 1000		/* initial window width */
#define IWINHEIGHT 700		/* initial window height */
#define SELDIST 10		/* max distance of mouse from selected line */

struct Node {
	int NodeID;
    float x, y;			/* line endpoints */
    int selected;			/* non-zero if line is selected */
	int printable;     // 0 if the node is deleted
	int route;
	int malicious;
} node[36];

	const float DEG2RAD = 3.14159/180; // use to draw circle at a given point.
	float pointx[6] = {10, 50, 100, 200, 250, 300};  // x co-ordinate of the point.
	float pointy[6] = {10, 50, 100, 150, 200, 250};	 // y co-ordinate of the point.
	float minx = -20.0, maxx = 320.0, miny = -20.0, maxy = 270.0; // area that displays on screen.
	float xpos = 100, ypos = 100; // position of the windown on the screen.
	//example output
	int routex[20] ={0,1,2,3,4,5,5,5};
	int routey[20] ={0,0,0,3,3,3,4,5};
	int routing_policy[36];
	
	int mousex, mousey;			/* mouse position, window relative */
	double pmousex,pmousey;
	int winWidth = IWINWIDTH;		/* initial window size */
int winHeight = IWINHEIGHT;
int start_node;
//**********************************************************************************************************
//	This function intializes glut parameters
//**********************************************************************************************************
	
void init2D(float r, float g, float b)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(xpos,ypos);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow ("6*6 Grid Network");
	glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
	glClearColor(r,g,b,0.0);   // sets the background color
	glMatrixMode (GL_PROJECTION); // sets the projection
	glLoadIdentity();
	gluOrtho2D (minx, maxx, miny, maxy); // zooms the given dimensions
}

//**********************************************************************************************************
//	This function draw a circle at a specified point.
//**********************************************************************************************************

void drawwCircle(float x, float y, float radius, int count)
{
   glBegin(GL_TRIANGLE_FAN); //draw circular proints
   for (int i=0; i < 360; i++)
   {
	   float degInRad = i*DEG2RAD;
	   glVertex2f(x + cos(degInRad)*radius, y + sin(degInRad)*radius);
   }
   glEnd();
   
   glColor3f( 0, 0, 1 );

	glRasterPos2f(x-10, y);
	char buffer[20] ={'\0'};
	sprintf(buffer,"%d",(int)  count);
    for (int i=0; i < 10; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);
    }

}

//**********************************************************************************************************
//	This function is responsible for what ever it displays on the screen.
//**********************************************************************************************************





void Loadnodes()
{int count =0, a= 0;
	for(int i = 0; i < 36; i++)
	{node[i].selected = 0;
	node[i].printable = 1;
	node[i].route = 0;
	node[i].NodeID = a;
	node[i].malicious = 0;
	a++;
	}
	
		for(int i =0; i < 6; i++)
			for(int j =0; j < 6; j++)
			{
				
				node[count].x = pointx[j];
				node[count].y = pointy[i];
				count++;
			}
}



void draw(void)
{
	int count = 0;

	/*for(int i = 0; i < 36; i++)
		{
			printf("node %d(%f,%f) \t %d \t %d\n",i,node[i].x,node[i].y, node[i].selected, node[i].printable);
		}*/
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	// draw circular points
	for(int i = 0; (i < 36); i++)
		{
			if(node[i].printable){
			
			if(node[i].selected)
			glColor3f(1.0, 0.0, 1.0);
		else
		{
			if(node[i].route) glColor3f(0.0, 1.0, 0.0);
			else 
				{if(node[i].malicious) glColor3f(1.0, 0.0, 0.0);
					else
					glColor3f(0.0, 0.0, 0.0);}
		}
			drawwCircle(node[i].x,node[i].y,3, node[i].NodeID);
			count++;}
		}
	
	//draw lines between points
	glBegin(GL_LINES);

	
	for(int i =0; (i < 35 ); i++)
	{
		if(node[i].printable){
			glColor3f(0.0, 0.0, 0.0);
		
		if((i != 5 && i != 11 && i != 17 && i != 23 && i != 29 && i != 35 && i != 8 && i != 14 && i != 26 && i != 24 && i != 28) )
		{
		if(node[i+1].printable){
		glVertex2i(node[i].x,node[i].y);
		glVertex2i(node[i+1].x,node[i+1].y);}
				
					if((i+7 < 36) && node[i+7].printable)
		{
		
		glVertex2i(node[i].x,node[i].y);
		glVertex2i(node[i+7].x,node[i+7].y);
				
		}
		
		}
		
		if((i+6 < 36) && node[i+6].printable)
		{
		
		glVertex2i(node[i].x,node[i].y);
		glVertex2i(node[i+6].x,node[i+6].y);
				
		}
		
	
		}
	}

	glEnd();

	//printroute();
	
	glFlush();	
}

//**********************************************************************************************************
//	This function paints the output route in diffrent colour.
//**********************************************************************************************************


int set_select(void)
{
    int i, j;
    double num, den;	/* u's numerator, denominator */
    double u;		/* line parameter */
    double x, y;	/* intersection point, projection coordinates */
    double dx, dy;	/* delta x, y for mouse to line in pixels */
    double dsq;		/* squared dist, mouse to line, in pixels */
    int select_change;	/* did any segment's selection flag change? */
    int done;
    
	
    select_change = 0;			/* assume nothing was changed */
	for(i=0;i<36;i++)
		node[i].selected = 0;

    glutPostRedisplay();
    
	for(i=0;i<36;i++) {
		done = 0;

		
	 //x = minx + node[i].x * (double)(maxx - minx) / winWidth;
     //y = miny + (winHeight - node[i].y) * (double)(maxy - miny) / winHeight;
	
	    dx = (double)(pmousex - node[i].x) * winWidth / (maxx - minx);
	    dy = (double)(pmousey - node[i].y) * winHeight / (maxy - miny);
	    dsq = dx * dx + dy * dy;
		//printf("mouse: %f,%f \n", pmousex, pmousey);
		//printf("node: %f,%f \n", node[i].x, node[i].y);
		//printf("diff: %f \n",dsq);
	    if (dsq <= SELDIST * SELDIST) {
			select_change = 1;
			node[i].selected = 1;	/* selected the node. */
		break;
	    }
		
	}
	return select_change;
}

void delete_node(void)
{
	for(int i = 0 ; i < 36; i++)
	{
		if(node[i].selected)
		{
			node[i].printable = 0;

		}
	}
}

void refresh(void)
{
	Loadnodes();
}

void run(void)
{
	//int k =0;
	//while(k < 35){
	for(int i = 0; i < 36; i ++)node[i].route = 0;
	//printf("Hi from run method");
	
	int states[36];
	
	//find the list of nodes present in the n/w.
	for(int i =0; (i < 36 ); i++)
	{
		if(node[i].printable){states[i]=i;}
		else{states[i] = -1;}
		//printf("%d : %d \n",states[i]);
	}

	FILE *fp;
		fp = fopen("nodes.in", "w");
    if (!fp) {
        printf("Error: File not present: %s\n");
		return;
		}	
    else
    for(int i=0;i<36;i++)
    fprintf(fp,"%d\n",states[i]);
    
    fclose(fp);
    printf("before valit\n");
	  int ret = system("valuit");
	printf("After Val\n");
	
	int k=0;
	int malval = -1;

	fp = fopen("malstates.in", "r");
			if (!fp) {
				printf("Error: File not present: %s\n");
				return;
				}
		while(!feof(fp)){		
		fscanf(fp, "%d \n", &malval);
		if(malval > 0) node[malval].malicious = 1;

		}
		fclose(fp);

printf("After setting malicious\n");

	fp = fopen("policy.in", "r");
		if (!fp) {
			printf("Error: File not present: %s\n");
			return;
			}
	while(!feof(fp) && k <=35){		
	fscanf(fp, "%d \n", &routing_policy[k]);
	k++;
	}
	fclose(fp);
	
	//for(int i =0; i <= 35; i ++) printf("routing policy: %d \n", routing_policy[i]);
	
	start_node = 0;
	while(start_node != 35)
	{
		if(node[start_node].printable){node[start_node].route = 1;
			//printf("node: %d \n", start_node);
			start_node = routing_policy[start_node];
			}
		else{
				for(int i = 0; i < 36; i ++)node[i].route = 0;
				break;
		}
	
	}
	if(start_node == 35){node[start_node].route = 35;}

	//calulate the q-values
	//compute the route
	//redisplay the screen with the route.
	//k++;
	//}
}


/*---------------------------------*/
/* Handle "normal" keyboard input. */
/* We only deal with q/Q here.     */
/*---------------------------------*/
void keyboardfun(unsigned char key, int x, int y)
{
    int i;

    switch(key) {
	case 'r':
	case 'R':
		refresh();
		glutPostRedisplay();
		break;
	case 'd':
	case 'D':
	  delete_node();
	  glutPostRedisplay();
	case 'c':
	case 'C':
		run();
		glutPostRedisplay();
		break;

    }
}


/*-----------------------------------------------------*/
/* GLUT Passive Mouse Motion Function                  */
/* This function is called when the mouse moves within */
/* the window while NO mouse buttons are pressed.      */
/*-----------------------------------------------------*/
void MouseDrag(int x, int y)
{
    mousex = x;			/* save position, for possible later use */
    mousey = y;

	pmousex = minx + x * (double)(maxx - minx) / winWidth;
    pmousey = miny + (winHeight - y) * (double)(maxy - miny) / winHeight;
	
     if (set_select()){
		 printf("selected!!");
		glutPostRedisplay();
	 }
}


//**********************************************************************************************************
//	Main function controlles the program.
//**********************************************************************************************************

void main(int argc,char *argv[])
{
	Loadnodes();
	glutInit(&argc,argv);

	init2D(1.0,1.0,1.0);
	glutDisplayFunc(draw);
	glutPassiveMotionFunc(MouseDrag);	/* passive mouse motion */
	     glutKeyboardFunc(keyboardfun);		/* "normal" keyboard entry */
	

	glutMainLoop();
}

//**********************************************************************************************************
//	                                          End of the program.
//**********************************************************************************************************