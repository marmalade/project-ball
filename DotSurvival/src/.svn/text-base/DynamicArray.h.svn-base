#pragma once

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

//#include "wzxSYS/wzx_Obj.h"
//#include "wzxSYS/wzx_Mem.h"
//#include "wzxSYS/wzx_Debug.h"
#include "string.h"
#include "src/utils.h"

typedef int int_t;
typedef uint32 uint_t;

template <class ItemData> class DynamicArray
{
//static const uint_t _nDefaultSize = N;

public:
	// N - initial array size
	DynamicArray( int_t N = 8 ) : m_nUsedCnt( 0 )
	{
		m_Data = new ItemData[N];
		m_nAllocatedCnt = N;
	}

	// copy construct
	DynamicArray( const DynamicArray& dynArr )
	{
		*this = dynArr;
	}

	~DynamicArray()
	{
		delete[] m_Data; m_Data = NULL;
	}

	DynamicArray& operator = ( const DynamicArray<ItemData>& rhs )
	{
		// do not allow self assignment
		wzx_ASSERT( &rhs != this );

		if( m_nAllocatedCnt < rhs.Size() ) ReallocNonCopy( rhs.Size() );
		m_nUsedCnt = rhs.Size();
		wzx_MemCopy( rhs.begin(), m_Data, sizeof(ItemData) * m_nUsedCnt );
		return *this;
	}

	int_t push_back( const ItemData& item ) { return Add(item); }
	int_t Add( const ItemData& item )
	{
		if( m_nUsedCnt >= m_nAllocatedCnt )
		{
			Realloc( (m_nAllocatedCnt*3)/2 + 1 );
		}
		m_Data[m_nUsedCnt++] = item;
		return ( m_nUsedCnt-1 );
	}

	void RemoveLast()
	{
		m_nUsedCnt--;
	}

	void Clear() { m_nUsedCnt = 0; }

	int_t Last() const { return m_nUsedCnt-1; }
	int_t Size() const { return m_nUsedCnt; }

	int_t AllocatedSize() const { return m_nAllocatedCnt; }

	void Realloc( uint_t newSize )
	{
		ItemData* newDataArr = new ItemData[newSize];
		wzx_ASSERT( newDataArr );
		wzx_MemCopy( m_Data, newDataArr, sizeof(ItemData) * m_nUsedCnt );
		delete[] m_Data;
		m_Data = newDataArr;
		m_nAllocatedCnt = newSize;
	}

	void ReallocNonCopy( uint_t newSize )
	{
		ItemData* newDataArr = new ItemData[newSize];
		wzx_ASSERT( newDataArr );
		delete[] m_Data;
		m_Data = newDataArr;
		m_nAllocatedCnt = newSize;
	}

	int_t Find( const ItemData& item )
	{
		for( int_t i = 0; i < m_nUsedCnt; i++ )
			if( m_Data[i] == item ) return i;

		return -1;
	}

	ItemData& operator [] ( int_t id )
	{
		wzx_ASSERT( id < m_nAllocatedCnt );
		return m_Data[id];
	}

	const ItemData& operator [] ( int_t id ) const
	{
		wzx_ASSERT( id < m_nAllocatedCnt );
		return m_Data[id];
	}

	ItemData* begin() { return m_Data; }
	ItemData* end() { return m_Data+m_nUsedCnt; }

	const ItemData* begin() const { return m_Data; }


private:
	ItemData* m_Data;
	int_t m_nUsedCnt;
	int_t m_nAllocatedCnt;
};


#endif // DYNAMIC_ARRAY_H
