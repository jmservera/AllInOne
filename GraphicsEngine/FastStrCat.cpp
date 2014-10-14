#include "stdafx.h"

#ifdef NOT_USED_NOW

#include "AllHeaders.h"

using System::Runtime::InteropServices;

namespace GraphicsEngineSpace {

	public ref class FastStrCat
	{
	private:
		char*	Destination;
		int		Position;

	public:
		FastStrCat((int length);
		~FastStrCat();

		void Reset();
	};

	FastStrCat::FastStrCat(int length)
	{
		m_Fast=new char[length];
	}

	FastStrCat::~FastStrCat()
	{
		if(m_Fast)
		{
			delete m_Fast;

			m_Fast=NULL;
		}
	}

	void FastStrCat::Reset()
	{
		Position=0;
		Destination[0]=0;
	}

	void Add(const char *src)
	{
		int l=(int)strlen(src);
		memcpy(&Destination[Position],src,l+1);

		Position+=l;
	}
		
	void FastStrCat::Add(String^src)
	{
		/*
        IntPtr ptr = Marshal::StringToHGlobalAnsi(src);

        Add(ptr.ToPointer());
            
        Marshal::FreeHGlobal(ptr);
		*/
		
		const __wchar_t __pin * str = PtrToStringChars(src);

		int l=(int)src->Length;
		
		char *dest=&Destination[Position];
		char *source=(char *)str;

		while(l-->0)
		{
			*dest++=*str++;
			str++;	// jump the unicode second byte
		}

		*dest=0;

		Position+=l;
	}
}
#endif
