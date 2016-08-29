#include<D3D11.h>
#include<DX11\D3DX11.h>
#include"GameObject2D.h"

GameObject2D::GameObject2D()
{
	m_position.x = m_position.y = 0.0f;
	m_rotation = 0.0f;
	m_scale.x = m_scale.y = 1.0f;
}
GameObject2D::~GameObject2D() {

}

XMMATRIX GameObject2D::GetWorldMatrix() {
	XMMATRIX translation = XMMatrixTranslation(m_position.x,m_position.y,0.0f);
	XMMATRIX rotationZ = XMMatrixRotationZ(m_rotation);
	XMMATRIX scale = XMMatrixScaling(m_scale.x,m_scale.y,1.0f);

	return translation * rotationZ * scale;
}

void GameObject2D::SetPosition(XMFLOAT2& _position) {
	m_position = _position;
}

void GameObject2D::SetRotation(float _rotation) {
	m_rotation = _rotation;
}
void GameObject2D::SetScale(XMFLOAT2& _scale) {
	m_scale = _scale;
}