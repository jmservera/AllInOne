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
    /// <summary>
    /// Represents the basic properties of a camera to use it in the shader
    /// </summary>
    public class CameraBase
    {
        public Vector3 Position;
    }
}
