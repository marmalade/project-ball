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
#include "cdt.h"

namespace p2t {

CDT::CDT()
: m_Sweep()
{
	m_SweepCtx = new SweepContext();
}

void CDT::SetPolyLine( const DynamicArray<Point*>& polyline )
{
	m_SweepCtx->SetPolyLine( polyline );
}

void CDT::AddHole( const DynamicArray<Point*>& polyline )
{
	m_SweepCtx->AddHole( polyline );
}

void CDT::AddPoint( Point* point )
{
	m_SweepCtx->AddPoint( point );
}

void CDT::Clear()
{
	m_Sweep.Clear();
	m_SweepCtx->Clear();
}

void CDT::Triangulate()
{
	m_Sweep.Triangulate( *m_SweepCtx );
}

const DynamicArray<p2t::Triangle>& CDT::GetTriangles()
{
	return m_SweepCtx->GetTriangles();
}

const DynamicArray<p2t::Triangle>& CDT::GetMap()
{
	return m_SweepCtx->GetMap();
}

CDT::~CDT()
{
	delete m_SweepCtx; m_SweepCtx = NULL;
	//delete sweep_;
}

}
