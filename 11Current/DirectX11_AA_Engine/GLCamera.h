#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include "GLcommon.h"

class GLCamera
{
public:
	GLCamera(const vmath::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		this->fov = fov;
		this->zNear = zNear;
		this->zFar = zFar;

		position = pos;
		forward = vmath::vec3(0.0f, 0.0f, 1.0f);
		up = vmath::vec3(0.0f, 1.0f, 0.0f);

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
	vmath::vec3 forward;
	vmath::vec3 up;
	vmath::vec3 right;
	float fov;
	float zNear;
	float zFar;

};

#endif