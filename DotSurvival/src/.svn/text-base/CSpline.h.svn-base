#pragma once

#ifndef CSPLINE_H
#define CSPLINE_H

#include "IwGeomFVec2.h"
#include "IwGeomSVec2.h"

typedef  CIwFVec2 Vector2;
typedef  CIwSVec2 Vector2i;

class CSpline
{
public:
	struct SSegment
	{
		Vector2 p0;
		Vector2 p0c;
		Vector2 p1;
		Vector2 p1c;

		SSegment() { p0 = Vector2(0.0f, 0.0f); p0c = Vector2(0.0f, 0.0f); p1c = Vector2(0.0f, 0.0f); p1 = Vector2(0.0f, 0.0f);};
		SSegment(Vector2& ap0, Vector2& ap0c, Vector2& ap1c, Vector2& ap1) { p0 = ap0; p0c = ap0c; p1 = ap1; p1c = ap1c; };

		SSegment operator-(const SSegment& right) const
		{
			SSegment result;
			result.p0 = p0 - right.p0;
			result.p0c = p0c - right.p0c;			
			result.p1c = p1c - right.p1c;
			result.p1 = p1 - right.p1;
			return result;
		}

		SSegment operator+(const SSegment& right) const
		{
			SSegment result;
			result.p0 = p0 + right.p0;
			result.p0c = p0c + right.p0c;			
			result.p1c = p1c + right.p1c;
			result.p1 = p1 + right.p1;
			return result;
		}

		SSegment operator*(const float right)
		{
			SSegment result;
			result.p0 = Vector2((p0.x * right), (p0.y * right));
			result.p0c = Vector2((p0c.x * right), (p0c.y * right));			
			result.p1c = Vector2((p1c.x * right), (p1c.y * right));
			result.p1 = Vector2((p1.x * right), (p1.y * right));
			return result;
		}
	};

	CSpline(uint16 iSegments, uint16 iNumPoints);
	~CSpline();

	void SetSegment(uint32 iNr, const SSegment& sSeg);
	const SSegment& GetSegment(int i) { return m_Segments[i]; };
	void Generate();

	Vector2i*	GetPointsI() { return m_PointsI;}
	Vector2*	GetPoints() { return m_Points;}

	uint16		GetPointCount() { return m_iNumPoints; };

	void		lerp(CSpline* pNextSpline, float k);

	void		SetSplineDiff(CSpline* pSpline1, CSpline* pSpline2, float fProgress);

private:

	SSegment*	m_Segments;
	Vector2*	m_Points;
	Vector2i*	m_PointsI;

	uint16		m_iNumSegments;
	uint16		m_iNumPoints;

	float		m_fStep;
};

#endif

