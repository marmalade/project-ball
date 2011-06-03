#include "CSpline.h"
#include "math.h"
#include "Src/Utils.h"


CSpline::CSpline(uint16 iNumSegments, uint16 iNumPoints)
{
	m_iNumSegments = iNumSegments;
	m_iNumPoints = iNumPoints;

	m_Segments = new SSegment[iNumSegments];
	m_Points = new Vector2[iNumPoints];
	m_PointsI = new Vector2i[iNumPoints];

	m_fStep = (float) (iNumSegments) / iNumPoints;
}

CSpline::~CSpline()
{
	delete [] m_Segments;
	delete [] m_Points;
	delete [] m_PointsI;
}

void CSpline::SetSegment(uint32 iNr, const SSegment& sSeg)
{
	IwAssert(0, iNr < m_iNumSegments);

	m_Segments[iNr] = sSeg;
}

void CSpline::Generate()
{
	uint32 iPointNum = 0;
	for (float fProgress = 0.0f; fProgress <= (float)m_iNumSegments; fProgress += m_fStep)
	{
		int iSegment = (int)fProgress;
		float t = fmodf(fProgress, 1.0f);
		float t_sq = t*t;
		float one_minus_t = 1.0f - t;
		float one_minut_t_sq = one_minus_t*one_minus_t;

		m_Points[iPointNum].x =	one_minut_t_sq * one_minus_t * m_Segments[iSegment].p0.x + 
								3 * one_minut_t_sq * t * m_Segments[iSegment].p0c.x + 
								3 * one_minus_t * t_sq * m_Segments[iSegment].p1c.x + 
								t_sq * t * m_Segments[iSegment].p1.x;
		m_Points[iPointNum].y =	one_minut_t_sq * one_minus_t * m_Segments[iSegment].p0.y + 
								3 * one_minut_t_sq * t * m_Segments[iSegment].p0c.y + 
								3 * one_minus_t * t_sq * m_Segments[iSegment].p1c.y + 
								t_sq * t * m_Segments[iSegment].p1.y;
		m_PointsI[iPointNum].x = (int16) m_Points[iPointNum].x;
		m_PointsI[iPointNum].y = (int16) m_Points[iPointNum].y;

		iPointNum++;
		if (iPointNum >= m_iNumPoints)
			break;
	}
}

void CSpline::lerp(CSpline* pNextSpline, float k)
{
	for (int i = 0; i < m_iNumSegments; i++)
	{
		SSegment sSeg1 = pNextSpline->GetSegment(i);
		SSegment sDiff = sSeg1 - m_Segments[i];
		SSegment sMul = sDiff * k;
		m_Segments[i] = m_Segments[i] + sMul;
	}
}

void CSpline::SetSplineDiff(CSpline* pSpline1, CSpline* pSpline2, float fProgress)
{
	for (int i = 0; i < m_iNumSegments; i++)
	{
		m_Segments[i] = pSpline1->GetSegment(i) + (pSpline2->GetSegment(i) - pSpline1->GetSegment(i)) * fProgress;//sinf(fProgress * half_pi);
	}
}



