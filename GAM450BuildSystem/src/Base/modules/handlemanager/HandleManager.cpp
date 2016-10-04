/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: HandleManager.cpp 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#include "HandleManager.h"
#include "Base/modules/handle/Handle.h"

#include <assert.h>

namespace Base
{
  HandleManager::HandleManager() : m_freeSlots(m_MaxEntries), m_freeList(0)
  {
    Clear();
  }

  HandleManager::HandleEntry::HandleEntry()
    : m_nextFreeIndex(0), m_counter(0), m_endOfList(false), m_allocated(false), m_ptr(nullptr) {}

  HandleManager::HandleEntry::HandleEntry(unsigned nextFreeIndex)
    : m_nextFreeIndex(nextFreeIndex), m_counter(0), m_endOfList(false), m_allocated(false), m_ptr(nullptr) {}

  void HandleManager::Clear()
  {
    // This links all of the free slots together, sort of like a linked list
    for (unsigned int i = 0; i < m_MaxEntries - 1; ++i)
      m_array[i] = HandleEntry(i + 1);

    // Sets the last HandleEntry to be the end of the list
    m_array[m_MaxEntries - 1].m_endOfList = true;

    m_freeSlots = m_MaxEntries;
  }

  Handle HandleManager::Insert(void* ptr)
  {
    // Get the next free entry in the array
    HandleEntry* entry = m_array + m_freeList;

    // If we are out of entries, assert
    assert(entry->m_endOfList != true);

    // If the entry we got is allocated already
    // then there is some sort of bug
    assert(entry->m_allocated == false);

    entry->m_allocated = true; // We are allocating to this entry

    // Increment the freeList
    unsigned int index = m_freeList;
    m_freeList = m_array[m_freeList].m_nextFreeIndex;

    // Insert the ptr into the entry
    entry->m_ptr = ptr;

    --m_freeSlots;

    return Handle(index, entry->m_counter);
  }

  void HandleManager::Replace(void* ptr, Handle& handle)
  {
    HandleEntry* entry = m_array + handle.m_index;

    // Increment the uid counter to signify a new handle at this entry
    if (entry->m_allocated == true)
    {
      entry->m_counter += 1;
      handle.m_counter = entry->m_counter;
    }

    entry->m_ptr = ptr;
  }

  void HandleManager::Update(void* ptr, Handle& handle)
  {
    // Check if the handle is invalid
    assert(handle != Handle::null);

    // Get the handle entry
    HandleEntry* entry = m_array + handle.m_index;

    if (!entry || !entry->m_allocated || entry->m_counter != handle.m_counter)
      return;
    //assert(entry->m_allocated == true);

    // Set the entries pointer
    entry->m_ptr = ptr;
  }

  void HandleManager::Remove(Handle handle)
  {
    HandleEntry* entry = m_array + handle.m_index;

    // Ensure that the entry was actually allocated
    assert(entry->m_allocated == true);

    entry->m_counter += 1;
    entry->m_allocated = false;

    // Push removed slot onto the freeList
    entry->m_nextFreeIndex = m_freeList;
    m_freeList = handle.m_index;

    ++m_freeSlots;
  }

  void* HandleManager::Get(Handle handle) const
  {
    const HandleEntry* entry = m_array + handle.m_index;

    if (entry->m_allocated == true && entry->m_counter == handle.m_counter)
      return entry->m_ptr;

    return nullptr;
  }

  bool HandleManager::Exists(Handle handle) const
  {
    const HandleEntry *entry = m_array + handle.m_index;

    if (entry->m_counter == handle.m_counter && entry->m_allocated == true)
      return true;

    return false;
  }

  void HandleManager::Run(void(*runFunc)(void* ptr))
  {
    for (unsigned int i = 0; i < m_MaxEntries - 1; ++i)
    {
      if (m_array[i].m_allocated)
        runFunc(m_array[i].m_ptr);
    }
  }
}