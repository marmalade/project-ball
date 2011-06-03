#pragma once

#ifndef CSPLINEPOLY2TRI_H
#define CSPLINEPOLY2TRI_H

#include "IwGeomFVec2.h"
#include "src/poly2tri_mod/common/shapes.h"

typedef  CIwFVec2 Vector2;
typedef  CIwSVec2 Vector2i;
typedef  CIwSVec3 Vector3i;

class CSpline;

namespace p2t
{
	class CDT;
}

class CSplinePoly2Tri
{
public:
	CSplinePoly2Tri(CSpline* pSpline);
	~CSplinePoly2Tri();

	void Render();
	void Generate();
	void SetSpline(CSpline* pSpline) { m_pSpline = pSpline; }

	bool IsPointWithin(const Vector2& vPos);
private:

	CSpline*	m_pSpline;
	Vector3i*	m_apVertices;
	uint16*		m_apIndices;
	uint16		m_iIndexCount;
	//uint16*		m_apIndices;

	//uint16		m_iSplinePointCount;
	//uint16		m_iVertexCount;
	//uint16		m_iIndexCount;

	// constrained delaunay triangulation
	p2t::CDT*	m_cdt;
};


#endif