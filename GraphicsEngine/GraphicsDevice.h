#pragma once

namespace GraphicsEngineSpace {
	public value struct _DefaultStates
	{
	internal:
		GraphicsDevice^ m_Device;
	public:

		_DepthStencilState^ DepthEnabled;
		_DepthStencilState^ DepthDisabled;
		_SamplerState^ Linear;
		_RasterizerState^ CullingNone;

		void Create();
		void Destroy();
	};
	public ref class GraphicsDevice
	{
	private:
		int							m_BDID;
		ID3D11Device*				m_Device;
		ID3D11DeviceContext*		m_ImmediateContext;
		IDXGIFactory1*				m_Factory;
		
		List<_BlendingState^>		m_BlendingStates;
		List<_RasterizerState^>		m_RasterizerStates;
		List<_DepthStencilState^>	m_DepthStencilStates;
		List<_SamplerState^>		m_SamplerStates;
		List<_InputLayout^>			m_InputLayouts;

		// Used to render a Quad
		_MeshBasic^					m_QuadMesh;
		array<UInt32>^ ib;

		array<Vector3>^ vb;

		array<Vector3>^ normal;

		array<Vector2, 2>^ texCoord;
		Material^					m_QuadMeshMaterial;

		// Temporal Buffer used for DrawLines
		_Buffer^ m_TemporalBuffer;
		_VertexBuffer^ m_TemporalVB;
		_IndexBuffer^ m_TemporalIB;

	internal:
		~GraphicsDevice();

		_DefaultStates				m_DefaultStates;

		List<_Texture^>				m_Textures;

		List<_Shader^>				m_Shaders;

		ID3D11Device*				GetDevice() { return m_Device; };
		ID3D11DeviceContext*		GetInmediateContext() { return m_ImmediateContext; };

	public:

		property int ID { int get() { return m_BDID; }}
		GraphicsDevice();
		_RenderTarget^ CreateSwapChainRenderTarget(String^ name,int hwnd);
		_RenderTarget^ CreateDX9SharedRenderTarget(String^ name,int width, int height);
		void Initialize(String^ deviceName, String^ internalPath);
		void GraphicsDevice::DrawIndexed(UInt32 indexBufferCount, UInt32 StartIndexLocation, int BaseVertexLocation);
		void DrawIndexed(int);
		void DrawLines(array<Vector3>^ arrayOfVertices, Vector4 color, Matrix% world, Matrix% view, Matrix% projection);
		void DrawLines(array<Vector3>^ arrayOfVertices, array<UInt32>^ arrayOfIndices, Vector4 color, Matrix% world, Matrix% view, Matrix% projection);

		void Destroy();
		void DrawQuad(ITexture^ texture);
		void SetBlendingState(BlendDescription^ item,float* blendFactor,UINT32 sampleMask);
		void SetScissorRect(int left, int top, int right, int bottom);

		void Resize(int width,int height);

		void SetRenderTargetAndStencil(_RenderTarget^ renderTarget,_DepthStencil^ depthStencil);
		void SetViewPort(int x,int y,int width,int height,float zmin,float zmax);
		void SetViewPort(int x,int y,int width,int height);
		_InputLayout^ AddInputLayoutIfNeeded(_InputLayout^ layout);

		void SetDeviceState(DeviceState^ device);

		_Shader^ GetShaderByID(UInt32 id);

		_Shader^ Shader;
		_Shader^ RectangleShader;
		_Shader^ PositionColorShader;
		/*
		IntPtr GetSharedTextureForDX9()
		{
			return IntPtr(m_Texture2D11);
		}
		*/
	private:

		HRESULT Init();
	};
}

