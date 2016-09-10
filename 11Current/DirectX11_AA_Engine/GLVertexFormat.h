#ifndef GL_VERTEX_FORMAT_H
#define GL_VERTEX_FORMAT_H
/* ========================================================================
$File: GLVertexFormat.h $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

#include "GLcommon.h"

struct VERTEX_FORMAT
{
	vmath::vec3 position;
	vmath::vec2 uv;
	vmath::vec3 normals;
};

enum VERTEX_BUFFERS
{
	VB_POSITION,
	//VB_COLOR,
	VB_UV,
	VB_INDICES,
	VB_NORMALS,

	VB_SIZE,	// Place more buffers before this line!!
};

#endif