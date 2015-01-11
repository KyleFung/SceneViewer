/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h"

using namespace std; 
#include "variables.h"
#include "readfile.h"

// New helper transformation function to transform vector by modelview 
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.  
// Some of you may want to use the more modern routines in readfile.cpp 
// that can also be used.  
void transformvec (const GLfloat input[4], GLfloat output[4]) 
{
  GLfloat modelview[16]; // in column major order
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview); 

  for (int i = 0 ; i < 4 ; i++) {
    output[i] = 0; 
    for (int j = 0 ; j < 4 ; j++) {
      output[i] += modelview[4*j+i] * input[j]; 
    }
  }
}

void display() 
{
  glClearColor(0.5, 0.5, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // I'm including the basic matrix setup for model view to 
  // give some sense of how this works.  

  glMatrixMode(GL_MODELVIEW);
  mat4 mv; 

  // Either use the built-in lookAt function or the lookAt implemented by the user.
  if (useGlu) {
    mv = Transform::lookAt(eye,center,up);
  } else {
    //mv = Transform::lookAt(eye,center,up); 
	mv = glm::lookAt(eye,center,up); 
  }

  glLoadMatrixf(&mv[0][0]); 

  if (numused) {
    glUniform1i(enablelighting,true);	
    glUniform1i(numusedcol, numused);
	//GLint myUniformLocation = glGetUniformLocation(shaderprogram, "numused");
	//glUniform1i(myUniformLocation, numused);

	for (int i = 0; i < numused; i++)
	{
		vec4 lightVector = vec4(lightposn[0 + 4*i], lightposn[1 + 4*i], lightposn[2 + 4*i], lightposn[3 + 4*i]);
		lightVector = mv * lightVector;
		GLfloat passedPosnVector[4] = {lightVector.x, lightVector.y, lightVector.z, lightVector.w};

		vec4 colorVector = vec4(lightcolor[0 + 4*i], lightcolor[1 + 4*i], lightcolor[2 + 4*i], lightcolor[3 + 4*i]);
		GLfloat passedColorVector[4] = {colorVector.x, colorVector.y, colorVector.z, colorVector.w};

		GLuint lightpospost = lightpos + i;
		GLuint lightcolpost = lightcol + i;

		glUniform4fv(lightpospost, 1, passedPosnVector);
	    glUniform4fv(lightcolpost, 1, passedColorVector); 
	}
  } 
  else 
  {
    glUniform1i(enablelighting,false); 
  }

  // Transformations for objects, involving translation and scaling 
  mat4 sc(1.0) , tr(1.0), transf(1.0); 
  sc = Transform::scale(sx,sy,1.0); 
  tr = Transform::translate(tx,ty,0.0); 

  transf = tr * sc * mv;

  glLoadMatrixf(&transf[0][0]); 

  for (int i = 0 ; i < numobjects ; i++) {
    object* obj = &(objects[i]);
	mat4 transfOld = transf;
	transf = transf * obj->transform;
	glLoadMatrixf(&transf[0][0]); 
	transf = transfOld;

    glUniform4fv(ambientcol, 1, obj->ambient); 
	glUniform4fv(diffusecol, 1, obj->diffuse); 
	glUniform4fv(specularcol, 1, obj->specular); 
	glUniform4fv(emissioncol, 1, obj->emission); 
	glUniform1f(shininesscol, obj->shininess); 

    if (obj->type == cube) {
      glutSolidCube(obj->size); 
    }
    else if (obj->type == sphere) {
      const int tessel = 20; 
      glutSolidSphere(obj->size, tessel, tessel); 
    }
    else if (obj->type == teapot) {
      glutSolidTeapot(obj->size); 
    }

  }

  glutSwapBuffers();
}
