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
    public class OmniLight:Light
    {
        public OmniLight(float range)
            : base()
        {
            Attenuation = Attenuation.Inverse;

            this.Range = range;
        }
    }
}
