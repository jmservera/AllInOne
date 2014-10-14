// stdafx.h: archivo de inclusión de los archivos de inclusión estándar del sistema
// o archivos de inclusión específicos de un proyecto utilizados frecuentemente,
// pero rara vez modificados

#pragma once

#define DIRECTINPUT_VERSION 0x0800

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <windows.h>
#include <d3dx9.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <dxerr.h>
#include <dinput.h>

#include <atlstr.h>	// CString
#include <vector>  
#include <memory>  //declarations of unique_ptr 

using namespace std;

using namespace System;

using namespace System::Collections::Generic;

//#include "RasterizerDescription.h"