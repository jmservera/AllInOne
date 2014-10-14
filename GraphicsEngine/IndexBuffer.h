#pragma once

namespace GraphicsEngineSpace {

	public ref class _IndexBuffer:_Buffer 
	{

	private:

		UInt32*					m_Indices;
		int						m_Count;

	internal:

		property	int Count							{ int get() { return m_Count; } }

	public:

		bool					Use32BitIndices;

	public:

		_IndexBuffer(GraphicsDevice^);

		void					BindIndices(UInt32 *indices,int count);
		virtual void			SetToDevice(int offsetToFirstIndex) override;

		void					Update();

	private:

	};
}
