/* 
 * Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors
 * http://code.google.com/p/poly2tri/
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * * Neither the name of Poly2Tri nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#ifndef P2T_UTILS_H
#define P2T_UTILS_H

//#include <exception>
//#include <math.h>
//#include "src/utils.h"
#include "shapes.h"

//#include "wzxSYS\wzx_Math.h"

namespace p2t {

const float PI_3div4 = 3 * pi * 0.25f;
const float myEPSILON = 1e-5f;

enum EOrientation { CW = 0, CCW = 1, COLLINEAR = 2 };

/**
 * Forumla to calculate signed area<br>
 * Positive if CCW<br>
 * Negative if CW<br>
 * 0 if collinear<br>
 * <pre>
 * A[P1,P2,P3]  =  (x1*y2 - y1*x2) + (x2*y3 - y2*x3) + (x3*y1 - y3*x1)
 *              =  (x1-x3)*(y2-y3) - (y1-y3)*(x2-x3)
 * </pre>
 */

EOrientation Orient2d( const p2t::Point& pa, const p2t::Point& pb, const p2t::Point& pc )
{
  const float detleft = ( pa.pos.x - pc.pos.x ) * ( pb.pos.y - pc.pos.y );
  const float detright = ( pa.pos.y - pc.pos.y ) * ( pb.pos.x - pc.pos.x );
  const float val = detleft - detright;

  if (val > -myEPSILON && val < myEPSILON) {
    return COLLINEAR;
  } else if (val > 0) {
    return CCW;
  }
  return CW;
}

bool InScanArea( const Point& pa, const Point& pb, const Point& pc, const Point& pd )
{
  const float pdx = pd.pos.x;
  const float pdy = pd.pos.y;
  const float adx = pa.pos.x - pdx;
  const float ady = pa.pos.y - pdy;
  const float bdx = pb.pos.x - pdx;
  const float bdy = pb.pos.y - pdy;

  const float oabd = ( adx * bdy ) - ( bdx * ady );

  if( oabd <= myEPSILON ) return false;

  const float cdx = pc.pos.x - pdx;
  const float cdy = pc.pos.y - pdy;
  const float ocad = ( cdx * ady ) - ( adx * cdy );

  if( ocad <= myEPSILON ) return false;

  return true;
}

}

#endif // P2T_UTILS_H

