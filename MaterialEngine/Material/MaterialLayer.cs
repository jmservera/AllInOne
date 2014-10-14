using AIOEngine.MathSpace;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AIOEngine.Graphics
{
    public class MaterialLayer
    {
        /// <summary>
        /// The Texture assigned to the diffuse map
        /// </summary>
        public ITexture DiffuseMap;

        /// <summary>
        /// The Texture assigned to the Normal map
        /// </summary>
        public ITexture NormalMap;

        /// <summary>
        /// Shader ID. 0 means Default Generic Shader
        /// </summary>
        public uint ShaderID;

        public struct _Specular
        {
            /// <summary>
            /// The texture assign to the Specular Map if used
            /// </summary>
            public ITexture SpecularMap;

            /// <summary>
            /// Specular Power. Defaults 32
            /// </summary>
            public float Power;

            /// <summary>
            /// Specularcolor. Defaults White
            /// </summary>
            public Vector3 Color;

            /// <summary>
            /// Indicates if the Specular Map must be used or not. Defaults false
            /// </summary>
            public bool Enabled;
        }
        
        public _Specular Specular;

        internal MaterialLayer()
        {
            Specular.Power = 32;
            Specular.Color = new Vector3(1, 1, 1);
        }

    }
}
