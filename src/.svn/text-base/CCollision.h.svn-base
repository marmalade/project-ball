#ifndef CCOLLISION
#define CCOLLISION

#include "IwResManager.h"
#include "IwGraphics.h"
#include "IwModel.h"
#include "IwModelBuilder.h"
#include "s3eKeyboard.h"
#include "s3ePointer.h"


//-----------------------------------------------------------------------------
// CCollision is a custom resource that is constructed from a geo at ModelParseClose time.
// It is then used to generate some procedural geometry based on a sub set of the model's faces 
//-----------------------------------------------------------------------------
class CCollision : public CIwResource
{
public:
	IW_MANAGED_DECLARE(CCollision);

	CCollision() : m_pModel(NULL) {};

#ifdef IW_BUILD_RESOURCES
	void AddFace(CIwFace* pFace, CIwModel* pModel);
#endif

	// Standard CIwResource interface
	void Serialise();
	void Resolve();
	//int32 GetFaceUnderCursor();

	// helper function for looking up verts from CIwModel's vertex block
	CIwSVec3& GetVert(int32 i)
	{
		return ((CIwSVec3*)m_pModel->GetVerts())[m_Points[i]];
	}

	int GetVertCount() { return m_Points.size();};

private:
	// The model this collision is based on
	CIwModel* m_pModel;
	// A list of points. Each triplet is one triangle.
	CIwArray<uint16> m_Points;
	CIwArray<CIwSVec3> m_Norms;
};

#endif // CCOLLISION
