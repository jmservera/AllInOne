#pragma once

namespace GraphicsEngineCommonSpace {

	public enum class FILTER
    {	
		MIN_MAG_MIP_POINT	= 0,
		MIN_MAG_POINT_MIP_LINEAR	= 0x1,
		MIN_POINT_MAG_LINEAR_MIP_POINT	= 0x4,
		MIN_POINT_MAG_MIP_LINEAR	= 0x5,
		MIN_LINEAR_MAG_MIP_POINT	= 0x10,
		MIN_LINEAR_MAG_POINT_MIP_LINEAR	= 0x11,
		MIN_MAG_LINEAR_MIP_POINT	= 0x14,
		MIN_MAG_MIP_LINEAR	= 0x15,
		ANISOTROPIC	= 0x55,
		COMPARISON_MIN_MAG_MIP_POINT	= 0x80,
		COMPARISON_MIN_MAG_POINT_MIP_LINEAR	= 0x81,
		COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT	= 0x84,
		COMPARISON_MIN_POINT_MAG_MIP_LINEAR	= 0x85,
		COMPARISON_MIN_LINEAR_MAG_MIP_POINT	= 0x90,
		COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR	= 0x91,
		COMPARISON_MIN_MAG_LINEAR_MIP_POINT	= 0x94,
		COMPARISON_MIN_MAG_MIP_LINEAR	= 0x95,
		COMPARISON_ANISOTROPIC	= 0xd5
    };


	public enum class TEXTUREADDRESSMODE
    {	
		WRAP	= 1,
		MIRROR	= 2,
		CLAMP	= 3,
		BORDER	= 4,
		MIRROR_ONCE	= 5
    } ;

	public ref class SamplerDescription
    {
	public:
		FILTER Filter;
		TEXTUREADDRESSMODE AddressU;
		TEXTUREADDRESSMODE AddressV;
		TEXTUREADDRESSMODE AddressW;
		float MipLODBias;
		int MaxAnisotropy;
		COMPARISONFUNC ComparisonFunc;
		float BorderColor0;
		float BorderColor1;
		float BorderColor2;
		float BorderColor3;
		float MinLOD;
		float MaxLOD;

		SamplerDescription();

		int InternalIndex;

		void Changed() { InternalIndex=-1; }
    };
}
