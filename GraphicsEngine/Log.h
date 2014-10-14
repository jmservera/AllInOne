#pragma once

namespace GraphicsEngineSpace {

	char *UnicodeToAnsi(BSTR unicodestr);

	public class Log
	{
	private:

		bool m_Created;
		WCHAR m_FileName[1024];

		FILE*				m_File;

		int m_Tabs;

		char m_TabsString[50];
		
	public:

		Log();

		~Log();

		void SetFileName(const WCHAR *fileName);

		void Delete();
		void AddTab();
		void RemoveTab();
		void StartMethod(const char *text);
		void EndMethod();
		void Create();
		void WriteLine(String^ text);
		void WriteLine(const WCHAR *text);
		void WriteLine(const char *text);
		void WriteDXError(const WCHAR *text,HRESULT hr);
		void WriteDXError(const char *text,HRESULT hr);

	};
}
