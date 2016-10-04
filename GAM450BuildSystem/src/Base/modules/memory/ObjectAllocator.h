/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: ObjectAllocator.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include <xutility>

namespace Base
{
	class ObjectAllocator
	{
	public:
    ObjectAllocator();

		ObjectAllocator(unsigned int objectSize);

		ObjectAllocator(unsigned int objectSize, unsigned int numElements);

    ~ObjectAllocator();

		// Allocate a single object, returns a pointer to the object, does not call new
		void* Allocate();

		template <typename T>
		inline T* begin(void) { return (T*)m_array; }
		template <typename T>
		inline T* end(void) { return (T*)m_array + m_size; }

		// Move rightmost element into deleted slot, does not placement delete
		void* Free(void* memory); // Returns pointer to the moved element (was rightmost element before free)

    void Clear();

		// Shrink internal array to exactly number of elements
		void Shrink(void);

		void* operator[](unsigned index);
		const void* operator[](unsigned index) const;
    inline unsigned Size(void) const { return m_size; }// Number of allocated elements
    inline unsigned Capacity(void) const { return m_capacity; } // Number of elements total
    inline bool Grew(void) const { return m_grew; }
		unsigned GetIndex(void* data) const;
		void ClearGrewFlag(void);
    void Initialize(unsigned int objectSize, unsigned int numElements);

	private:
		void* m_array;          // Pointer to the array
		unsigned m_objectSize;  // The size of individual objects
		unsigned m_size;        // How many elements there are in the array
		unsigned m_capacity;    // The capacity of the array
		bool m_grew;            // If the array was grown

		// Grow internal array
		void Grow();
		void GrowInteral();

		// Move data from one index to another
		void Move(unsigned int a, unsigned int b);
	};
}