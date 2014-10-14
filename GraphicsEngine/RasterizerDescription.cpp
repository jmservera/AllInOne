#include "stdafx.h"

#include "AllHeadersCommon.h"

namespace GraphicsEngineCommonSpace {

	RasterizerDescription::RasterizerDescription()
	{
		FillMode = FILLMODE::SOLID;
		CullMode = CULLMODE::BACK;
		FrontCounterClockwise = false;
		DepthBias = 0;
		DepthBiasClamp = 0;
		SlopeScaledDepthBias = 0;
		DepthClipEnable = true;
		ScissorEnable = false;
		MultisampleEnable = false;
		AntialiasedLineEnable = false;

		InternalIndex=-1;
	}
}