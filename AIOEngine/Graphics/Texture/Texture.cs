using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GraphicsEngineSpace;

namespace AIOEngine.Graphics
{
    public class Texture:ITexture
    {
        protected readonly StringBuilder TAG = new StringBuilder("Texture");

        public Int32 GetID() { return Internal.GetID(); }

        /// <summary>The texture name</summary>
        public StringBuilder Name;

        internal GraphicsEngineSpace._Texture Internal;

        /// <summary>The Engine instance asociated to this texture</summary>
        public VisualEngine Engine { get; private set; }

        public Texture(VisualEngine engine, StringBuilder name)
        {
            Engine = engine;

            Name = name;

        }

        /// <summary>
        /// Destroy and free the resources
        /// </summary>
        public virtual void Destroy()
        {

            if (Internal != null)
            {
                Engine.Log.AddInformation(TAG, new StringBuilder("Destroy " + Name));

                Internal.Destroy();

                Internal = null;
            }
        }

    }
}
