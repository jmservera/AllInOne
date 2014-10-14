#pragma once

namespace GraphicsEngineSpace {

	public enum class _D3D11_BIND_FLAG
    {	
		D3D11_BIND_VERTEX_BUFFER	= 0x1L,
		D3D11_BIND_INDEX_BUFFER	= 0x2L,
		D3D11_BIND_CONSTANT_BUFFER	= 0x4L,
		D3D11_BIND_SHADER_RESOURCE	= 0x8L,
		D3D11_BIND_STREAM_OUTPUT	= 0x10L,
		D3D11_BIND_RENDER_TARGET	= 0x20L,
		D3D11_BIND_DEPTH_STENCIL	= 0x40L,
		D3D11_BIND_UNORDERED_ACCESS	= 0x80L
    };

	public ref class _Buffer 
	{

	protected:

		GraphicsDevice^			m_Device;
		ID3D11Buffer*			m_Buffer;
		D3D11_BUFFER_DESC*		m_Description;

		int						m_BufferSizeInBytes;
		int						m_BufferSizeForAddData;

		char*					m_Data;

		static const int MaxData=4096;

		bool					m_Dynamic;

		void					Initialize(GraphicsDevice^ device,bool dynamic);

	internal:

		property GraphicsDevice^ Device	{ GraphicsDevice^ get() { return m_Device; } }	

		property int BufferSizeInBytes				{ int get() { return m_BufferSizeInBytes; } }

	public:

		_Buffer(GraphicsDevice^);
		_Buffer(GraphicsDevice^,bool dynamic);
		~_Buffer();

		virtual void	Destroy();

		virtual void SetToDevice(int numBuffer);

		void Create(_D3D11_BIND_FLAG,int);

		void UpdateData(void *data);

		void BeginAddData();
		void AddMatrix(Matrix% data);
		void Add(Vector4% data);
		void Add(Vector3% data);
		void AddFloat(float data);
		void AddData(void *data,int dataSize);
		void EndAddData();

	protected:

		HRESULT					Create(void *,D3D11_BIND_FLAG);
	};
}
