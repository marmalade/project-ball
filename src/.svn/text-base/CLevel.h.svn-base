#ifndef CLEVEL_H
#define CLEVEL_H

#include "IwGeomVec3.h"
#include "btBulletDynamicsCommon.h"

class CIwModel;
class CCollision;

class CLevel
{
public:
	CLevel();
	~CLevel();

	void Initialize(btDiscreteDynamicsWorld* pDynamicsWorld);

	CIwModel*	 GetCurrentSectorModel();
	btRigidBody* GetCurrentSectorCollision();

	int	GetCurrentSegmentY() { return m_iCurrentSegmentY; };
	
	void Update(int dt, CIwVec3 vBallPos);

private:
	static void BuildCollision(const char* pUserString, CIwModel* pModel);

	int							m_iCurrentSegmentX;
	int							m_iCurrentSegmentY;

	CIwModel**					m_apCollisionModel;
	CIwModel*					m_pCurrentCollisionModel;
	CCollision**				m_apCollision;
	CCollision*					m_pCurrentCollision;
	btBvhTriangleMeshShape**	m_apGroundShape;
	btDefaultMotionState**		m_apMotionState;
	btRigidBody**				m_apBodyGround;
	btTriangleIndexVertexArray** m_apIndexVertexArrays;

	int**			m_apiColIndices;
	float**			m_apfColVertices;	

	CIwVec3						m_vBallPos;

	// bullet

	btDiscreteDynamicsWorld* m_pDynamicsWorld;
	btAlignedObjectArray<btCollisionShape*>	m_pCollisionShapes;

	

};

#endif // CLEVEL_H
