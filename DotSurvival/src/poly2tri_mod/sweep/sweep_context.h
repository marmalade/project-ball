#pragma once

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

#ifndef SWEEP_CONTEXT_H
#define SWEEP_CONTEXT_H

#include "Src/DynamicArray.h"

//#include <list>
//#include <vector>


namespace p2t {

// Inital triangle factor, seed triangle will extend 30% of
// PointSet width to both left and right.
const float kAlpha = 0.3f;

struct Point;
class Triangle;
struct Node;
struct Edge;
class AdvancingFront;

class SweepContext
{
public:

	struct Basin
	{
		Node* left_node;
		Node* bottom_node;
		Node* right_node;
		float width;
		bool left_highest;

		Basin()
			: left_node( NULL ), bottom_node( NULL ), right_node( NULL )
			, width( 0.0f ), left_highest( false )
		{}

		void Clear()
		{
			left_node = NULL;
			bottom_node = NULL;
			right_node = NULL;
			width = 0.0;
			left_highest = false;
		}
	};

	struct EdgeEvent
	{
		Edge* constrained_edge;
		bool right;

		EdgeEvent() : constrained_edge(NULL), right(false) {}

		void Clear() { constrained_edge = NULL; right = false; }
	};


public:
	SweepContext();
	~SweepContext();

	void Clear();

	void SetPolyLine( const DynamicArray<Point*>& polyline );
	void AddHole( const DynamicArray<Point*>& polyline );
	void AddPoint( Point* point ) { points_.push_back( point ); }


	void set_head( Point* p1 ) { head_ = p1; }
	void set_tail( Point* p1 ) { tail_ = p1; }
	Point* head() { return head_; }
	Point* tail() { return tail_; }

	AdvancingFront* front() { return front_; }
	int_t PointsCnt() const { return points_.Size(); }

	Node& LocateNode( Point& point );
	void RemoveNode( Node* node );

	void CreateAdvancingFront(/*std::vector<Node*> nodes*/);

	/// Try to map a node to all sides of this triangle that don't have a neighbor
	void MapTriangleToNodes( Triangle& t );

	//int_t AddToMap(Triangle* triangle)
	int_t AddToMap( const Triangle& triangle )
	{
		return map_.push_back( triangle );
	}

	Point* GetPoint( const int_t index ) { return points_[index]; }
	Point* GetPoints();

	//void RemoveFromMap(Triangle* triangle);

	void MeshClean( Triangle& triangle );

	const DynamicArray<Triangle>& GetTriangles() const { return triangles_; }
	const DynamicArray<Triangle>& GetMap() const { return map_; }


public:
	DynamicArray<Edge*> edge_list;

	Basin basin;
	EdgeEvent edge_event;


private:
	friend class Sweep;

	DynamicArray<Triangle> triangles_;
	DynamicArray<Triangle> map_;
	DynamicArray<Point*> points_;

	// Advancing front
	AdvancingFront* front_;
	// head point used with advancing front
	Point* head_;
	// tail point used with advancing front
	Point* tail_;

	Node *af_head_, *af_middle_, *af_tail_;

private:
	void InitTriangulation();
	void InitEdges( const DynamicArray<Point*>& polyline );

	Triangle* GetTriFromMap( int_t idx ) { return &map_[idx]; }
};

}

#endif
