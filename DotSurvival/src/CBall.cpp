#include "CBall.h"
#include "IwGx.h"

CBall::CBall()
{
	m_vPos = Vector2(200.0f, 200.0f);
	m_vAcc = Vector2i(0, 0);

	float iSize = 10.0f;

	m_avVertices[0] = Vector2i(int16(m_vPos.x - iSize), int16(m_vPos.y - iSize));
	m_avVertices[1]	= Vector2i(int16(m_vPos.x - iSize), int16(m_vPos.y + iSize));
	m_avVertices[2]	= Vector2i(int16(m_vPos.x + iSize), int16(m_vPos.y + iSize));
	m_avVertices[3] = Vector2i(int16(m_vPos.x + iSize), int16(m_vPos.y - iSize));
	m_aiIndices[0] = 0;
	m_aiIndices[1] = 1;
	m_aiIndices[2] = 2;
	m_aiIndices[3] = 3;
}

void CBall::Update(float dt)
{
	int iSize = 10;
	m_vPos = m_vPos + Vector2(float(m_vAcc.x), float(-m_vAcc.y)) * 0.4f * dt;
	m_vPos.x = fClamp(m_vPos.x, 0, float(IwGxGetScreenWidth()));
	m_vPos.y = fClamp(m_vPos.y, 0, float(IwGxGetScreenHeight()));
	
	m_avVertices[0] = Vector2i(int16(m_vPos.x - iSize), int16(m_vPos.y - iSize));
	m_avVertices[1]	= Vector2i(int16(m_vPos.x - iSize), int16(m_vPos.y + iSize));
	m_avVertices[2]	= Vector2i(int16(m_vPos.x + iSize), int16(m_vPos.y + iSize));
	m_avVertices[3] = Vector2i(int16(m_vPos.x + iSize), int16(m_vPos.y - iSize));
}


void CBall::Render()
{
	// Set screenspace vertex coords
	IwGxSetVertStreamScreenSpace(m_avVertices, 4);
	IwGxDrawPrims(IW_GX_QUAD_LIST, m_aiIndices, 4);
}
