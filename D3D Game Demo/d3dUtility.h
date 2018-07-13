#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <tchar.h>

namespace d3d 
{
	//
	//cleanUp º¯Êý
	//
	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t)
	{
		if (t)
		{
			delete t;
			t = 0;
		}
	}
}