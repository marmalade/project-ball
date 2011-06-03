#include "CLevel.h"
#include "CCollision.h"

#include "IwModelBuilder.h"
#include "IwGraphics.h"
#include "IwResManager.h"

#define NUM_SECTORS 9

CLevel::CLevel() {};

CLevel::~CLevel()
{
	for (int i = 0; i < NUM_SECTORS; i++)
	{
		free(m_apfColVertices[i]);
		free(m_apiColIndices[i]);
	}

	delete [] m_apiColIndices;
	delete [] m_apfColVertices;


	//delete collision shapes
	for (int j=0;j<m_pCollisionShapes.size();j++)
	{
		btCollisionShape* shape = m_pCollisionShapes[j];
		delete shape;
	}

	delete m_apCollisionModel;
	delete m_apCollision;
	delete m_apBodyGround;
	delete[] m_apMotionState;
	delete[] m_apIndexVertexArrays;
};


void CLevel::Initialize(btDiscreteDynamicsWorld* pDynamicsWorld)
{

	#if defined IW_DEBUG && (defined IW_BUILD_RESOURCES)
		if (IwGetResManager()->BuildResources()) 
		return; 
		// Register the model builder callbacks
		IwGetModelBuilder()->SetPostBuildFn(&BuildCollision);
	#endif 

	CIwResGroup* pGroup = 0;

	m_iCurrentSegmentX = 0;
	m_iCurrentSegmentY = 0;

	m_pCurrentCollisionModel = NULL;
	m_pCurrentCollision = NULL;

	// allocating pointers
	m_apCollisionModel = new CIwModel*[NUM_SECTORS];
	m_apCollision = new CCollision*[NUM_SECTORS];
	m_apBodyGround = new btRigidBody*[NUM_SECTORS];
	m_apGroundShape = new btBvhTriangleMeshShape*[NUM_SECTORS];
	m_apMotionState = new btDefaultMotionState*[NUM_SECTORS];
	m_apIndexVertexArrays = new btTriangleIndexVertexArray*[NUM_SECTORS];
	m_apiColIndices = new int*[NUM_SECTORS];
	m_apfColVertices = new float*[NUM_SECTORS];

	m_pDynamicsWorld = pDynamicsWorld;
	btVector3 localInertia(0,0,0);

	pGroup = IwGetResManager()->LoadGroup("col3.group");

	for (int i = 0; i < NUM_SECTORS; i++)
	{
		char strColFile[20];
		sprintf(strColFile, "Box%d%d", i / 3, i % 3);
		m_apCollisionModel[i] = (CIwModel*)pGroup->GetResNamed(strColFile, IW_GRAPHICS_RESTYPE_MODEL);
		m_apCollision[i] = (CCollision*)pGroup->GetResNamed(strColFile, "CCollision");
		// find some way to delete old unused trimeshes

		uint16 vertcount = m_apCollision[i]->GetVertCount();
		m_apiColIndices[i] = (int*)malloc(vertcount * sizeof(int));
		for (int j = 0; j < vertcount; j+=1)
		{
			m_apiColIndices[i][j] = j;
			//m_colIndices[i+1] = i+1;
			//m_colIndices[i+2] = i;
		}

		m_apfColVertices[i] = (btScalar*)malloc(vertcount*3 * sizeof(btScalar));
		for (int j = 0; j<vertcount; j++)
		{
			m_apfColVertices[i][j*3] = m_apCollision[i]->GetVert(j).x;
			m_apfColVertices[i][j*3+1] = m_apCollision[i]->GetVert(j).y;
			m_apfColVertices[i][j*3+2] = m_apCollision[i]->GetVert(j).z;
		}

		// bullet

		m_apIndexVertexArrays[i] = new btTriangleIndexVertexArray(vertcount/3,
			m_apiColIndices[i],
			3*sizeof(int),
			vertcount,(btScalar*) m_apfColVertices[i], 3*sizeof(btScalar));

		m_apGroundShape[i] = new btBvhTriangleMeshShape(m_apIndexVertexArrays[i],true);
		m_pCollisionShapes.push_back(m_apGroundShape[i]);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0,0,0));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		m_apMotionState[i] = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f,m_apMotionState[i],m_apGroundShape[i],localInertia);
		m_apBodyGround[i] = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		//body->setFriction(0.0f);
		m_apBodyGround[i]->setRestitution(0.9f);			
	}

	m_pDynamicsWorld->addRigidBody(m_apBodyGround[0]);
	
}

CIwModel* CLevel::GetCurrentSectorModel()
{
	return m_pCurrentCollisionModel;
}

btRigidBody* CLevel::GetCurrentSectorCollision()
{
	return m_apBodyGround[0];
}


void CLevel::Update(int dt, CIwVec3 vPos)
{
	m_vBallPos = vPos;

	int iNewCurrentSegmentY = ((vPos.y - 100) / -200) * 3 + (vPos.x + 300) / 200;
	if (m_iCurrentSegmentY != iNewCurrentSegmentY)
	{
		m_pDynamicsWorld->removeRigidBody(m_apBodyGround[m_iCurrentSegmentY]);

		m_iCurrentSegmentY = iNewCurrentSegmentY;

		m_pDynamicsWorld->addRigidBody(m_apBodyGround[m_iCurrentSegmentY]);

		m_pCurrentCollisionModel = m_apCollisionModel[m_iCurrentSegmentY];
		m_pCurrentCollision = m_apCollision[m_iCurrentSegmentY];
		
	}

}

#ifdef IW_BUILD_RESOURCES
void CLevel::BuildCollision(const char* pUserString, CIwModel* pModel)
{
	//check user string to test whether to affect this asset
	/*if( strcmp(pUserString, "level") != 0 )
		return*/;
	
	// Create a new collision resource and name it after its model partner
	CCollision* pCollision = new CCollision;
	pCollision->SetName(pModel->DebugGetName());

	for(uint32 i = 0; i < IwGetModelBuilder()->GetNumFaces(); i++)
	{
		pCollision->AddFace(IwGetModelBuilder()->GetFace(i), pModel);
	}

	// Add the collision resource to IwResManager - it will be serialised as part of the current group.
	IwGetResManager()->AddRes("CCollision", pCollision);
}
#endif
