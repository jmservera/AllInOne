using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using GraphicsEngineSpace;

using AIOEngine.MathSpace;

namespace AIOEngine.Graphics
{
    public enum TextureFormat
    {
        RGBA,
        Float
    }

    public class Texture2D : Texture
    {

        public StringBuilder FullPath { get; private set; }

        public _Texture2D InternalTexture2D { get { return (_Texture2D)Internal; } }

        public int Width { get { return ((_Texture2D)Internal).Width; } }
        public int Height { get { return ((_Texture2D)Internal).Height; } }

        public TextureFormat Format { get; private set; }
        public DXGIFORMAT DXFormat { get { return InternalTexture2D.Format; } }

        /// <summary>
        /// Creates a Texture without size. This is used by <see cref="DepthStencil"/> constructor
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="name"></param>
        internal Texture2D(VisualEngine engine, StringBuilder name)
            : base(engine,name)
        {

        }

        /// <summary>
        /// Creates a Texture assigning then C++ object. This is used when creating a backbuffer based <see cref="RenderTarget"/>
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="name"></param>
        internal Texture2D(VisualEngine engine, StringBuilder name,_Texture2D internalTexture)
            : base(engine, name)
        {
            Engine.Log.AddInformation(TAG, new StringBuilder("Create " + Name + " (C++ _Texture2D based)"));

            Internal = internalTexture;
        }

        /// <summary>
        /// Creates a Texture and loads it
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="name"></param>
        public Texture2D(VisualEngine engine, StringBuilder name, StringBuilder fullPath)
            : base(engine,name)
        {
            Engine.Log.AddInformation(TAG,new StringBuilder("Creating " + name));

            FullPath = fullPath;

            Internal = new GraphicsEngineSpace._Texture2D(engine.Device,name.ToString());

            LoadFromFile();
        }

        public void CreateForRenderTargetUse(StringBuilder name,int width, int height,TextureFormat format)
        {
            Create(name, width, height, format,true);
        }

        public void Create(StringBuilder name,int width, int height,TextureFormat format)
        {
            Create(name, width, height, format, false);
        }

        void Create(StringBuilder name,int width, int height,TextureFormat format,bool renderTargetUse)
        {
            Engine.Log.AddInformation(TAG, new StringBuilder("Creating " + name));

            this.Format = format;

            DXGIFORMAT dxFormat=(DXGIFORMAT)0;

            if(format==TextureFormat.RGBA)
                dxFormat=DXGIFORMAT.DXGI_FORMAT_R8G8B8A8_UNORM;

            Internal = new GraphicsEngineSpace._Texture2D(Engine.Device,name.ToString());

            InternalTexture2D.Create(width, height, dxFormat, renderTargetUse);

        }

        internal void LoadFromFile()
        {
            InternalTexture2D.LoadFromFile(FullPath.ToString());

        }

        public void Save(string fullPath)
        {
            InternalTexture2D.Save(fullPath);
        }

        public unsafe void GetRGBData(Color32[] data)
        {
            int rowPitch;
            byte* p = InternalTexture2D.MapData(out rowPitch,false);

            int i=0;

            for (int y = 0; y < Height; y++)
            {
                byte* row = p;
                for (int x = 0; x < Width; x++)
                {
                    byte r = *row++;
                    byte g = *row++;
                    byte b = *row++;
                    byte a = *row++;

                    data[i++] = new Color32(r, g, b, a);
                }
                p += rowPitch;
            }

            InternalTexture2D.Unmap();
        }

        public unsafe void SetRGBData(Color32[] data)
        {
            int rowPitch;
            byte* p = InternalTexture2D.MapData(out rowPitch,true);

            int i = 0;

            for (int y = 0; y < Height; y++)
            {
                byte* row = p;
                for (int x = 0; x < Width; x++)
                {
                    *row++ = data[i].R;
                    *row++ = data[i].G;
                    *row++ = data[i].B;
                    *row++ = data[i].A;

                    i++;
                }
                p += rowPitch;
            }

            InternalTexture2D.Unmap(true);
        }
    }
}
