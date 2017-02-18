#include <GL/glut.h>
#include <iostream>
#include <math.h>

#define PHI 1.61803398874989484820
#define PI  3.14159265358979323846
#define SCALA 1.5f

int width=768; 
int height=768;
int iterations=2000;

GLdouble minX = -1.5f, maxX = 1.5f, minY = -1.5f, maxY = 1.5f; // complex plane boundaries

double S1=pow(0.5, 0.5);;
double S2=pow(1./PHI, 1./PHI);
int N=16;
double nullx[2]={0.0,0.0};

void ricorsione(double* x, int n, double t);

void trasformazione1(double* x, int n, double t) {
	double punto[2];
	punto[0]= (S1-(S1-S2)*t)*cos(PI/4.-(PI/4.-32.893818*PI/180.+2*PI)*t)*x[0]-(S1-(S1-S2)*t)*sin(PI/4.-(PI/4.-32.893818*PI/180.+2*PI)*t)*x[1]; 
	punto[1]= (S1-(S1-S2)*t)*sin(PI/4.-(PI/4.-32.893818*PI/180.+2*PI)*t)*x[0]+(S1-(S1-S2)*t)*cos(PI/4.-(PI/4.-32.893818*PI/180.+2*PI)*t)*x[1];
		glColor3ub(255,0,0);
		glVertex2f((punto[0]-0.6f)/SCALA,(punto[1]-0.4f)/SCALA);
	ricorsione(punto,n,t);  
}

void trasformazione2(double* x, int n, double t) {
	double punto[2];
	punto[0]= (S1-(S1-S2*S2)*t)*cos(3*PI/4.-(3*PI/4.-133.0140178*PI/180.+2*PI)*t)*x[0]-(S1-(S1-S2*S2)*t)*sin(-PI/4.-(-PI/4.-133.0140178*PI/180.+2*PI)*t)*x[1]+1; 
	punto[1]= (S1-(S1-S2*S2)*t)*sin(3*PI/4.-(3*PI/4.-133.0140178*PI/180.+2*PI)*t)*x[0]+(S1-(S1-S2*S2)*t)*cos(-PI/4.-(-PI/4.-133.0140178*PI/180.+2*PI)*t)*x[1]; 
		glColor3ub(0,0,255);
		glVertex2f((punto[0]-0.6f)/SCALA,(punto[1]-0.4f)/SCALA);
	ricorsione(punto,n,t);  
}


void ricorsione(double* x, int n, double t){
	if(n<N) trasformazione1(x,n+1,t);
	if(n<N) trasformazione2(x,n+1,t); 
}

int buu=1;

void displayMe(void){
	if(buu){ buu=0;
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_POINTS);
			ricorsione(nullx,0,0);
		glEnd();
		glFlush();
	}
}	


static void		mouseCB(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			for(int i=0; i<iterations+1; i++) {
				glClear(GL_COLOR_BUFFER_BIT);
				glBegin(GL_POINTS);
					ricorsione(nullx,0,i/(float)iterations);
				glEnd();
				glFlush();
			}
		}
	}
	else if (state == GLUT_UP) {
		//glutPostRedisplay();		
    }
	
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
				glClear(GL_COLOR_BUFFER_BIT);
				glBegin(GL_POINTS);
					ricorsione(nullx,0,0);
				glEnd();
				glFlush();
		}
	}
	else if (state == GLUT_UP) {
		//glutPostRedisplay();		
    }
	
}

static void		keyCB(unsigned char c, int x, int y){
	
	if (c == 27) {
		exit(0);
	}
	glutPostRedisplay();
}



int	main(int argc, char** argv)
{
  /*
   * initialize GLUT and open a window
   */
  glutInit(&argc, argv);
  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_SINGLE);
  glutCreateWindow("Dragon curves");
  
    glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(minX, maxX, minY, maxY, (GLdouble)(-1), (GLdouble)1);
	
	
  //glutDisplayFunc(redrawCB);
  //glutReshapeFunc(reshapeCB);
  glutMouseFunc(mouseCB);
  //glutMotionFunc(motionCB);
  glutKeyboardFunc(keyCB);
  glutDisplayFunc(displayMe);
  
  
  
  
  glutMainLoop();
  return 0;
}