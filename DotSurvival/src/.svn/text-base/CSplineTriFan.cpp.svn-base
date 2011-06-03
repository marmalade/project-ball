#include "CSpline.h"
#include "String.h"
#include "IwGx.h"
#include "Utils.h"

#include "CSplineTriFan.h"

CSplineTriFan::CSplineTriFan(CSpline* pSpline)
{
	m_pSpline = pSpline;

	m_iSplinePointCount = m_pSpline->GetPointCount();
	m_iVertexCount = m_iSplinePointCount+1; // plus center
	m_iIndexCount = m_iVertexCount+1; // to close the gap

	m_apVertices = new Vector2i[m_iVertexCount];
	m_apIndices = new uint16[m_iIndexCount];

	m_vCenter = Vector2i(0, 0);

	for (uint16 i = 0; i < m_iVertexCount; i++)
	{
		m_apIndices[i] = i;
	}

	m_apIndices[m_iIndexCount-1] = 1;
}

CSplineTriFan::~CSplineTriFan()
{
	delete [] m_apVertices;
	delete [] m_apIndices;
}

void CSplineTriFan::Generate()
{
	m_apVertices[0] = m_vCenter;
	memcpy(&m_apVertices[1], m_pSpline->GetPointsI(), m_iSplinePointCount*sizeof(Vector2i));
}

void CSplineTriFan::Render()
{
	// Set screenspace vertex coords
	IwGxSetVertStreamScreenSpace(m_apVertices, m_iVertexCount);

	IwGxDrawPrims(IW_GX_TRI_FAN, m_apIndices, m_iIndexCount);
}

bool CSplineTriFan::IsPointWithin(const Vector2& vPos)
{
	float raypt[3] = { vPos.x, vPos.y, 0.0f };
	float rayvec[3] = { 0.0f, 0.0f, 1.0f };
	float triv0[3] = { (float)m_apVertices[0].x, (float)m_apVertices[0].y, 0.0f };
	float triv1[3], triv2[3];

	Vector2* vPoints = m_pSpline->GetPoints();

	for (int i = 1; i < m_iVertexCount; i++)
	{
		triv1[0] = vPoints[i-1].x;
		triv1[1] = vPoints[i-1].y;
		triv1[2] = 0.0f;
		triv2[0] = vPoints[i].x;
		triv2[1] = vPoints[i].y;
		triv2[2] = 0.0f;

		float u, v, t;

		int rez = intersect_triangle(raypt, rayvec, 
						   triv0, triv1, triv2, 
						   &t, &u, &v);
		if (rez)
			return true;
	}

	return false;

}
