#include <GL/glew.h> 
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "plot.h"
#include "LoadShaders.h" 

const int width=1280, height=720; // window size

GLuint	  program;
GLuint	  vao;

struct
{
	GLint   zoom;
	GLint   offset;
	GLint   Resolution;
} uniforms;

float zoom = 0.6f;
float x_offset = 2.55f;
float y_offset = 3.4f;


void startup()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "lyapunov_vertex.txt"},
		{GL_FRAGMENT_SHADER, "lyapunov_frag.txt"},
        {GL_NONE, NULL}
    };
	
	program = LoadShaders(shaders);

	uniforms.zoom   = glGetUniformLocation(program, "zoom");
	uniforms.offset = glGetUniformLocation(program, "offset");
	uniforms.Resolution = glGetUniformLocation(program, "Resolution");
}

void Draw() 
{
	static const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, color);

    static float t = 0.0f;
 
	float offset[2] = { x_offset, y_offset };
	float Resolution[2] = { width, height };

    glUseProgram(program);


	glUniform2fv(uniforms.Resolution, 1, Resolution);
    glUniform2fv(uniforms.offset, 1, offset);
    glUniform1f(uniforms.zoom, zoom);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		//std::cout<<"drawarray_done"<<std::endl<<std::flush;		
	
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

void screenshot()
{
	plt::init();

	npy_intp nn = 4*width*height;
	float *data = new float[nn];

	if( data ) {
	    glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, data);
	}

	PyObject *a = PyArray_SimpleNewFromData(1,&nn,NPY_FLOAT,(void *)data);

	PyObject* shape = PyTuple_New(3);
	PyTuple_SetItem(shape, 0, PyLong_FromSize_t(height));
	PyTuple_SetItem(shape, 1, PyLong_FromSize_t(width));
	PyTuple_SetItem(shape, 2, PyLong_FromSize_t(4));
	PyObject *img = PyArray_Reshape((PyArrayObject*)a,shape);

	plt::imsave(img,"./Zircon_city.png");

	Py_Finalize();
}

static void	keyFunction(unsigned char c, int x, int y)
{
	if(c == 27) { //ESC
		exit(0);
	}	
	
	switch (c)
	{
		case 'p':
			screenshot();
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
		default:
			break;
	};
	
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);	// initialise glut
	glutInitWindowSize(width, height);	// set the initial window size
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Fractal"); 	// create the window
	
	init();
		
	// set the event handling methods
	// set the function to use to draw our scene
	glutDisplayFunc(Draw);
	glutReshapeFunc(reshape); 
	glutKeyboardFunc(keyFunction);
	
	glutMainLoop(); // this function runs a while loop to keep the program running.
	
	return 0;
}