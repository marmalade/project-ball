#include "CSplineAnimator.h"
#include "CSpline.h"
#include "IwGx.h"
#include "tinyxml/tinyxml.h"
#include "Utils.h"

CKeyFrameNode::~CKeyFrameNode()
{
	delete pSpline;
}

CSplineAnimator::CSplineAnimator()
{
	m_apKFList = new CIwList<CKeyFrameNode*>();

	m_pCurrentSpline = new CSpline(kSplineSegments, kSplinePoints);

	for (int i = 0; i < kKFCount; i++)
	{
		m_pSplines[i] = new CSpline(kSplineSegments, kSplinePoints);
	}

	m_fKFProgress = 0.0f;
	m_fProgress = 0.0f;

	m_txDoc = new TiXmlDocument();
	m_txSplineElem = NULL;
}

CSplineAnimator::~CSplineAnimator()
{	
	for (CIwList<CKeyFrameNode*>::iterator it = m_apKFList->begin(); it != m_apKFList->end(); it++ )
	{
		delete *it;
	}

	//m_apKFList->clear();
	delete m_apKFList;
	delete m_pCurrentSpline;
}

void CSplineAnimator::Init()
{
	ReadSVG("test.svg");

	for (int i = 0; i < kKFCount; i++)
	{
		const char* strSpline = m_txSplineElem->FirstChildElement("path")->Attribute("d");

		const int kiMaxTokenSize = 100;
		const char* strToken[kiMaxTokenSize];
		strToken[0] = strtok((char*)strSpline, " ");
		for (int j = 1; j < kiMaxTokenSize; j++)
		{
			strToken[j] = strtok(NULL, " ");
		}

		IwAssert(0, strcmp(strToken[0], "M") == 0);
		IwAssert(0, strcmp(strToken[2], "C") == 0);

		Vector2 vBase = StringToVector2(strToken[1], ",");
		Vector2 vP0 = vBase;
		Vector2 vC0 = StringToVector2(strToken[3], ",");
		Vector2 vC1 = StringToVector2(strToken[4], ",");
		Vector2 vP1 = StringToVector2(strToken[5], ",");

		m_pSplines[i]->SetSegment(0, CSpline::SSegment(vP0,vC0,vC1,vP1));

		for (int j = 1; j < kSplineSegments; j++)
		{
			vP0 = m_pSplines[i]->GetSegment(j-1).p1;
			vC0 = StringToVector2(strToken[3+j*3], ",");
			vC1 = StringToVector2(strToken[3+j*3+1], ",");
			vP1 = StringToVector2(strToken[3+j*3+2], ",");

			m_pSplines[i]->SetSegment(j, CSpline::SSegment(vP0,vC0,vC1,vP1));
		}

		// getting the keyframe time from layer name
		double dKFTime = 0;
		m_txSplineElem->Attribute("inkscape:label", &dKFTime);

		m_apKFList->append(new CKeyFrameNode((float)dKFTime, m_pSplines[i]));

		m_txSplineElem = m_txSplineElem->NextSiblingElement("g");
		if (!m_txSplineElem)
			break;
	}

	m_pKFRoot = m_apKFList->front();
	m_pKFRoot->Init();

	for (CIwList<CKeyFrameNode*>::iterator it = m_apKFList->begin(); it != m_apKFList->end(); it++ )
	{
		IwListAddNodeEnd(m_pKFRoot, *it);
	}

	m_pCurKF = m_apKFList->front();
	m_pNextKF = (CKeyFrameNode*)m_pCurKF->m_Next;

	//for (int i = 0; i < 2; i++)
	//{
	//	m_pCurrentSpline->SetSegment(i, m_pCurKF->pSpline->GetSegment(i));
	//}

	delete m_txDoc;
}

void CSplineAnimator::ReadSVG(const char* strFile)
{
	m_txDoc->LoadFile(strFile);
	TiXmlNode* txRoot = m_txDoc->RootElement();
	m_txSplineElem = txRoot->FirstChildElement()->NextSiblingElement("g")->NextSiblingElement("g");
}

void CSplineAnimator::Update(float dt)
{
	m_fKFProgress += dt * Consts::kfSpeed;
	m_fProgress += dt * Consts::kfSpeed;

	float fCurKFDuration = m_pNextKF->fProgress - m_pCurKF->fProgress;
	if (m_fKFProgress >= fCurKFDuration)
	{
		m_pCurKF = m_pNextKF;
		m_pNextKF = (CKeyFrameNode*)m_pCurKF->m_Next;
		m_fKFProgress = 0.0f;
	}

	float m_fKFProgressNormalized = m_fKFProgress / fCurKFDuration;
	m_pCurrentSpline->SetSplineDiff(m_pCurKF->pSpline, m_pNextKF->pSpline, m_fKFProgressNormalized);

	//char buf[900];
	//CSpline::SSegment seg0;
	//CSpline::SSegment seg1;
	//seg0 = m_pCurKF->pSpline->GetSegment(0);
	//seg1 = m_pCurKF->pSpline->GetSegment(1);	
	//sprintf(buf,"curKF seg0: %.0f %.0f, %.0f %.0f, %.0f %.0f, %.0f %.0f\n", seg0.p0.x, seg0.p0.y, seg0.p0c.x, seg0.p0c.y, seg0.p1.x, seg0.p1.y, seg0.p1c.x, seg0.p1c.y);
	//IwGxPrintString(10, 10, buf, true);
	//seg0 = m_pNextKF->pSpline->GetSegment(0);
	//seg1 = m_pNextKF->pSpline->GetSegment(1);	
	//sprintf(buf,"NextKF seg0: %.0f %.0f, %.0f %.0f, %.0f %.0f, %.0f %.0f\n", seg0.p0.x, seg0.p0.y, seg0.p0c.x, seg0.p0c.y, seg0.p1.x, seg0.p1.y, seg0.p1c.x, seg0.p1c.y);
	//IwGxPrintString(10, 20, buf, true);
	//seg0 = m_pCurrentSpline->GetSegment(0);
	//seg1 = m_pCurrentSpline->GetSegment(1);
	//sprintf(buf,"curSpline seg0: %.0f %.0f, %.0f %.0f, %.0f %.0f, %.0f %.0f\n", seg0.p0.x, seg0.p0.y, seg0.p0c.x, seg0.p0c.y, seg0.p1.x, seg0.p1.y, seg0.p1c.x, seg0.p1c.y);
	//IwGxPrintString(10, 30, buf, true);
	
	//m_pCurrentSpline->lerp(m_pNextKF->pSpline, 0.01f);

	m_pCurrentSpline->Generate();	
}

void CSplineAnimator::Reset()
{
	m_fProgress = 0.0f;
	m_pCurKF = m_pKFRoot;
	m_pNextKF = (CKeyFrameNode*)m_pKFRoot->m_Next;
}
