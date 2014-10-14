#pragma once

namespace GraphicsEngineCommonSpace {

	public enum class DEPTHWRITEMASK
    {	
		ZERO	= 0,
		ALL	= 1
    };

	public enum class COMPARISONFUNC
	{	
		NEVER	= 1,
		LESS	= 2,
		EQUAL	= 3,
		LESS_EQUAL	= 4,
		GREATER	= 5,
		NOT_EQUAL	= 6,
		GREATER_EQUAL	= 7,
		ALWAYS	= 8
	};

	public enum class STENCILOP
    {
		KEEP	= 1,
		ZERO	= 2,
		REPLACE	= 3,
		INCR_SAT	= 4,
		DECR_SAT	= 5,
		INVERT	= 6,
		INCR	= 7,
		DECR	= 8
    };

	public ref class DepthStencipOpDescription
    {
	public:
        STENCILOP StencilFailOp;
        STENCILOP StencilDepthFailOp;
        STENCILOP StencilPassOp;
        COMPARISONFUNC StencilFunc;
    };

    public ref class DepthStencilDescription
    {
	public:
        bool DepthEnable;
        DEPTHWRITEMASK DepthWriteMask;
        COMPARISONFUNC DepthFunc;
        bool StencilEnable;
        Byte StencilReadMask;
        Byte StencilWriteMask;
        DepthStencipOpDescription FrontFace;
        DepthStencipOpDescription BackFace;

		DepthStencilDescription();

		int InternalIndex;

		void Changed() { InternalIndex=-1; }
	};
}

