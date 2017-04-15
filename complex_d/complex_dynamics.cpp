#include <GL/glew.h> 
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "plot.h"
#include "LoadShaders.h" 

const int width=640, height=360; // window size

GLuint	  program;
GLuint	  vao;

struct
{
	GLint   zoom;
	GLint   offset;
	GLint   Resolution;
	GLint	GlobalTime;
} uniforms;

float zoom = 0.2f;
float x_offset = -0.18f;
float y_offset = -0.1f;
float timestop = 0.0f;
bool  animation = true;


void startup()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "complex_vertex.txt"},
		{GL_FRAGMENT_SHADER, "complex_frag.txt"},
        {GL_NONE, NULL}
    };
	
	program = LoadShaders(shaders);

	uniforms.zoom   = glGetUniformLocation(program, "zoom");
	uniforms.offset = glGetUniformLocation(program, "offset");
	uniforms.Resolution = glGetUniformLocation(program, "Resolution");
	uniforms.GlobalTime = glGetUniformLocation(program, "GlobalTime");
}

int count = 0;
int tim = 0;

void screenshot(int);
void Draw() 
{
	static const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, color);

    static float t = 0.0f;
    int Time = glutGet(GLUT_ELAPSED_TIME);
 
	float offset[2] = { x_offset, y_offset };
	float Resolution[2] = { width, height };
	float GlobalTime = animation? tim/1000.0f/*Time/1000.0f*/ : timestop;

    glUseProgram(program);

    glUniform1f(uniforms.GlobalTime, GlobalTime);
	glUniform2fv(uniforms.Resolution, 1, Resolution);
    glUniform2fv(uniforms.offset, 1, offset);
    glUniform1f(uniforms.zoom, zoom);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	glutSwapBuffers();
	//glutPostRedisplay();
}

void Drawt(int Time) 
{
	static const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, color);

    static float t = 0.0f;

	float offset[2] = { x_offset, y_offset };
	float Resolution[2] = { width, height };
	float GlobalTime = animation? Time/1000.0f : timestop;

    glUseProgram(program);

    glUniform1f(uniforms.GlobalTime, GlobalTime);
	glUniform2fv(uniforms.Resolution, 1, Resolution);
    glUniform2fv(uniforms.offset, 1, offset);
    glUniform1f(uniforms.zoom, zoom);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	glutSwapBuffers();
	//glutPostRedisplay();
}

void init()
{    
    if (glewInit()) { // checks if glewInit() is activated
        std::cout << "Unable to initialize GLEW." << std::endl;
    }
	
	std::cout<<"glewinit_done"<<std::endl<<std::flush;

	startup();
	std::cout<<"startup_done"<<std::endl<<std::flush;
}

void reshape (int w, int h)
{ 
	glViewport (0, 0, (GLsizei)w, (GLsizei)h); // set new dimension of viewable screen
	glutPostRedisplay(); // repaint the window
}

void screenshot(int i = 0)
{
	npy_intp nn = 3*width*height;
	float *data = new float[nn];

	if( data ) glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, data);

	PyObject *a = PyArray_SimpleNewFromData(1,&nn,NPY_FLOAT,(void *)data);

	PyObject* shape = PyTuple_New(3);
	PyTuple_SetItem(shape, 0, PyLong_FromSize_t(height));
	PyTuple_SetItem(shape, 1, PyLong_FromSize_t(width));
	PyTuple_SetItem(shape, 2, PyLong_FromSize_t(3));
	PyObject *img = PyArray_Reshape((PyArrayObject*)a,shape);

	plt::imsave(img,"./imgs/Complex"+std::to_string(i)+".png");
	std::cout << "screenshot " << i << " done" << std::endl;

	Py_DECREF(a);
	Py_DECREF(img);
	Py_DECREF(shape);
}

static void	keyFunction(unsigned char c, int x, int y)
{
	if(c == 27) { //ESC
		exit(0);
	}	
	
	switch (c)
	{
		case 'p':
				  screenshot(count);
			break;
		case 'o':
				  tim += 150;
			 	  count++;
			 	  glutPostRedisplay();
			break;
		case 'q': zoom *= 1.05f;
			break;
		case 'e': zoom /= 1.05f;
			break;
		case 's': y_offset -= zoom * 0.05f;
			break;
		case 'a': x_offset -= zoom * 0.05f;
			break;
		case 'w': y_offset += zoom * 0.05f;
			break;
		case 'd': x_offset += zoom * 0.05f;
			break;
		case 'r': zoom = 1.0f;
				  x_offset = 0.0f;
				  y_offset = 0.0f;
				  timestop = 0.0f;
			break;
		case 't': animation = !animation;
				  timestop  = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
			break;

		default:
			break;
	};
}

static void		mouseCB(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) 
	{
		if (state == GLUT_DOWN) 
		{
			int iterations = 200;
			for(int i=1; i<iterations+1; i++)
			{
				Drawt((i*8000.0f)/iterations);
				screenshot(i);
			}
			
		}
	}
	else if (state == GLUT_UP) {}
	
	if (button == GLUT_RIGHT_BUTTON) 
	{
		if (state == GLUT_DOWN) 
		{
			Drawt(0);
		}
	}
	else if (state == GLUT_UP) {}
	
}

int main(int argc, char** argv)
{
	plt::init();

	glutInit(&argc, argv);	// initialise glut
	glutInitWindowSize(width, height);	// set the initial window size
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Fractal"); 	// create the window
	
	init();
		
	// set the event handling methods
	// set the function to use to draw our scene
	glutDisplayFunc(Draw);
	glutReshapeFunc(reshape); 
	glutMouseFunc(mouseCB);
	glutKeyboardFunc(keyFunction);
	
	glutMainLoop(); // this function runs a while loop to keep the program running.

	Py_Finalize();

	return 0;
}