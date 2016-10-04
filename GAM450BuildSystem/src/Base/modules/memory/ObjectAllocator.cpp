/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ObjectAllocator.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "ObjectAllocator.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

namespace Base
{
  ObjectAllocator::ObjectAllocator()
    : m_array(nullptr), m_objectSize(1), m_size(0), m_capacity(0), m_grew(false) {}

  ObjectAllocator::ObjectAllocator(unsigned int objectSize)
    : m_array(nullptr), m_objectSize(objectSize), m_size(0), m_capacity(0), m_grew(false) {}

  ObjectAllocator::ObjectAllocator(unsigned int objectSize, unsigned int numElements)
    : m_array(malloc(objectSize * numElements)), m_objectSize(objectSize), m_size(0),
      m_capacity(numElements), m_grew(false) {}

  void ObjectAllocator::Initialize(unsigned int objectSize, unsigned int numElements)
  {
    m_capacity = numElements;
    m_objectSize = objectSize;
    m_size = 0;

    free(m_array);

    m_array = malloc(objectSize * numElements);
  }

	void* ObjectAllocator::Allocate()
	{
		if (m_size == m_capacity)
			Grow();

    return (char*)m_array + m_size++ * m_objectSize;
	}

	void* ObjectAllocator::Free(void* data)
	{
		unsigned int index = GetIndex(data);

		// Rightmost now goes to the empty slot
		if (--m_size)
		{
			Move(index, m_size);

			return (char*)m_array + index * m_objectSize;
		}

		return nullptr;
	}

	void ObjectAllocator::Grow()
	{
		m_capacity = m_capacity * 3 + 10;

		m_grew = true;

		GrowInteral();
	}

	void ObjectAllocator::GrowInteral()
	{
		// Allocates a new array
		void* new_array = calloc(m_objectSize, m_capacity);

		// Copies over all of the data from the old array into the new
		memcpy(new_array, m_array, m_size * m_objectSize);

		// Free's the old array
		free(m_array);

		// Updates the pointers
		m_array = new_array;
	}

	void ObjectAllocator::Move(unsigned int a, unsigned int b)
	{
		void* A = (char*)m_array + a * m_objectSize;
		void* B = (char*)m_array + b * m_objectSize;
		memcpy(A, B, m_objectSize);
	}

	void ObjectAllocator::Shrink()
	{
		m_capacity = m_size;

		GrowInteral();
	}

	void* ObjectAllocator::operator[](unsigned int index)
	{
		assert(index < m_size);
		return (char*)m_array + index * m_objectSize;
	}

	const void* ObjectAllocator::operator[](unsigned int index) const
	{
		assert(index < m_size);
		return (char*)m_array + index * m_objectSize;
	}

	unsigned int ObjectAllocator::GetIndex(void* data) const
	{
		return ((char*)data - (char*)m_array) / m_objectSize;
	}

	void ObjectAllocator::ClearGrewFlag(void)
	{
		m_grew = false;
	}

  void ObjectAllocator::Clear()
  {
    m_capacity = 0;
    m_size = 0;
    m_objectSize = 0;

    if (m_array)
      free(m_array);
    
    m_array = nullptr;
  }

  ObjectAllocator::~ObjectAllocator()
  {
    Clear();
  }
}