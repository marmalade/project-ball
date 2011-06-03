#include "Utils.h"
#include "string.h"
#include "stdio.h"
#include "IwGx.h"

const float pi = 3.1415926535897932384626433832795f;
const float half_pi = 1.5707963267948966192313216916398f;
const float two_pi = 9.8696044010893586188344909998762f;


Vector2 StringToVector2(const char* str, const char* strDelimiter)
{
	const char* strToken[2];
	strToken[0] = strtok((char*)str, strDelimiter);
	strToken[1] = strtok(NULL, strDelimiter);

	IwAssert(0, strToken[0] && strToken[1]);


	float fx, fy;
	if(EOF == sscanf(strToken[0], "%f", &fx) ||
	   EOF == sscanf(strToken[1], "%f", &fy))
	{
		IwAssertMsg(0, false, ("Bad data in string to float coversion"));
	}

	return Vector2(fx, fy);
}

#define EPSILON 0.000001
#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

int
intersect_triangle(float orig[3], float dir[3],
                   float vert0[3], float vert1[3], float vert2[3],
                   float *t, float *u, float *v)
{
   float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   float det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

#ifdef TEST_CULL           /* define TEST_CULL if culling is desired */
   if (det < EPSILON)
      return 0;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   *u = DOT(tvec, pvec);
   if (*u < 0.0 || *u > det)
      return 0;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, edge1);

    /* calculate V parameter and test bounds */
   *v = DOT(dir, qvec);
   if (*v < 0.0 || *u + *v > det)
      return 0;

   /* calculate t, scale parameters, ray intersects triangle */
   *t = DOT(edge2, qvec);
   inv_det = 1.0 / det;
   *t *= inv_det;
   *u *= inv_det;
   *v *= inv_det;
#else                    /* the non-culling branch */
   if (det > -EPSILON && det < EPSILON)
     return 0;
   inv_det = 1.0f / det;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   *u = DOT(tvec, pvec) * inv_det;
   if (*u < 0.0f || *u > 1.0f)
     return 0;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, edge1);

   /* calculate V parameter and test bounds */
   *v = DOT(dir, qvec) * inv_det;
   if (*v < 0.0f || *u + *v > 1.0f)
     return 0;

   /* calculate t, ray intersects triangle */
   // COMMENTED OUT TO IMPROVE SPEED
   //*t = DOT(edge2, qvec) * inv_det;
#endif
   return 1;
}


void PrintfProperty(int16 x, int16 y, const char* str, float f)
{
	char strstr[50];
	sprintf(strstr, "%s: %f", str, f);
	IwGxPrintString(x, y, strstr, true);
}