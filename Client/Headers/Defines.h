#pragma once

#pragma warning (disable : 4251)

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#ifdef _DEBUG 
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG 

#define CREATE_OBJ

//#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>  



 

#include "Constant.h"
#include "Struct.h"

extern HWND g_hWnd;
extern int		g_iScene;
extern bool		g_bShowingCollider;
extern bool		g_bUsingStaticCamera;
extern bool		g_bShowingNaviMesh;