#pragma once

namespace GraphicsEngineSpace {

	struct _FastStrCat
	{
		char*	Destination;
		int		Position;

		_FastStrCat(int length)
		{
			Destination=new char[length];
			Reset();
		}
		~_FastStrCat()
		{
			delete Destination;
		}
		void Reset()
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
		
		void AddReplacingNumChannel(const char *src,int numChannel)
		{
			static char *channel="%Channel%";

			while(*src!=0)
			{
				const char *c=src;
				int i=0;
				while(*c!=0 && i<8)
				{
					if(*c!=channel[i++]) break;

					c++;
				}
				
				if(i==8)
				{
					src+=i;

					Destination[Position++]=numChannel+'0';
				}
				else
				{
					Destination[Position++]=*src;
				}

				src++;
			}

			Destination[Position]=0;
		}

		void AddTabbedLine(const char *src)
		{
			AddTabbedLine(src,1);
		}

		void AddTabbedLine(const char *src,int tab)
		{
			for(int i=0;i<tab;i++)
			{
				Destination[Position++]='\t';
			}

			Add(src);

			Destination[Position++]='\n';
			Destination[Position]=0;
		}

	};
}
