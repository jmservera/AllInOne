using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using AIOEngine.MathSpace;

namespace AIOEngine.Graphics
{
    /// <summary>
    /// Creates a camera that the view matrix is the inverse of the node's World matrix
    /// </summary>
    public class Camera:NodeBase
    {
        public Matrix View;
        public Matrix Projection;
        public Matrix ViewProjection { get; internal set; }

        public Camera(StringBuilder name)
            : base(name)
        {
            //View = Matrix.LookAtLH(new Vector3(0, 0, -5), new Vector3(0, 0, 0), new Vector3(0, 1, 0));

            //Projection = Matrix.PerspectiveFovLH(GraphicsMath.DegreesToRadians(45.0f), 1, 0.1f, 100f);

            View = Matrix.CreateLookAt(new Vector3(0, 0, 0), new Vector3(0, 0, 0), new Vector3(0, 1, 0));

            Projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.ToRadians(45.0f), 1, 0.1f, 100f);
            
        }

        void Resize(int width,int height)
        {

        }

        /// <summary>
        /// This is the node's Update method. We override it to compute the View Matrix
        /// </summary>
        public override void OnUpdate()
        {
            // Get View from World Matrix!!
            View = Matrix.Invert(WorldMatrix);

            ViewProjection = View * Projection;
        }

    }
}
