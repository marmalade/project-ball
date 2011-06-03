#pragma once

#ifndef CSPLINEANIMATOR_H
#define CSPLINEANIMATOR_H

#include "IwList.h"

class CSpline;
class TiXmlDocument;
class TiXmlElement;

namespace Consts
{
	static const float kfSpeed = 1.0f;
}

class CKeyFrameNode : public CIwListNode
{
public:
	CKeyFrameNode(float f, CSpline* p) { fProgress = f; pSpline = p; }
	~CKeyFrameNode();
public:
	float fProgress;
	CSpline* pSpline;
};


class CSplineAnimator
{
private:
	static const int kSplineSegments = 16; 
	static const int kSplinePoints = 100;
	static const int kKFCount = 17;
public:
	CSplineAnimator();
	~CSplineAnimator();

	void Init();
	void Update(float fdt);
	void Reset();

	CSpline* GetCurrentSpline() { return m_pCurrentSpline; }
	
private:

	void ReadSVG(const char* strFile);

	CSpline*	m_pSplines[kKFCount];
	
	CIwList<CKeyFrameNode*>* m_apKFList;
	CKeyFrameNode* m_pKFRoot;
	CKeyFrameNode* m_pCurKF;
	CKeyFrameNode* m_pNextKF;

	CSpline*			m_pCurrentSpline;

	float	m_fKFProgress;
	float	m_fProgress;

	TiXmlDocument* m_txDoc;
	TiXmlElement* m_txSplineElem;

};


#endif