#ifndef TRANSFORM_H
#define TRANSFORM_H
/* ========================================================================
$File: Transform.h $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

#include "GLcommon.h"

class Transform
{
public:
	Transform()
	{
		position = vmath::vec3(0);
		rotation = vmath::vec3(0);
		scale = vmath::vec3(1.0f);
	}

	~Transform()
	{}

	inline vmath::mat4 ToMatrix() const
	{
		vmath::mat4 transMat = vmath::translate(position);
		
		vmath::mat4 rotXMat = vmath::rotate(rotation[0], vmath::vec3(1.0f, 0.0f, 0.0f));
		vmath::mat4 rotYMat = vmath::rotate(rotation[1], vmath::vec3(0.0f, 1.0f, 0.0f));
		vmath::mat4 rotZMat = vmath::rotate(rotation[2], vmath::vec3(0.0f, 0.0f, 1.0f));
		
		vmath::mat4 rotMat = rotZMat * rotYMat * rotXMat;
		
		vmath::mat4 scaleMat = vmath::scale(scale);

		return (transMat * rotMat * scaleMat);
	};

	inline vmath::vec3& GetPosition() { return position; }
	inline vmath::vec3& GetRotation() { return rotation; }
	inline vmath::vec3& GetScale() { return scale; }

	inline void SetPosition(vmath::vec3& pos) { position = pos; }
	inline void SetRotation(vmath::vec3& rot) { rotation = rot; }
	inline void SetScale(vmath::vec3& scale) { this->scale = scale; }

private:
	vmath::vec3 position;
	vmath::vec3 rotation;
	vmath::vec3 scale;
};

#endif