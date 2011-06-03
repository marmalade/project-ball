#include "CCollision.h"

//-----------------------------------------------------------------------------
IW_MANAGED_IMPLEMENT(CCollision);
IW_CLASS_FACTORY(CCollision);
//-----------------------------------------------------------------------------
#ifdef IW_BUILD_RESOURCES
void CCollision::AddFace(CIwFace* pFace, CIwModel* pModel)
{
	// Remember the model
	IwAssert(GRAPHICS, !m_pModel || m_pModel == pModel);
	m_pModel = pModel;

	// Model build info contains information about how the data in the model was re-arranged.
	// Verts may be reordered for cache performance, etc.
	CIwModelBuildInfoMap& map = m_pModel->GetModelBuildInfo().GetMap(IW_MB_STREAM_VERTS);

	// If this face is a triangle...
	if( pFace->GetNumPoints() == 3 )
	{
		for(uint32 i = 0; i < 3; i++)
		{
			CIwArray<uint16> deps;
			map.GetDependents(pFace->GetVertID(i), deps);
			m_Points.push_back(deps[0]);
		}
	}
	else
	{
		// Only support quads
		IwAssert(GRAPHICS, pFace->GetNumPoints() == 4);
		
		// Add the quad as two triangles
		uint32 i;
		for(i = 0; i < 3; i++)
		{
			CIwArray<uint16> deps;
			map.GetDependents(pFace->GetVertID(i), deps);
			m_Points.push_back(deps[0]);
		}
		for(i = 0; i < 4; i == 0 ? i += 2 : i++)
		{
			CIwArray<uint16> deps;
			map.GetDependents(pFace->GetVertID(i), deps);
			m_Points.push_back(deps[0]);
		}
	}
}
#endif
//-----------------------------------------------------------------------------
void CCollision::Serialise()
{
	CIwManaged::Serialise();

	IwSerialiseManagedHash(&m_pModel);
	m_Points.SerialiseHeader();
	IwSerialiseUInt16(m_Points[0], m_Points.size());
}
//-----------------------------------------------------------------------------
void CCollision::Resolve()
{
	CIwManaged::Resolve();
	
	IwResolveManagedHash(&m_pModel, IW_GRAPHICS_RESTYPE_MODEL);

	//Build face normals (done on resolve to save disk space)
	for(uint32 i = 0; i < m_Points.size(); i+=3)
	{
		CIwVec3 v1 = (CIwVec3)GetVert(i);
		CIwVec3 v2 = (CIwVec3)GetVert(i+1);
		CIwVec3 v3 = (CIwVec3)GetVert(i+2);

		CIwSVec3 cross = (CIwSVec3)(v2 - v1).Cross(v3 - v1);
		if( cross != CIwSVec3::g_Zero )
			cross.Normalise();
		m_Norms.push_back(cross);
	}
}
//-----------------------------------------------------------------------------
//int32 CCollision::GetFaceUnderCursor()
//{
//	//Calculate pos/dir of cursor from camera
//	CIwVec3 pos = IwGxGetViewMatrix().t;
//	CIwVec3 dir(s3ePointerGetX(), s3ePointerGetY(), IwGxGetPerspMul());
//	dir.x -= IwGxGetScreenWidth()/2;
//	dir.y -= IwGxGetScreenHeight()/2;
//
//	//Transform pos/dir into model space
//	dir = IwGxGetViewMatrix().RotateVec(dir);
//	dir = s_ModelMatrix.TransposeRotateVec(dir);
//	dir.Normalise();
//
//	pos = s_ModelMatrix.TransposeTransformVec(pos);
//
//	//find first face intersection
//	int32 minf = INT32_MAX; //nearest intersection distance
//	uint32 nearest = 0; //nearest intersection index
//
//	for(uint32 i = 0; i < m_Points.size(); i+=3)
//	{
//		CIwVec3 v1 = (CIwVec3)GetVert(i);
//		CIwVec3 v2 = (CIwVec3)GetVert(i+1);
//		CIwVec3 v3 = (CIwVec3)GetVert(i+2);
//
//		int32 f = 0;
//		if( IwIntersectLineTriNorm(pos, dir, v1, v2, v3, m_Norms[i/3], f) )
//		{
//			if( f < minf )
//			{
//				minf = f;
//				nearest = i;
//			}
//		}
//	}
//
//	if( minf != INT32_MAX )
//	{
//		return nearest;
//	}
//
//	return -1;
//}
//-----------------------------------------------------------------------------
//#ifdef IW_BUILD_RESOURCES
//void BuildCollision(const char* pUserString, CIwModel* pModel)
//{
//	//check user string to test whether to affect this asset
//	if( strcmp(pUserString, "level") != 0 )
//		return;
//	
//	// Create a new collision resource and name it after its model partner
//	CCollision* pCollision = new CCollision;
//	pCollision->SetName(pModel->DebugGetName());
//
//	for(uint32 i = 0; i < IwGetModelBuilder()->GetNumFaces(); i++)
//	{
//		pCollision->AddFace(IwGetModelBuilder()->GetFace(i), pModel);
//	}
//
//	// Add the collision resource to IwResManager - it will be serialised as part of the current group.
//	IwGetResManager()->AddRes("CCollision", pCollision);
//}
//#endif
