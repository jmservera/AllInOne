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
    /// A DepthStencil 
    /// </summary>
    public class DepthStencil
    {
        readonly StringBuilder TAG = new StringBuilder("DepthStencil");

        /// <summary>The DepthStencil name</summary>
        public StringBuilder Name;

        /// <summary>The Engine instance asociated to this texture</summary>
        public VisualEngine Engine { get; private set; }

        public _DepthStencil Internal;

        public int Width { get; private set; }

        public int Height { get; private set; }
        /// <summary>
        /// Creates a new DepthStencil Resource. To add a new DepthStencil, you must call to DepthStenciles.Create()
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="name"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        internal DepthStencil(VisualEngine engine, StringBuilder name, int width, int height)
        {
            Engine = engine;

            Name = name;

            Width = width;

            Height = height;

            Engine.Log.AddInformation(TAG, new StringBuilder("Create " +name+" "+ Width + "x" + Height));

            // Creates the DepthStencil & the Texture2D

            Internal = new _DepthStencil(Engine.Device,name.ToString(), width, height);

        }

        /// <summary>
        /// Resizes the DepthStencil
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        internal void Resize(int width, int height)
        {
            Destroy();

            Width = width;

            Height = height;

            Engine.Log.AddInformation(TAG, new StringBuilder("Resize "+Name+": " + Width + "x" + Height));

            Internal = new _DepthStencil(Engine.Device, Name.ToString(), width, height);

        }

        /// <summary>
        /// Destroy & Releases the resources
        /// </summary>
        public void Destroy()
        {
            Engine.Log.AddInformation(TAG, new StringBuilder("Destroy" + Width + "x" + Height));

            Internal.Destroy();

            Internal = null;
        }

        public void Clear()
        {
            Clear(1.0f, 0);
        }

        public void Clear(float depth,int stencil)
        {
            Internal.Clear(depth,stencil);
        }
    }
}
