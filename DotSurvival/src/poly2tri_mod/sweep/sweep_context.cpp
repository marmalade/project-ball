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
#include "sweep_context.h"
//#include <algorithm>
#include "advancing_front.h"
#include "..\common\shapes.h"


#define SORT_NAME sorter
#define SORT_TYPE p2t::Point*
#define SORT_CMP(x,y) cmp(x,y)

#include "src/sort.h"

#include "Src/DynamicArray.h"

namespace p2t {

SweepContext::SweepContext()
: basin()
, edge_event()
, front_( NULL )
, head_( NULL )
, tail_( NULL )
, af_head_( NULL )
, af_middle_( NULL )
, af_tail_( NULL )
{
}

#define SAFE_DEL(a) { if ((a)!=NULL) { delete (a); (a)=NULL; } }

void SweepContext::Clear()
{
	basin.Clear();
	edge_event.Clear();

	SAFE_DEL( head_ );
	SAFE_DEL( tail_ );
	SAFE_DEL( front_ );
	SAFE_DEL( af_head_ );
	SAFE_DEL( af_middle_ );
	SAFE_DEL( af_tail_ );

	for( int_t i = 0; i < edge_list.Size(); i++ ) {
		delete edge_list[i];
	}

	edge_list.Clear();

	points_.Clear();
	triangles_.Clear();
	map_.Clear();
}

void SweepContext::SetPolyLine( const DynamicArray<Point*>& polyline )
{
	if( polyline.Size() > edge_list.AllocatedSize() ) edge_list.ReallocNonCopy( polyline.Size() );

	points_ = polyline;
	InitEdges( points_ );
}

void SweepContext::AddHole( const DynamicArray<Point*>& polyline )
{
	const int_t size = points_.Size()+polyline.Size();
	if( size > edge_list.AllocatedSize() ) edge_list.Realloc( size );
	if( size > points_.AllocatedSize() )   points_.Realloc( size );

	InitEdges( polyline );
	for( int_t i = 0; i < polyline.Size(); i++ ) {
		points_.push_back( polyline[i] );
	}
}



void SweepContext::InitTriangulation()
{
	const int_t cnt = points_.Size() * 2 - 2;

	if( cnt > map_.AllocatedSize() ) map_.ReallocNonCopy( cnt );
	if( cnt > triangles_.AllocatedSize() ) triangles_.ReallocNonCopy( cnt );

	float xmax(points_[0]->pos.x), xmin(points_[0]->pos.x);
	float ymax(points_[0]->pos.y), ymin(points_[0]->pos.y);

  // Calculate bounds.
  for( int_t i = 0; i < points_.Size(); i++ ) {
    Point& p = *points_[i];
    if (p.pos.x > xmax)
      xmax = p.pos.x;
    if (p.pos.x < xmin)
      xmin = p.pos.x;
    if (p.pos.y > ymax)
      ymax = p.pos.y;
    if (p.pos.y < ymin)
      ymin = p.pos.y;
  }

  float dx = kAlpha * (xmax - xmin);
  float dy = kAlpha * (ymax - ymin);
  head_ = new Point(xmax + dx, ymin - dy);
  tail_ = new Point(xmin - dx, ymin - dy);

  // Sort points along y-axis
  //std::sort(points_.begin(), points_.end(), cmp);

  sorter_tim_sort( points_.begin(), points_.Size() );
  //sorter_quick_sort( points_.begin(), points_.Size() );
}

void SweepContext::InitEdges( const DynamicArray<Point*>& polyline )
{
  int_t num_points = polyline.Size();
  for (int i = 0; i < num_points; i++) {
    int j = i < num_points - 1 ? i + 1 : 0;
    edge_list.push_back(new Edge(*polyline[i], *polyline[j]));
  }
}

Node& SweepContext::LocateNode(Point& point)
{
	// TODO implement search tree
	return *front_->LocateNode(point.pos.x);
}

void SweepContext::RemoveNode(Node* node)
{
	delete node;
}

void SweepContext::CreateAdvancingFront(/*std::vector<Node*> nodes*/)
{
	// Initial triangle
	//Triangle* triangle = new Triangle(*points_[0], *tail_, *head_);

	int_t idxTri = map_.push_back( Triangle( *points_[0], *tail_, *head_ ) );
	Triangle* triangle = &map_[idxTri];

	//map_.push_back(triangle);

	af_head_ = new Node(*triangle->GetPoint(1), *triangle);
	af_middle_ = new Node(*triangle->GetPoint(0), *triangle);
	af_tail_ = new Node(*triangle->GetPoint(2));
	front_ = new AdvancingFront(*af_head_, *af_tail_);

	// TODO: More intuitive if head is middles next and not previous?
	//       so swap head and tail
	af_head_->next = af_middle_;
	af_middle_->next = af_tail_;
	af_middle_->prev = af_head_;
	af_tail_->prev = af_middle_;
}

void SweepContext::MapTriangleToNodes(Triangle& t)
{
  for (int i = 0; i < 3; i++) {
    if (!t.GetNeighbor(i)) {
      Node* n = front_->LocatePoint(t.PointCW(*t.GetPoint(i)));
      if (n)
        n->triangle = &t;
    }
  }
}

/*
void SweepContext::RemoveFromMap(Triangle* triangle)
{
  //map_.remove(triangle);
	wzx_ASSERT(false);
}
*/

void SweepContext::MeshClean(Triangle& triangle)
{
  if (&triangle != NULL && !triangle.IsInterior()) {
    triangle.IsInterior( true );
    triangles_.push_back( triangle );
    for (int i = 0; i < 3; i++) {
      if (!triangle.constrained_edge[i])
        MeshClean(*triangle.GetNeighbor(i));
    }
  }
}

SweepContext::~SweepContext()
{
	Clear();
}

}
