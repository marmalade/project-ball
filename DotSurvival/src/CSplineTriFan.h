#pragma once

#ifndef CSPLINETRIFAN_H
#define CSPLINETRIFAN_H

#include "IwGeomFVec2.h"

typedef  CIwFVec2 Vector2;
typedef  CIwSVec2 Vector2i;

class CSpline;

class CSplineTriFan
{
public:
	CSplineTriFan(CSpline* pSpline);
	~CSplineTriFan();

	void Render();
	void Generate();
	void SetCenterPoint(const Vector2i& v) { m_vCenter = v; }
	void SetSpline(CSpline* pSpline) { m_pSpline = pSpline; }

	bool IsPointWithin(const Vector2& vPos);
private:

	CSpline*	m_pSpline;
	Vector2i*	m_apVertices;
	uint16*		m_apIndices;

	uint16		m_iSplinePointCount;
	uint16		m_iVertexCount;
	uint16		m_iIndexCount;

	Vector2i	m_vCenter;
};

#endif