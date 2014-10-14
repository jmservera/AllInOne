#pragma once

namespace GraphicsEngineSpace {

	public ref class SQL
	{
	private:

		static OleDbConnection^ m_Connection;

	public:

		static void Open(String^ path);

		static void Insert(String^ sql);

		static void Destroy();

		static String^ ToString(String^ sql);
	};
}
