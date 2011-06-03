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

// Include guard
#ifndef SHAPES_H
#define SHAPES_H

//#include <vector>
//#include <cstddef>
//#include <assert.h>
//#include <cmath>

#include "Src/Utils.h"
//#include "Src/poly2tri_mod/common/Utils.h"

typedef Vector2 Vec2;

//#include "wzxSYS/wzx_Debug.h"

#include "Src/DynamicArray.h"

namespace p2t {

struct Edge;

struct Point 
{
	Vec2 pos;
	//double x, y;

	/// The edges this point constitutes an upper ending point
	//std::vector<Edge*>
	DynamicArray<Edge*> edge_list;

	Point() : pos( 0.f, 0.f )
	{
	}

	Point( const Vec2& v ) : pos(v) {}
	Point( float x, float y ) : pos( x,y ) {}

	/*
  /// Add a point to this point.
  void operator +=(const Point& v)
  {
    x += v.x;
    y += v.y;
  }

  /// Subtract a point from this point.
  void operator -=(const Point& v)
  {
    x -= v.x;
    y -= v.y;
  }

  /// Multiply this point by a scalar.
  void operator *=(double a)
  {
    x *= a;
    y *= a;
  }

  /// Get the length of this point (the norm).
  double Length() const
  {
    return sqrt(x * x + y * y);
  }

  /// Convert this point into a unit point. Returns the Length.
  double Normalize()
  {
    double len = Length();
    x /= len;
    y /= len;
    return len;
  }
	*/
};

inline bool operator == ( const Point& a, const Point& b )
{
	return a.pos == b.pos;
}

inline bool operator != ( const Point& a, const Point& b )
{
	return a.pos != b.pos;
}


// Represents a simple polygon's edge
struct Edge {

  Point *p,*q;

  /// Constructor
  Edge(Point& p1, Point& p2) : p(&p1), q(&p2)
  {
    if (p1.pos.y > p2.pos.y) {
      q = &p1;
      p = &p2;
    } else if (p1.pos.y == p2.pos.y) {
      if (p1.pos.x > p2.pos.x) {
        q = &p1;
        p = &p2;
      } else if (p1.pos.x == p2.pos.x) {
        // Repeat points
        wzx_ASSERT(false);
      }
    }

    q->edge_list.push_back(this);
  }
};

// Triangle-based data structures are know to have better performance than quad-edge structures
// See: J. Shewchuk, "Triangle: Engineering a 2D Quality Mesh Generator and Delaunay Triangulator"
//      "Triangulations in CGAL"
class Triangle
{
public:

	Triangle() {}
	Triangle( Point& a, Point& b, Point& c );

	Point* GetPoint( const int index ) const { return points_[index]; }
	Point* PointCW( Point& point );
	Point* PointCCW( Point& point );
	Point* OppositePoint( Triangle& t, Point& p );

	Triangle* GetNeighbor( const int index ) { return neighbors_[index]; }
	void MarkNeighbor(Point* p1, Point* p2, Triangle* t);
	void MarkNeighbor(Triangle& t);

	void MarkConstrainedEdge(const int index);
	void MarkConstrainedEdge(Edge& edge);
	void MarkConstrainedEdge(Point* p, Point* q);

	int Index(const Point* p);
	int EdgeIndex(const Point* p1, const Point* p2);

	Triangle* NeighborCW(Point& point);
	Triangle* NeighborCCW(Point& point);
	bool GetConstrainedEdgeCCW(Point& p);
	bool GetConstrainedEdgeCW(Point& p);
	void SetConstrainedEdgeCCW(Point& p, bool ce);
	void SetConstrainedEdgeCW(Point& p, bool ce);
	bool GetDelunayEdgeCCW(Point& p);
	bool GetDelunayEdgeCW(Point& p);
	void SetDelunayEdgeCCW(Point& p, bool e);
	void SetDelunayEdgeCW(Point& p, bool e);

	bool Contains(Point* p);
	bool Contains(const Edge& e);
	bool Contains(Point* p, Point* q);
	void Legalize(Point& point);
	void Legalize(Point& opoint, Point& npoint);
	void ClearNeighbors();
	void ClearDelunayEdges();

	inline bool IsInterior() { return interior_; }
	inline void IsInterior( bool b ) { interior_ = b; }

	Triangle& NeighborAcross(Point& opoint);

	Triangle( const Triangle& tri )
	{
		*this = tri;
		int i = 0;
	}

	Triangle& operator = ( const Triangle& rhs )
	{
		points_[0] = rhs.points_[0];
		points_[1] = rhs.points_[1];
		points_[2] = rhs.points_[2];

		neighbors_[0] = rhs.neighbors_[0];
		neighbors_[1] = rhs.neighbors_[1];
		neighbors_[2] = rhs.neighbors_[2];

		constrained_edge[0] = rhs.constrained_edge[0];
		constrained_edge[1] = rhs.constrained_edge[1];
		constrained_edge[2] = rhs.constrained_edge[2];

		delaunay_edge[0] = rhs.delaunay_edge[0];
		delaunay_edge[1] = rhs.delaunay_edge[1];
		delaunay_edge[2] = rhs.delaunay_edge[2];
		interior_ = rhs.interior_;

		return *this;
	}

//void DebugPrint();

private:

	/// Triangle points
	Point* points_[3];
	/// Neighbor list
	Triangle* neighbors_[3];

public:
	/// Flags to determine if an edge is a Constrained edge
	bool constrained_edge[3];
	/// Flags to determine if an edge is a Delauney edge
	bool delaunay_edge[3];

private:
	/// Has this triangle been marked as an interior triangle?
	bool interior_;
};

inline int cmp(const Point* a, const Point* b)
{
  if (a->pos.y < b->pos.y) {
    return -1;
  } else if (a->pos.y == b->pos.y) {
    // Make sure q is point with greater x value
	if( a->pos.x < b->pos.x ) return -1;
    if( a->pos.x == b->pos.x ) return 0;
  }
  return 1;
}
/*
inline bool cmp(const Point* a, const Point* b)
{
  if (a->pos.y < b->pos.y) {
    return true;
  } else if (a->pos.y == b->pos.y) {
    // Make sure q is point with greater x value
    if (a->pos.x < b->pos.x) {
      return true;
    }
  }
  return false;
}
*/

inline bool Triangle::Contains(Point* p)
{
  return p == points_[0] || p == points_[1] || p == points_[2];
}

inline bool Triangle::Contains(const Edge& e)
{
  return Contains(e.p) && Contains(e.q);
}

inline bool Triangle::Contains(Point* p, Point* q)
{
  return Contains(p) && Contains(q);
}

}

#endif


