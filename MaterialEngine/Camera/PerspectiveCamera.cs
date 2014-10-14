using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using AIOEngine.MathSpace;

namespace AIOEngine.Graphics
{
    public class PerspectiveCamera:Camera
    {
        /// <summary>
        /// Point where the camera targets
        /// </summary>
        public Vector3 LookAt;

        /// <summary>
        /// Up vector for LookAt method
        /// </summary>
        public Vector3 Up;

        /// <summary>
        /// Create a perspective camera From (0,0,-4) to (0,0,0)
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="name"></param>
        public PerspectiveCamera(StringBuilder name)
            : base(name)
        {
            //View = Matrix.CreateLookAt(new Vector3(0, 0, -4), new Vector3(0, 0, 0), new Vector3(0, 1, 0));

            this.Position = new Vector3(0, 0, -4);

            LookAt = new Vector3(0, 0, 0);

            Up = new Vector3(0, 1, 0);

            Projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.ToRadians(45.0f), 1, 0.1f, 100f);
            
        }

        /// <summary>
        /// We must override this method and do not call to base.Update, because it inherits from <see cref="Camera"/> and it will build the view matrix from the inverse node's world matrix
        /// </summary>
        public override void OnUpdate()
        {
            View = Matrix.CreateLookAt(this.Position, LookAt, Up);

            ViewProjection = View * Projection;
        }

    }
}
