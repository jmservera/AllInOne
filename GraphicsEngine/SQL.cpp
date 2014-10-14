#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	void SQL::Destroy()
	{

		if (m_Connection != nullptr)
		{
			m_Connection->Close();

			m_Connection = nullptr;
		}
	}

	void SQL::Open(String^ path)
	{

		m_Connection = nullptr;

		try
		{
			m_Connection = gcnew OleDbConnection("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + path + "\\Log\\database.accdb;Persist Security Info = False;");

			m_Connection->Open();
			
			// This method will be called only once at the begining
			// Now DELETE ALL Tables

			Insert("DELETE * FROM Device");

			Insert("DELETE * FROM Texture");
		}
		catch (Exception ^_exception)
		{
			FatalLog.WriteLine("SQL::Open Error : " + _exception->Message);
		}
	}

	String^ SQL::ToString(String^ sql)
	{
		return Char(34) + sql + Char(34);
	}

	void SQL::Insert(String^ sql)
	{

		try
		{
			OleDbCommand^ command=m_Connection->CreateCommand();
			command->CommandText = sql;
			command->CommandType = System::Data::CommandType::Text;
			command->ExecuteNonQuery();
		}
		catch (Exception ^_exception)
		{
			FatalLog.WriteLine("SQL Error : " + _exception->Message);
		}

	}
}