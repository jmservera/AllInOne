#pragma once

namespace GraphicsEngineCommonSpace {
	public enum class BLEND
    {	
		ZERO	= 1,
		ONE	= 2,
		SRC_COLOR	= 3,
		INV_SRC_COLOR	= 4,
		SRC_ALPHA	= 5,
		INV_SRC_ALPHA	= 6,
		DEST_ALPHA	= 7,
		INV_DEST_ALPHA	= 8,
		DEST_COLOR	= 9,
		INV_DEST_COLOR	= 10,
		SRC_ALPHA_SAT	= 11,
		BLEND_FACTOR	= 14,
		INV_BLEND_FACTOR	= 15,
		SRC1_COLOR	= 16,
		INV_SRC1_COLOR	= 17,
		SRC1_ALPHA	= 18,
		INV_SRC1_ALPHA	= 19
    };

 
	public enum class BLENDOP
    {	
		ADD	= 1,
		SUBTRACT	= 2,
		REV_SUBTRACT	= 3,
		MIN	= 4,
		MAX	= 5
    };

 
	public enum class COLORWRITEENABLE
    {	
		RED	= 1,
		GREEN	= 2,
		BLUE	= 4,
		ALPHA	= 8,
		ALL	= ( ( ( RED | GREEN )  | BLUE )  | ALPHA ) 
    } ;


    public ref class RenderTargetBlendDescription
    {
	public:
        bool BlendEnable;
        BLEND SrcBlend;
        BLEND DestBlend;
        BLENDOP BlendOp;
        BLEND SrcBlendAlpha;
        BLEND DestBlendAlpha;
        BLENDOP BlendOpAlpha;
        COLORWRITEENABLE RenderTargetWriteMask;
    };

    public ref class BlendDescription
    {
	public:

		bool AlphaToCoverageEnable;
        bool IndependentBlendEnable;

        List<RenderTargetBlendDescription^>^ RenderTarget;

		BlendDescription();

		int InternalIndex;

		void Changed() { InternalIndex=-1; }
	};

}
