using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using AIOEngine.MathSpace;

#region NUMERICAL DEFINITION FOR FLOAT OR DOUBLE

using Numeric = System.Single;

#endregion

namespace AIOEngine.Graphics
{
    public class Light:NodeBase
    {
        public Attenuation Attenuation;

        /// <summary>The range distance limit for this light</summary>
        public float Range;

        /// <summary>The Intensity that will be multiplied by <see cref="Color "/> in the shader. Defaults 1.0</summary>
        public float Intensity;

        /// <summary>Light's color. This value will be multiplied by <see cref="Intensity"/> and then passed to the shader as Color</summary>
        public Vector4 Color;

        /// <summary>The attenuation start point, relative to the light position. Only used for Attenuation=DualRadius</summary>
        public float FarAttenStart;

        public Light()
            : base()
        {
            Attenuation = Attenuation.None;

            Range = 5.3f;

            Intensity = 1;

            Color = new Vector4(1, 0, 0, 1);

            FarAttenStart = 2;
        }

    }
}
