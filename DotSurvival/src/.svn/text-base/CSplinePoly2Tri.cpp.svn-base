#include "CSplinePoly2Tri.h"
#include "Src/poly2tri_mod/poly2tri.h"
#include "CSpline.h"
#include "DynamicArray.h"
#include "IwGx.h"

CSplinePoly2Tri::CSplinePoly2Tri(CSpline* pSpline)
{
	m_cdt = new p2t::CDT();
	m_apVertices = NULL;
	
	int iMaxTheoreticalTriCount = pSpline->GetPointCount()*2;

	m_iIndexCount = iMaxTheoreticalTriCount*3;

	m_apVertices = new Vector3i[m_iIndexCount];
	m_apIndices = new uint16[m_iIndexCount];

	for (int i = 0; i < m_iIndexCount; i++)
	{
		m_apVertices[i] = Vector3i(0, 0, 0);
		m_apIndices[i] = i;
	}
}

CSplinePoly2Tri::~CSplinePoly2Tri()
{
	delete m_cdt;

	delete [] m_apVertices;
	delete [] m_apIndices;
}

void CSplinePoly2Tri::Generate()
{
	DynamicArray<p2t::Point*> myArray;
	Vector2* pSplinePoints = m_pSpline->GetPoints();

	for (int i = 0; i < m_pSpline->GetPointCount(); i++)
	{
		myArray.Add( new p2t::Point( pSplinePoints[i].x, pSplinePoints[i].y ) );
	}

	m_cdt->SetPolyLine(myArray);
	
	m_cdt->Triangulate();	

	int iTriangleCount = m_cdt->GetTriangles().Size();
	m_iIndexCount = iTriangleCount*3;

	for (int i = 0; i < iTriangleCount; i++)
	{
		m_apVertices[i*3].x	= (int16)m_cdt->GetTriangles()[i].GetPoint(2)->pos.x;
		m_apVertices[i*3].y	= (int16)m_cdt->GetTriangles()[i].GetPoint(2)->pos.y;
		m_apVertices[i*3+1].x = (int16)m_cdt->GetTriangles()[i].GetPoint(1)->pos.x;
		m_apVertices[i*3+1].y = (int16)m_cdt->GetTriangles()[i].GetPoint(1)->pos.y;
		m_apVertices[i*3+2].x = (int16)m_cdt->GetTriangles()[i].GetPoint(0)->pos.x;
		m_apVertices[i*3+2].y = (int16)m_cdt->GetTriangles()[i].GetPoint(0)->pos.y;
	}

	// clear
	m_cdt->Clear();
	for( int i = 0; i < myArray.Size(); i++ )
	{
		delete myArray[i];
		myArray[i]->edge_list.Clear();
	}
}

void CSplinePoly2Tri::Render()
{
	// Build model matrix from angles
	//CIwMat	rotX, rotY, rotZ;
	//rotX.SetRotX(s_Angles.x);
	//rotY.SetRotY(s_Angles.y);
	//rotZ.SetRotZ(s_Angles.z);
	//m_ModelMatrix = rotX * rotY * rotZ;
	CIwMat m_ModelMatrix;
	m_ModelMatrix.SetIdentity();

	// Set the model matrix
	IwGxSetModelMatrix(&m_ModelMatrix);

	// Set screenspace vertex coords
	IwGxSetVertStreamModelSpace(m_apVertices, m_iIndexCount);

	IwGxDrawPrims(IW_GX_TRI_LIST, m_apIndices, m_iIndexCount);
}