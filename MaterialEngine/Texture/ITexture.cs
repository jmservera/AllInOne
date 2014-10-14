using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AIOEngine.Graphics
{
    public interface ITexture
    {
        /// <summary>
        /// Internal Pointer that references the Texture created at C++ Engine
        /// </summary>
        int GetID();
    }
}
