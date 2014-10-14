#pragma once

namespace GraphicsEngineSpace {


	public ref class _VertexBuffer :public _Buffer
	{

	private:

		int						m_OffsetForPosition;
		int						m_OffsetForNormal;
		int						m_OffsetForTangent;
		int						m_OffsetForColor;
		int						m_OffsetForTexCoord;

		int						m_VertexSizeInBytes;	// Size of one vertex in bytes

		int						m_NumElements;		// Number of elements (used for the InputLayout)

	internal:

		property int VertexSizeInBytes	 { int get() { return m_VertexSizeInBytes; } }
		property int NumElements	 { int get() { return m_NumElements; } }
		property int NumChannels	 { int get() { return m_NumChannels; } }


		float*					m_Position;
		float*					m_Color;
		float*					m_Normal;
		float*					m_Tangent;
		float*					m_TexCoord;
		int						m_NumChannels;
		int						m_Count;

		internal:

	public:

		_VertexBuffer(GraphicsDevice^,bool dynamic);

		virtual void					SetToDevice(int slot) override;

		void					Update();

		void	BindPosition(float* ,int ,int,int);
		void	BindColor(float*,int,int );
		void	BindNormal(float*,int,int );
		void	BindTangent(float*,int,int );
		void	BindTexCoord(float*,int,int,int );

	private:

		void					ResetValues();
	};
}
