/*******************************************************************************
* All content © 2015 DigiPen (USA) Corporation, all rights reserved.
* Reproduction or disclosure of this file or its contents without the prior
* written consent of DigiPen Institute of Technology is prohibited.
*
* File: Singleton.h
* Author: Austin Jensen
* Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
******************************************************************************/
#pragma once

#include <assert.h>

template <typename T>
class Singleton
{
	static T *ms_Singleton;

public:
	Singleton(void)
	{
		assert(!ms_Singleton);
		//int offset = (int)(T *)1 - (int)(Singleton<T> *)(T *)1;
		//ms_Singleton = (T *)((int)this + offset);
    ms_Singleton = static_cast<T *>(this);
	}

	~Singleton(void)
	{
		assert(ms_Singleton);
		ms_Singleton = 0;
	}
	
	static T& GetSingleton(void)
	{
		assert(ms_Singleton);
		return *ms_Singleton;
	}

	static T *GetSingletonPtr(void)
	{
		return ms_Singleton;
	}
};

template <typename T> T *Singleton<T>::ms_Singleton = 0;