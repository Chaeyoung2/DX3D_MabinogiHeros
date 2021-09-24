#pragma once

#pragma warning (disable : 4251)

#include <d3dx9.h>
#include <d3d9.h>

#ifdef _DEBUG 
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG 

#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>  

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <map>
#include <list>
#include <vector>
#include <tuple>
#include <algorithm>
using namespace std;

#include <iostream>
#include <io.h>



#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Struct.h"



using namespace Engine;


