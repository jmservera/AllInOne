#pragma once

namespace GraphicsEngineSpace {

	public ref class _Texture2D:public _Texture
	{
	private:

		ID3D11Texture2D* m_TemporalTextureForData;

	internal:

		void GetDescription();
		void UpdateSQLInfo(String^ cad);
		void Resize(ID3D11Texture2D* pBackBuffer);

		D3D11_TEXTURE2D_DESC* m_TextureDescription;
		ID3D11Texture2D*	m_TextureInferface;
	public:

		_Texture2D(GraphicsDevice^, String^ name);
		_Texture2D(GraphicsDevice^, String^ name, ID3D11Texture2D* pBackBuffer);
		virtual void Destroy() override;

		IntPtr GetTexturePtr()
		{
			return IntPtr(m_TextureInferface);
		}

		void LoadFromFile(String^ fullpath);
		void Save(String^ fullPath);

		void Create(int width,int height,DXGIFORMAT format,bool renderTarget);

		property DXGIFORMAT Format { DXGIFORMAT get() { return (DXGIFORMAT)m_TextureDescription->Format; } }

		property int Width { int get() { return m_TextureDescription->Width; } }
		property int Height { int get() { return m_TextureDescription->Height; } }

		virtual void SetToDevice(UINT32 channel) override;

		BYTE* MapData([Out] int% rowPitch,bool canWrite);
		void Unmap() { Unmap(false); }
		void Unmap(bool copyBack);
	};

}
