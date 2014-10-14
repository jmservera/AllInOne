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
    public enum Attenuation
    {
        None=0,
        Inverse=1,
        DualRadius=2,
        InverseSquare=3
    }
}
