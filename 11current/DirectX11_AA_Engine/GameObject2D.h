#pragma once

#include <DX11\xnamath.h>

class GameObject2D
{
public:
	GameObject2D();
	virtual ~GameObject2D();

	XMMATRIX GetWorldMatrix();

	void SetPosition(XMFLOAT2& position);
	void SetRotation(float rotation);
	void SetScale(XMFLOAT2& scale);

private:
	XMFLOAT2 m_position;
	float m_rotation;
	XMFLOAT2 m_scale;
};
