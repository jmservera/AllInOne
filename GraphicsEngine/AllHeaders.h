#define SAFE_DESTROY(a) if(a!=nullptr) { a->Destroy(); a=nullptr; }

#include "Log.h"
using namespace System::Runtime::InteropServices;
using namespace System::Data::OleDb;

using namespace AIOEngine::MathSpace;

using namespace AIOEngine::Graphics;

namespace GraphicsEngineSpace
{
ref class InputLayout;
ref class GraphicsDevice;

extern Log InternalLog;
extern Log FatalLog;
}

#include "AllHeadersCommon.h"
#include "Math.h"

using namespace GraphicsEngineCommonSpace;

#include "SQL.h"

#include "FastStrCat.h"

#include "Timer.h"

#include "Buffer.h"

#include "IndexBuffer.h"

#include "VertexBuffer.h"

#include "InputLayout.h"

#include "Texture.h"
#include "Texture2D.h"

#include "Shader.h"

#include "RenderTarget.h"

#include "DepthStencil.h"

#include "DeviceStates.h"

#include "MeshBasic.h"

#include "GraphicsDevice.h"

#include "Mouse.h"
