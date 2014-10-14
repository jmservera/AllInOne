#include "stdafx.h"

#include "AllHeadersCommon.h"

namespace GraphicsEngineCommonSpace {

	DepthStencilDescription::DepthStencilDescription()
	{
		DepthEnable = true;
		DepthWriteMask = DEPTHWRITEMASK::ALL;
		DepthFunc = COMPARISONFUNC::LESS;
		StencilEnable = false;
		StencilReadMask = 0xff;	 // D3D11_DEFAULT_STENCIL_READ_MASK;
		StencilWriteMask = 0xff; // D3D11_DEFAULT_STENCIL_WRITE_MASK;
		FrontFace.StencilFunc = COMPARISONFUNC::ALWAYS;
		FrontFace.StencilPassOp = STENCILOP::KEEP;
		FrontFace.StencilFailOp = STENCILOP::KEEP;
		FrontFace.StencilDepthFailOp = STENCILOP::KEEP;
		BackFace.StencilFunc = COMPARISONFUNC::ALWAYS;
		BackFace.StencilPassOp = STENCILOP::KEEP;
		BackFace.StencilFailOp = STENCILOP::KEEP;
		BackFace.StencilDepthFailOp = STENCILOP::KEEP;

	InternalIndex=-1;
	}
}
