#ifndef GL_COMMON_H
#define GL_COMMON_H
/* ========================================================================
$File: GLCommon.h $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

// OPENGL STUFF
#include <GL/glew.h>
#include <GL/wglext.h>
#include <vmath.h>

#define RED		vmath::vec3(1.0f, 0.0f, 0.0f)
#define GREEN	vmath::vec3(0.0f, 1.0f, 0.0f)
#define BLUE	vmath::vec3(0.0f, 0.0f, 1.0f)
#define YELLOW	vmath::vec3(0.5f, 0.5f, 0.0f)
#define CYAN	vmath::vec3(0.0f, 0.5f, 0.5f)
#define PURPLE	vmath::vec3(0.5f, 0.0f, 0.5f)

#endif