#pragma once

namespace GraphicsEngineSpace {
 /*
	public enum class D3D11BLEND
    {	
		D3D11_BLEND_ZERO	= 1,
		D3D11_BLEND_ONE	= 2,
		D3D11_BLEND_SRC_COLOR	= 3,
		D3D11_BLEND_INV_SRC_COLOR	= 4,
		D3D11_BLEND_SRC_ALPHA	= 5,
		D3D11_BLEND_INV_SRC_ALPHA	= 6,
		D3D11_BLEND_DEST_ALPHA	= 7,
		D3D11_BLEND_INV_DEST_ALPHA	= 8,
		D3D11_BLEND_DEST_COLOR	= 9,
		D3D11_BLEND_INV_DEST_COLOR	= 10,
		D3D11_BLEND_SRC_ALPHA_SAT	= 11,
		D3D11_BLEND_BLEND_FACTOR	= 14,
		D3D11_BLEND_INV_BLEND_FACTOR	= 15,
		D3D11_BLEND_SRC1_COLOR	= 16,
		D3D11_BLEND_INV_SRC1_COLOR	= 17,
		D3D11_BLEND_SRC1_ALPHA	= 18,
		D3D11_BLEND_INV_SRC1_ALPHA	= 19
    };

 
	public enum class D3D11BLEND_OP
    {	
		D3D11_BLEND_OP_ADD	= 1,
		D3D11_BLEND_OP_SUBTRACT	= 2,
		D3D11_BLEND_OP_REV_SUBTRACT	= 3,
		D3D11_BLEND_OP_MIN	= 4,
		D3D11_BLEND_OP_MAX	= 5
    };

 
	public enum class D3D11COLOR_WRITE_ENABLE
    {	
		D3D11_COLOR_WRITE_ENABLE_RED	= 1,
		D3D11_COLOR_WRITE_ENABLE_GREEN	= 2,
		D3D11_COLOR_WRITE_ENABLE_BLUE	= 4,
		D3D11_COLOR_WRITE_ENABLE_ALPHA	= 8,
		D3D11_COLOR_WRITE_ENABLE_ALL	= ( ( ( D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN )  | D3D11_COLOR_WRITE_ENABLE_BLUE )  | D3D11_COLOR_WRITE_ENABLE_ALPHA ) 
    } ;


    public ref class D3D11RENDER_TARGET_BLEND_DESC
    {
	public:
        bool BlendEnable;
        D3D11BLEND SrcBlend;
        D3D11BLEND DestBlend;
        D3D11BLEND_OP BlendOp;
        D3D11BLEND SrcBlendAlpha;
        D3D11BLEND DestBlendAlpha;
        D3D11BLEND_OP BlendOpAlpha;
        D3D11COLOR_WRITE_ENABLE RenderTargetWriteMask;
    };

    public ref class BlendDescription
    {
	public:

		bool AlphaToCoverageEnable;
        bool IndependentBlendEnable;

        List<D3D11RENDER_TARGET_BLEND_DESC^>^ RenderTarget;

		BlendDescription();
    };

	*/
	public ref class _BlendingState
	{

	internal:
		GraphicsDevice^				m_Device;

		D3D11_BLEND_DESC *m_Blending;
		ID3D11BlendState *m_State;

		static void Convert(BlendDescription^ item,D3D11_BLEND_DESC*);

	public:

		_BlendingState(GraphicsDevice^);
		~_BlendingState();
		void Destroy();
		void SetToDevice(float* BlendFactor,UINT32 sampleMask);
		void Create(D3D11_BLEND_DESC *dest);
		bool CompareIftheSame(D3D11_BLEND_DESC *dest);

	};

	public ref class _RasterizerState
	{

	private:
		GraphicsDevice^				m_Device;

		ID3D11RasterizerState *m_State;

	public:

		D3D11_RASTERIZER_DESC *m_Rasterizer;

		_RasterizerState(GraphicsDevice^);
		~_RasterizerState();
		void CreateDefault();
		void CreateCullNone();
		void Destroy();
		void Create();
		void Create(D3D11_RASTERIZER_DESC *dest);
		bool CompareIftheSame(D3D11_RASTERIZER_DESC *dest);
		void SetToDevice();
	};
	/*
	public enum class D3D11DEPTH_WRITE_MASK
    {	
		D3D11_DEPTH_WRITE_MASK_ZERO	= 0,
		D3D11_DEPTH_WRITE_MASK_ALL	= 1
    };

	public enum class D3D11COMPARISON_FUNC
	{	
		D3D11_COMPARISON_NEVER	= 1,
		D3D11_COMPARISON_LESS	= 2,
		D3D11_COMPARISON_EQUAL	= 3,
		D3D11_COMPARISON_LESS_EQUAL	= 4,
		D3D11_COMPARISON_GREATER	= 5,
		D3D11_COMPARISON_NOT_EQUAL	= 6,
		D3D11_COMPARISON_GREATER_EQUAL	= 7,
		D3D11_COMPARISON_ALWAYS	= 8
	};

	public enum class D3D11STENCIL_OP
    {
		D3D11_STENCIL_OP_KEEP	= 1,
		D3D11_STENCIL_OP_ZERO	= 2,
		D3D11_STENCIL_OP_REPLACE	= 3,
		D3D11_STENCIL_OP_INCR_SAT	= 4,
		D3D11_STENCIL_OP_DECR_SAT	= 5,
		D3D11_STENCIL_OP_INVERT	= 6,
		D3D11_STENCIL_OP_INCR	= 7,
		D3D11_STENCIL_OP_DECR	= 8
    };

	public ref class D3D11DEPTH_STENCILOP_DESC
    {
	public:
        D3D11STENCIL_OP StencilFailOp;
        D3D11STENCIL_OP StencilDepthFailOp;
        D3D11STENCIL_OP StencilPassOp;
        D3D11COMPARISON_FUNC StencilFunc;
    };

    public ref class DepthStencilDescription
    {
	public:
        bool DepthEnable;
        D3D11DEPTH_WRITE_MASK DepthWriteMask;
        D3D11COMPARISON_FUNC DepthFunc;
        bool StencilEnable;
        byte StencilReadMask;
        byte StencilWriteMask;
        D3D11DEPTH_STENCILOP_DESC FrontFace;
        D3D11DEPTH_STENCILOP_DESC BackFace;

		DepthStencilDescription();
    };
	*/
	public ref class _DepthStencilState
	{

	private:
		GraphicsDevice^				m_Device;

		D3D11_DEPTH_STENCIL_DESC *m_DepthStencil;
		ID3D11DepthStencilState *m_State;
		
		void Create();

	public:

		_DepthStencilState(GraphicsDevice^);
		~_DepthStencilState();
		void Destroy();
		void SetToDevice(UINT);
		void Create(D3D11_DEPTH_STENCIL_DESC *dest);
		bool CompareIftheSame(D3D11_DEPTH_STENCIL_DESC *dest);
		void BuildDefault_Depth(bool enabled);
	};


	public ref class _SamplerState
	{
	private:
		GraphicsDevice^				m_Device;
		D3D11_SAMPLER_DESC			*m_Sampler;
		ID3D11SamplerState*			m_State;

	internal:

	public:

		_SamplerState(GraphicsDevice^ device);
		~_SamplerState();
		void Destroy();
		void Create(D3D11_SAMPLER_DESC *dest);
		void CreateDefault();
		bool CompareIftheSame(D3D11_SAMPLER_DESC *dest);
		void SetToDevice(UINT channel);
	};}
