using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

#region NUMERICAL DEFINITION FOR FLOAT OR DOUBLE

using Numeric = System.Single;

#endregion

using GraphicsEngineSpace;

using AIOEngine.MathSpace;

namespace AIOEngine.Graphics
{
    /// <summary>
    /// A RenderTarget is a color buffer texture used for render. You can have a lot of them, but only one active.
    /// </summary>
    /// <remarks>The engine creates a <see cref="Texture2D"/> but it will not be included in the <see cref="Engine.Textures"/> list</remarks>
    public class RenderTarget:ITexture
    {
        readonly StringBuilder TAG = new StringBuilder("RenderTarget");

        public Int32 GetID() { return Texture.GetID(); }

        /// <summary>The Engine instance asociated to this texture</summary>
        public VisualEngine Engine { get; private set; }

        public Texture2D Texture;

        public _RenderTarget Internal;

        public int Width { get { return Texture.Width; } }
        public int Height { get { return Texture.Height; } }

        public StringBuilder Name;

        /// <summary>
        /// Creates a new RenderTarget
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="name"></param>
        protected RenderTarget(VisualEngine engine, StringBuilder name)
        {
            Engine = engine;

            this.Name = name;
        }

        /// <summary>
        /// Creates a new RenderTarget based in a previously created RenderTarget from C++
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="name"></param>
        /// <param name="internalRenderTarget"></param>
        internal RenderTarget(VisualEngine engine, StringBuilder name, _RenderTarget internalRenderTarget)
            : this(engine, name)
        {
            Engine.Log.AddInformation(TAG, new StringBuilder("Create " + Name + " (C++ _RenderTarget based)"));

            Internal = internalRenderTarget;

            Texture = new Texture2D(engine, name, Internal.GetTexture());
        }

        /// <summary>
        /// Creates a new RenderTarget object
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="name"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        internal RenderTarget(VisualEngine engine, StringBuilder name, int width, int height)
            : this(engine, name)
        {
            Engine.Log.AddInformation(TAG, new StringBuilder("Create " + Name + " " + width + "x" + height + " RGBA"));

            Texture = new Texture2D(engine, name);

            Texture.CreateForRenderTargetUse(name, width, height, TextureFormat.RGBA);

            Internal = new _RenderTarget(Engine.Device, Texture.InternalTexture2D);
        }

        /// <summary>
        /// Releases and destroy the resources
        /// </summary>
        internal void Destroy()
        {
            Engine.Log.AddInformation(TAG, new StringBuilder("Destroy " + Name));

            Texture.Destroy();

            Internal.Destroy();
        }

        public void Clear(float r, float g, float b, float a)
        {
            Internal.Clear(r, g, b, a);
        }

        public void Present()
        {
            Internal.Present();
        }

        /// <summary>
        /// Resize the RenderTarget if needed
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        internal void Resize(int width, int height)
        {
            if (width == Texture.Width && height == Texture.Height) return;

            Engine.Log.AddInformation(TAG, new StringBuilder("Resize " + Name + ": " + Width + "x" + Height));

            if (Internal.IsSwapChain)
            {
                Internal.Resize(width, height);
                return;
            }

            // Destroys Texture2D

            Texture.Destroy();

            // Destroys _RenderTarget

            Internal.Destroy();

            if (Internal.IsDX9BackBuffer)
            {
                Engine.Log.AddInformation(TAG, new StringBuilder("Recreating " + Name));

                _RenderTarget renderTarget = Engine.Device.CreateDX9SharedRenderTarget(Name.ToString(),width, height);
                Internal = renderTarget;
                Texture.Internal = renderTarget.GetTexture();
            }
            else
            {
                Texture.CreateForRenderTargetUse(Texture.Name, width, height, Texture.Format);

                Internal = new _RenderTarget(Engine.Device, Texture.InternalTexture2D);
            }
        }

    }
}
