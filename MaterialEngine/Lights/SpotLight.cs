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

    public class SpotLight : Light
    {
        /// <summary>
        /// Defines the possible parameters for the projectors
        /// </summary>
        public struct _Projector
        {
            /// <summary>Enable/Disable the projector</summary>
            public bool Enabled;
            /// <summary>the matrix for the projector</summary>
            public Matrix Matrix;
            /// <summary>The texture for the projector. Can be <see cref="HTexture2D"/> for spot lights or <see cref="HTextureCube"/> for omni lights</summary>
            public ITexture Map;

        }

        /// <summary>The light has no attenuation from angle 0 to HotSpot. This value must be set in Degrees</summary>
        public Numeric HotSpot;

        /// <summary>The light is dark when the angle is beyond the FallOff. This value must be set in Degrees</summary>
        public Numeric FallOff;

        /// <summary>The light's direction. Note: it's the same value that WorldMatrix.Forward</summary>
        public Vector3 Direction { get { return this.WorldMatrix.Forward; } }

        public _Projector Projector;
        public Matrix ProjectorPerspective;

        public SpotLight(Numeric range)
            : base()
        {
            Attenuation = Attenuation.Inverse;

            this.Range = range;

            // HotSpot por defecto
            HotSpot = 43.0f;
            // FallOff por defecto
            FallOff = 45.0f;
        }

        public void BuildProjectorPerspective()
        {
            // The Aspect Ratio should be based on TextureMap
            ProjectorPerspective = Matrix.CreatePerspectiveFieldOfView(MathHelper.ToRadians(FallOff * 2.0f), 1f, 0.25f, Range);

        }

    }
}
