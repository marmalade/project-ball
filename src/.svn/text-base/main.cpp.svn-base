#define CHECK_MEMORY_LEAKS 1
#include "btBulletDynamicsCommon.h"
#include "s3eAudio.h"
#include "IwGraphics.h"
#include "IwModel.h"
#include "IwModelBuilder.h"
#include "IwResManager.h"
#include "IwGxPrint.h"
#include "s3eKeyboard.h"
#include "s3eAccelerometer.h"

#include "CCollision.h"
#include "CLevel.h"




// bullet stuff

btDefaultCollisionConfiguration* m_collisionConfiguration;
btCollisionDispatcher*	m_dispatcher;
btBroadphaseInterface*	m_broadphase;
btConstraintSolver*	m_solver;
btDiscreteDynamicsWorld* m_dynamicsWorld;
//keep the collision shapes, for deletion/cleanup
btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
btRigidBody* bulletbody;



typedef CIwVec3 Vector3;
typedef CIwMat Matrix4;

const int NUM_WALLS = 6;

CIwModel* s_Model=0;
CIwMat s_ModelMatrix;
CIwMat s_OrientationCompensation;
bool s_Accelerometer;
s3eBool s_MP3Supported;
int s_FramesPerSecond = 0;

const int MAX_CONTACTS = 10;
char s_Errbuf [1024];

CLevel*	m_pLevel;
CIwMat m_mView;

Vector3 m_vBallPos;


long UpdateScreenOrientation(void*,void*)
{
	int rot = s3eSurfaceGetInt(S3E_SURFACE_DEVICE_BLIT_DIRECTION);
	if (rot == S3E_SURFACE_BLIT_DIR_NORMAL)
	{
		s_OrientationCompensation.SetIdentity();
	} 
	else if (rot == S3E_SURFACE_BLIT_DIR_ROT90)
	{
		s_OrientationCompensation.SetRotZ(0x0c00);
	} 
	else if (rot == S3E_SURFACE_BLIT_DIR_ROT180)
	{
		s_OrientationCompensation.SetRotZ(0x800);
	} 
	else if (rot == S3E_SURFACE_BLIT_DIR_ROT270)
	{
		s_OrientationCompensation.SetRotZ(0x400);
	}
	return 0;
}

void Init()
{
	s_Errbuf[0] = 0;

	// Initialise
	IwGxInit();
	IwGraphicsInit();

	IW_CLASS_REGISTER(CCollision);

	s_Accelerometer = (S3E_RESULT_SUCCESS == s3eAccelerometerStart());
	s_MP3Supported = s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3);

	s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, (s3eCallback) UpdateScreenOrientation, NULL);
	UpdateScreenOrientation(0,0);

	

//int32 pixelType = s3eSurfaceGetInt(S3E_SURFACE_PIXEL_TYPE);
//s3eResult res = s3eSurfaceSetup((s3eSurfacePixelType)pixelType, 0, NULL, S3E_SURFACE_BLIT_DIR_NATIVE );


	// Set screen clear colour
	IwGxSetColClear(100, 100, 100, 0);

	// Set field of view
	IwGxSetPerspMul(0xa0);

	// Set near and far planes
	IwGxSetFarZNearZ(0x800, 0x10);
	CIwResGroup* pGroup = 0;


	pGroup = IwGetResManager()->LoadGroup("ball.group");
	s_Model = (CIwModel*)pGroup->GetResNamed("ball", IW_GRAPHICS_RESTYPE_MODEL);


	//**************** bullet init
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
	m_broadphase = new btDbvtBroadphase();
	m_solver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	

	// init level and pass the dynamicsWorld
	m_pLevel = new CLevel();
	m_pLevel->Initialize(m_dynamicsWorld);


	//void* mem = btAlignedAlloc(sizeof(btDiscreteDynamicsWorld),16);
	//m_dynamicsWorld = new (mem) btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	//m_dynamicsWorld->setGravity(btVector3(5,-30,0));

	btVector3 localInertia(0,0,0);

	{
		//create a dynamic rigidbody
		btCollisionShape* colShape = new btSphereShape(10.0f);
		m_collisionShapes.push_back(colShape);

		btTransform startTransform;
		startTransform.setIdentity();

		colShape->calculateLocalInertia(1.0f, localInertia);


		startTransform.setOrigin(btVector3(0.0f,0.0f,0.0f));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f,myMotionState,colShape,localInertia);
		bulletbody = new btRigidBody(rbInfo);
		
		//bulletbody->setFriction(0.0f);
		bulletbody->setRestitution(0.5f);
		
		bulletbody->setActivationState(ACTIVE_TAG);

		bulletbody->setLinearFactor(btVector3(1.0f, 1.0f, 0.0f));
		m_dynamicsWorld->addRigidBody(bulletbody);
	}

	//*************** bullet init end




	sprintf(s_Errbuf+strlen(s_Errbuf),"init done\n");

	//if (s_MP3Supported)
	//	s3eAudioPlay("dice.mp3", 1);

	m_vBallPos = Vector3();
}

void ShutDown()
{

	delete m_pLevel;

	//bullet

	// remove the rigidbodies from the dynamics world and delete them
	int i;
	for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}

	//m_dynamicsWorld->~btDiscreteDynamicsWorld();
	//btAlignedFree(m_dynamicsWorld);
	delete m_dynamicsWorld;

	delete m_solver;
	
	delete m_broadphase;
	
	delete m_dispatcher;

	delete m_collisionConfiguration;

	//

	if (s_Accelerometer)
	{
		s3eAccelerometerStop();
		s_Accelerometer = false;
	}

	// Terminate
	s3eAudioStop();

	IwGraphicsTerminate();
	IwGxTerminate();
}

CIwMat GetMatrix(btRigidBody* b)
{
	CIwMat s_ModelMatrix;
	
	btMatrix3x3 mRot;
	btTransform btTrans = b->getWorldTransform();
	btVector3 pos = btTrans.getOrigin();
	mRot = btTrans.getBasis();

	s_ModelMatrix.SetIdentity();

	s_ModelMatrix.m[0][0] = IW_FIXED(mRot.getRow(0).x());
	s_ModelMatrix.m[0][1] = IW_FIXED(mRot.getRow(0).y());
	s_ModelMatrix.m[0][2] = IW_FIXED(mRot.getRow(0).z());

	s_ModelMatrix.m[1][0] = IW_FIXED(mRot.getRow(1).x());
	s_ModelMatrix.m[1][1] = IW_FIXED(mRot.getRow(1).y());
	s_ModelMatrix.m[1][2] = IW_FIXED(mRot.getRow(1).z());

	s_ModelMatrix.m[2][0] = IW_FIXED(mRot.getRow(2).x());
	s_ModelMatrix.m[2][1] = IW_FIXED(mRot.getRow(2).y());
	s_ModelMatrix.m[2][2] = IW_FIXED(mRot.getRow(2).z());

	s_ModelMatrix.SetTrans(CIwVec3((int32)(pos[0]),(int32)(pos[1]),(int32)(pos[2])));	
	return s_ModelMatrix;
}

bool Update(int dt)
{	
	if (s_Accelerometer)
	{
		int32 x = s3eAccelerometerGetX();
		int32 y = s3eAccelerometerGetY();
		int32 z = s3eAccelerometerGetZ();

		CIwVec3 r = s_OrientationCompensation.TransposeRotateVec(CIwVec3(x,y,z));

		m_dynamicsWorld->setGravity(btVector3(r.x/30.0f,r.y/30.0f,0.0f));
	}
	else
	{
		m_dynamicsWorld->setGravity(btVector3(0.0f,-50.0f,0.0f));
	}

	s_ModelMatrix = GetMatrix(bulletbody);
	m_vBallPos = s_ModelMatrix.GetTrans();

	m_pLevel->Update(dt, m_vBallPos);
	//if(s3eKeyboardGetState(s3eKeyUp) && S3E_KEY_STATE_DOWN)
	//{
	//	float f[3];
	//	dWorldImpulseToForce(world, 0.01f, (rand()/16386.0f-1.0f)*60.0f, (rand()/16386.0f-1.0f)*60.0f, (rand()/16386.0f-1.0f)*60.0f, f);
	//	dBodyAddForce(ball[0].body, f[0], f[1], f[2]);
	//}

	///step the simulation
	if (m_dynamicsWorld)
	{
		m_dynamicsWorld->stepSimulation(dt / 500.f);
		m_dynamicsWorld->applyGravity();
		//optional but useful: debug drawing
		//m_dynamicsWorld->debugDrawWorld();
	}

	//simLoop(0, 0.2f);

	//// Update IwGx state time stamp
	//IwGxTickUpdate();
	return true;
}


//void LookAt( Vector3 vPos, Vector3 vAtPoint, Vector3 vUp )
//{
//	Vector3 vAt = ( vAtPoint - vPos );
//	vAt.Normalise();
//	Vector3 vRight = vAt.Cross( vUp );
//	vUp = vRight.Cross( vAt );
//
//	Matrix4 mRot, m1, mTrans;
//	mRot.SetIdentity();
//	m1.SetIdentity();
//
//	mRot.
//	mRot.SetRotX( vRight );
//	mRot.SetRotY( -vAt );
//	mRot.SetRotZ( vUp );
//
//	m_mView = m1 * mRot;
//	
//	m_mView.Translate( -vPos );
//}


void Render()
{
	// Set up the view matrix
	//view.SetIdentity();
	//view.SetRotZ(0x800);
	//view = s_OrientationCompensation;

	//view.SetRotY(ua++);
	//view.t = GetMatrix(bulletbody).t;
	//view.t.y =  0x80;
	//view.t.z = -200;

	Vector3 ballPos = GetMatrix(bulletbody).t;

	m_mView.LookAt(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, -1, 0));	
	//m_mView *= s_OrientationCompensation;
	m_mView.t = GetMatrix(bulletbody).t;
	m_mView.t.z = 200;
	
	IwGxSetViewMatrix(&m_mView);

	// Clear the screen
	IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

	// Start with no lighting
	//IwGxLightingOff();

	char buf [128];
	if (s_Accelerometer)
	{
		int x = s3eAccelerometerGetX();
		int y = s3eAccelerometerGetY();
		int z = s3eAccelerometerGetZ();
		sprintf(buf,"FPS: %d, Acc X:%d Y:%d Z:%d",s_FramesPerSecond,x,y,z);
	}
	else
	{
		sprintf(buf,"FPS: %d, No Acc",s_FramesPerSecond);
	}

	//-------------------------------------------------------------------------
	// Render the model
	//-------------------------------------------------------------------------
	//CIwVec3 lpos (0,0,-100);
	//IwGxSetLightPos(0,&lpos);
	//IwGxLightingOn();
	//IwGxLightingDiffuse(true);

	s_ModelMatrix = GetMatrix(bulletbody);
	//s_ModelMatrix.SetIdentity();
	
	IwGxSetModelMatrix(&s_ModelMatrix);
	if (s_Model)
		s_Model->Render();

	s_ModelMatrix = GetMatrix(m_pLevel->GetCurrentSectorCollision());
	IwGxSetModelMatrix(&s_ModelMatrix);
	if (m_pLevel->GetCurrentSectorModel())
		m_pLevel->GetCurrentSectorModel()->Render();
	
	//sprintf(buf,"x: %d %d %d", (int)m_vBallPos.x, (int)m_vBallPos.y, (int)m_vBallPos.z);
	//sprintf(buf, "sector: %d", m_pLevel->GetCurrentSegmentY());

	IwGxPrintString(10, 10, buf, true);
	IwGxPrintString(10, 20, s_Errbuf, true);

	// Paint the cursor keys buttons and prompt text
	//IwGxPrintString(2, IwGxGetScreenHeight() - 60, "Rotate Model: Arrow Keys");

	// End drawing
	IwGxFlush();

	// Swap buffers
	IwGxSwapBuffers();
}

S3E_MAIN_DECL void IwMain() 
{
	Init();

	uint64 now = s3eTimerGetMs(); 
	uint64 fixedLoop = 0;
	int fpsLoop = 0;
	int frames = 0;
	while (1)
	{
		s3eDeviceYield(0);
		s3eKeyboardUpdate();
		s3ePointerUpdate();
		
		if	(
			(s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_DOWN) ||
			(s3eKeyboardGetState(s3eKeyAbsBSK) & S3E_KEY_STATE_DOWN) ||
			(s3eDeviceCheckQuitRequest())
			)
			break;
		uint64 newTime = s3eTimerGetMs();
		uint64 deltaTime = newTime - now;
		now = newTime;

		if (!Update(deltaTime))
			return;

		Render();
	}
	ShutDown();
}


//-----------------------------------------------------------------------------
