#include "stdafx.h"

#include "AllHeaders.h"

/*
#include <oleauto.h>
#pragma comment(lib, "OleAut32.lib")
*/
namespace GraphicsEngineSpace {

	char *UnicodeToAnsi(BSTR unicodestr)
	{
		char *ansistr;
		int lenW = (int)wcslen(unicodestr) + 1;
		int lenA = ::WideCharToMultiByte(CP_ACP, 0, unicodestr, lenW, 0, 0, NULL, NULL);
		if (lenA > 0)
		{
		  ansistr = new char[lenA + 1]; // allocate a final null terminator as well
			::WideCharToMultiByte(CP_ACP, 0, unicodestr, lenW, ansistr, lenA, NULL, NULL);
		  ansistr[lenA] = 0; // Set the null terminator yourself
		}
		else
		{
		  // handle the error
		}

		return ansistr;
	}

	Log::Log()
	{
		m_File=NULL;

		m_Created=false;

		m_Tabs=0;

		m_TabsString[0]=0;
	}

	Log::~Log()
	{
		if(m_File!=NULL)
		{
			fclose(m_File);

			m_File=NULL;
		}

	}

	void Log::Delete()
	{
		_wunlink(m_FileName);
	}

	void Log::SetFileName(const WCHAR *fileName)
	{
		wcscpy(m_FileName,fileName);
	}

	void Log::Create()
	{
		if(m_Created) return;

		m_File=_wfopen(m_FileName,L"w");

		m_Created=true;

	}

	void Log::AddTab()
	{
		m_Tabs++;

		for(int i=0;i<m_Tabs;i++)
			m_TabsString[i]='\t';

		m_TabsString[m_Tabs]=0;
	}

	void Log::RemoveTab()
	{
		m_Tabs--;
		if(m_Tabs<0) m_Tabs=0;

		for(int i=0;i<m_Tabs;i++)
			m_TabsString[i]='\t';

		m_TabsString[m_Tabs]=0;
	}

	void Log::StartMethod(const char *text)
	{
		WriteLine(text);
		AddTab();
	}
		
	void Log::EndMethod()
	{
		WriteLine("");
		RemoveTab();
	}

	void Log::WriteLine(String^ text)
	{
		// Transform to pointer
		CString wideChar(text);
		const TCHAR *w = (LPCTSTR)wideChar;

		WriteLine(w);
	}

	void Log::WriteLine(const WCHAR *text)
	{
		Create();

		char *p=UnicodeToAnsi((BSTR)text);

		fprintf(m_File,"%s",m_TabsString);
		
		fprintf(m_File,"%s\n",p);

		fflush(m_File);

		delete[] p;
	}

	void Log::WriteLine(const char *text)
	{
		Create();

		fprintf(m_File,"%s",m_TabsString);

		fprintf(m_File,"%s\n",text);

		fflush(m_File);
	}

	void Log::WriteDXError(const WCHAR *text,HRESULT hr)
	{
		char *p=UnicodeToAnsi((BSTR)text);
		WriteDXError(p,hr);

		delete[] p;
	}
	void Log::WriteDXError(const char *text,HRESULT hr)
	{
		WriteLine("*******************************");
		WriteLine(text);
		WriteLine(DXGetErrorStringA( hr ));
		WriteLine(DXGetErrorDescriptionA( hr ));
		WriteLine("*******************************");
	}


}
