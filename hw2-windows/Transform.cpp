#include <iostream>
#include "Transform.h"
using namespace std ; 
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    mat3 ret;

	float theta = degrees * 0.0174532925;
	
	vec3 refx = glm::normalize(axis);
	
	vec3 refy;

	if (refx.x == 1.0)
	{
        refy = glm::cross(refx, vec3(0.0, 1.0, 0.0));
	}
	else if(refx.y ==1.0)
	{
		refy = glm::cross(refx, vec3(0.0, 0.0, 1.0));
	}
	else 
	{
		refy = glm::cross(refx, vec3(0.0, 0.0, 1.0));
	}

	vec3 refz = glm::cross(refx, refy);

	mat3 Frame = glm::mat3(refx.x, refy.x, refz.x, refx.y, refy.y, refz.y, refx.z, refy.z, refz.z);

	mat3 rotation = glm::mat3(1.0, 0.0, 0.0, 0.0, cos(theta), sin(theta), 0.0, (-1.0)*sin(theta), cos(theta));

	mat3 invFrame = glm::inverse(Frame);

	ret = invFrame * rotation * Frame;

    return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
	mat3 rotation = rotate(degrees, up);
	eye = rotation * eye;
	up = rotation * up;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
	vec3 axis = glm::cross(eye, up);
	mat3 rotation = rotate(degrees, axis);
	eye = rotation * eye;
	up = rotation * up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1. 
	ret = glm::lookAt(eye, center, up);
    return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.  
	ret = glm::perspective(fovy, aspect, zNear, zFar);
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret;
    ret = mat4(sx, 0.0, 0.0, 0.0, 0.0, sy, 0.0, 0.0, 0.0, 0.0, sz, 0.0, 0.0, 0.0, 0.0, 1.0);
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret;
    ret = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, tx, ty, tz, 1.0);
    return ret;
}

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
