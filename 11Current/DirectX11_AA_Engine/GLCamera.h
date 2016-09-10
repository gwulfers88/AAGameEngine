#ifndef GL_CAMERA_H
#define GL_CAMERA_H
/* ========================================================================
$File: GLCamera.h $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

// TODO(George): Make this a movable camera.
#include "GLcommon.h"
#include "Transform.h"

// TODO: Make better camera
class GLCamera
{
public:
	GLCamera(vmath::vec3 pos, float fov, float aspect, float zNear, float zFar)
	{
		this->fov = fov;
		this->zNear = zNear;
		this->zFar = zFar;
		
		position = pos;
		forward = vmath::vec3(0.0f, 0.0f, 1.0f);
		up = vmath::vec3(0.0f, 1.0f, 0.0f);
		right = vmath::vec3(1.0f, 0.0f, 0.0f);

		perspective = vmath::perspective(fov, aspect, zNear, zFar);
	}

	inline void UpdateAspectRatio(float aspect)
	{
		perspective = vmath::perspective(fov, aspect, zNear, zFar);
	}

	inline vmath::vec3& GetPosition() { return position; }
	inline void SetPosition(vmath::vec3& pos) { position = pos; }

	inline vmath::mat4 ToMatrix() const
	{
		return perspective * vmath::lookat(position, position + forward, up);
	}

	~GLCamera(){}

private:
	vmath::mat4 perspective;
	vmath::vec3 position;
	vmath::vec3 target;
	vmath::vec3 forward;
	vmath::vec3 up;
	vmath::vec3 right;
	float fov;
	float zNear;
	float zFar;

};

#endif