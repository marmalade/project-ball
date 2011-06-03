#pragma once

#ifndef CBALL_H
#define CBALL_H

#include "Utils.h"

class CBall
{
public:
	CBall();
	~CBall() {}

	void Render();

	void SetAcceleration(Vector2i v) { m_vAcc = v; }
	void Update(float dt);

	void SetPos(const Vector2& v) { m_vPos = v; }
	const Vector2& GetPos() const { return m_vPos; }
private:

	Vector2i	m_avVertices[4];
	uint16		m_aiIndices[4];

	Vector2		m_vPos;
	Vector2i	m_vAcc;
};

#endif
