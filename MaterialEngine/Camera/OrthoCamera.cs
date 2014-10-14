using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using AIOEngine.MathSpace;

namespace AIOEngine.Graphics
{
    /// <summary>
    /// An Orthogonal camera is a camera used to draw sprites setting x,y as screen coords.
    /// With this camera, the values for the world matrix are in screen coords.
    /// </summary>
    public class OrthoCamera:Camera
    {
        public OrthoCamera(StringBuilder name)
            : base(name)
        {
        }

        /// <summary>
        /// Updates the projection using the screen size
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        public void UpdateProjection(int width,int height)
        {
            Projection = Matrix.CreateOrthographic(width, height, 0, 1);

        }

        public override void OnUpdate()
        {
        }
    }
}
