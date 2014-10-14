#include "stdafx.h"

#include "AllHeadersCommon.h"

#define D3D11_FLOAT32_MAX	( 3.402823466e+38f )

namespace GraphicsEngineCommonSpace {

	SamplerDescription::SamplerDescription()
	{
		Filter = FILTER::MIN_MAG_MIP_LINEAR;
		AddressU = TEXTUREADDRESSMODE::CLAMP;
		AddressV = TEXTUREADDRESSMODE::CLAMP;
		AddressW = TEXTUREADDRESSMODE::CLAMP;
		MinLOD = -D3D11_FLOAT32_MAX;
		MaxLOD = D3D11_FLOAT32_MAX;
		MipLODBias=0;
		MaxAnisotropy=16;
		ComparisonFunc = COMPARISONFUNC::NEVER;
		BorderColor0=0;
		BorderColor1=0;
		BorderColor2=0;
		BorderColor3=0;

		InternalIndex=-1;
	}

}