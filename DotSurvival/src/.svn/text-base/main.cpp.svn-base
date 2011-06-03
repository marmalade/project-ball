//--------------------------------------------------------------------------
// HelloWorld main file
//--------------------------------------------------------------------------

#define IW_USE_PROFILE

#include "s3e.h"
#include "IwGx.h"
#include "IwMaterial.h"
#include "IwTexture.h"
#include "CSpline.h"
#include "CSplinePoly2Tri.h"
#include "CSplineAnimator.h"
#include "CBall.h"
#include "s3eAccelerometer.h"
#include "math.h"
#include "IwUtil.h"

typedef CIwFVec2 Vector2;
typedef CIwSVec2 Vector2i;

// Externs for functions which examples must implement
void ExampleInit();
void ExampleShutDown();
void ExampleRender();
bool ExampleUpdate(float dt);

void ResetGame();
long UpdateScreenOrientation();

//CIwSVec2 centre = CIwSVec2((int16)Iw2DGetSurfaceWidth() / 2, (int16)Iw2DGetSurfaceHeight() / 2);

bool s_Accelerometer = false;
CSplineAnimator* m_pAnimator;
CSplinePoly2Tri* m_pSplinePoly2Tri;
CBall* m_pBall;

float g_fLife = 1.0f;

CIwMat s_OrientationCompensation;

//--------------------------------------------------------------------------
// Main global function
//--------------------------------------------------------------------------
S3E_MAIN_DECL void IwMain()
{
#ifdef EXAMPLE_DEBUG_ONLY
	// Test for Debug only examples
#endif

	// Example main loop
	ExampleInit();
	uint64 timeOld = s3eTimerGetMs();
	while (1)
	{
		s3eDeviceYield(0);
		s3eKeyboardUpdate();
		s3ePointerUpdate();

		uint64 timeNew = s3eTimerGetMs();
		float dt = (timeNew - timeOld) * 0.001f;
		timeOld = timeNew;

		bool result = ExampleUpdate(dt);
		if (
			(result == false) ||
			(s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_DOWN)||
			(s3eKeyboardGetState(s3eKeyLSK) & S3E_KEY_STATE_DOWN)||
			(s3eDeviceCheckQuitRequest())
			)
			break;
		ExampleRender();
		//s3eSurfaceShow();
	}
	ExampleShutDown();
}


void ExampleInit()
{
	IW_PROFILE_INIT();

	// Initialise
	IwGxInit();

	// Set screen clear colour
	IwGxSetColClear(0x40, 0x40, 0x40, 0x00);

	// Turn all lighting off
	IwGxLightingOff();

	// Set field of view
	IwGxSetPerspMul(0xa0);

	// Set near and far planes
	IwGxSetFarZNearZ(0x1000, 0x10);

	// Set the view matrix along the -ve z axis
	CIwMat view = CIwMat::g_Identity;	
	view.t.z = -0x200;
	IwGxSetViewMatrix(&view);
	//IwGxSetScreenOrient(IW_GX_ORIENT_NONE);

	s_Accelerometer = (S3E_RESULT_SUCCESS == s3eAccelerometerStart());

	m_pAnimator = new CSplineAnimator();
	m_pAnimator->Init();

	m_pSplinePoly2Tri = new CSplinePoly2Tri(m_pAnimator->GetCurrentSpline());

	// ball
	m_pBall = new CBall();
}

bool ExampleUpdate(float dt)
{
	IW_PROFILE("update");

	UpdateScreenOrientation();

	m_pAnimator->Update(dt);
	m_pSplinePoly2Tri->SetSpline(m_pAnimator->GetCurrentSpline());
	m_pSplinePoly2Tri->Generate();

	m_pBall->Update(dt);

	if (s_Accelerometer)
	{
		//iAccX = iClamp(iAccX, 0, 70
		m_pBall->SetAcceleration(Vector2i((int16)s3eAccelerometerGetX(), (int16)s3eAccelerometerGetY()));
	}

	// check collision

	//if (m_pSplineTriFan->IsPointWithin(m_pBall->GetPos()))
	//{
	//	IwGxPrintString(10, 10, "collision yes", true);
	//}
	//else
	//{
	//	IwGxPrintString(10, 10, "collision no", true);
	//	g_fLife += -0.1f * dt;		
	//}

	char strLife[50];
	sprintf(strLife, "life left: %f", g_fLife);
	IwGxPrintString(10, 20, strLife, true);

	if (g_fLife <= 0.0f)
	{		
		ResetGame();
	}

	return true;
}

class CIwProfileIteratorDerived : public CIwProfileIterator
{
public:
	CIwProfileIteratorDerived( CIwProfileNode* start ) : CIwProfileIterator( start ) {}
};

void ExampleRender()
{
	//IW_PROFILE_NEWFRAME();
	
	CIwProfileIteratorDerived pProfileIterator(CIwProfileManager::Get().GetRoot());

	pProfileIterator.First();
	int iy = 30;
	while (!pProfileIterator.IsDone())
	{
		PrintfProperty(10, iy, pProfileIterator.GetCurrentName(), pProfileIterator.GetCurrentLastTime());
		pProfileIterator.Next();
		iy += 10;
	}

	PrintfProperty(10, 90, "points: ", (float)m_pAnimator->GetCurrentSpline()->GetPointCount());


	IW_PROFILE_START("render");

	CIwMaterial* pMat;

	// Clear the screen
	IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

	//// Allocate and initialise material from the IwGx global cache
	pMat = IW_GX_ALLOC_MATERIAL();

	//// Set this as the active material
	IwGxSetMaterial(pMat);

	m_pSplinePoly2Tri->Render();
	//IwGxSetColStream(s_Cols, 8);
	m_pBall->Render();
	IwGxSetColStream(NULL);

	// End drawing
	IwGxFlush();

	IW_PROFILE_STOP();

	// Swap buffers
	IwGxSwapBuffers();
}


void ExampleShutDown()
{
	delete m_pAnimator;
	delete m_pSplinePoly2Tri;
	delete m_pBall;
	
	// Terminate
	IwGxTerminate();

	if (s_Accelerometer)
	{
		s3eAccelerometerStop();
		s_Accelerometer = false;
	}

	IW_PROFILE_TERM();
	delete &CIwProfileMenu::Get();
}


void ResetGame()
{
	g_fLife = 1.0f;
	m_pAnimator->Reset();
	m_pBall->SetPos(Vector2(200.0f, 200.0f));
}

long UpdateScreenOrientation()
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
