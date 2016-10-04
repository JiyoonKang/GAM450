/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: HandleManager.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Base/modules/handle/Handle.h"

namespace Base
{
  class HandleManager
  {
  public:
    HandleManager();

    void Clear(void);
    Handle Insert(void* ptr);
    void Replace(void* ptr, Handle& handle);
    void Update(void* ptr, Handle& handle);
    void Remove(Handle handle);

    void Run(void(*runFunc)(void* ptr));

    void* Get(Handle handle) const;
    bool Exists(Handle handle) const;

    template <typename T>
    T* GetAs(Handle handle) const;

    inline unsigned FreeSlots(void) const { return m_freeSlots; }

    template<typename T>
    void SyncHandles(ObjectAllocator& allocator, bool force = false);

    static const int m_MaxEntries = 16384; // 2^14

  private:
    struct HandleEntry
    {
      HandleEntry();
      HandleEntry(unsigned nextFreeIndex);

      unsigned m_nextFreeIndex : 16;
      unsigned m_counter : 16;
      unsigned m_endOfList : 1;
      unsigned m_allocated : 1;
      void* m_ptr;
    };

    unsigned m_freeSlots;
    unsigned m_freeList;
    HandleEntry m_array[m_MaxEntries];
  };

  //////////////////////////////////////////////////////////////////////////

  template <typename T>
  T* HandleManager::GetAs(Handle handle) const
  {
    if (handle == Handle::null)// && handle.m_index < m_MaxEntries)
      return nullptr;

    // Get the handle entry from the array
    if (handle.m_index < m_MaxEntries)
    {
      const HandleEntry* entry = m_array + handle.m_index;

      if (entry->m_counter == handle.m_counter && entry->m_allocated == true)
      {
        return (T*)entry->m_ptr;
      }
    }
    return nullptr;
  }

  // Updates all handles to ensure that the HandleManager has accurate
  // pointers to the data which the handle needs to point at

  template <typename T>
  void HandleManager::SyncHandles(ObjectAllocator& m_allocator, bool force)
  {
    if (m_allocator.Grew() || force)
    {
      // Update the handle manager and tell it the new location of the components
      for (auto i = m_allocator.begin<T>(); i != m_allocator.end<T>(); ++i)
      {
        Update(&(*i), i->self);
      }

      m_allocator.ClearGrewFlag();
    }
  }
}
