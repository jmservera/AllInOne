using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AIOEngine.MathSpace
{
    /// <summary>
    /// Represents a 32 bts RGBA colour. Each component contains values from 0 to 255
    /// </summary>
    public struct Color32
    {
        public byte R, G, B, A;

        public Color32(byte r,byte g,byte b)
        {
            R = r;
            G = g;
            B = b;

            A = 255;
        }

        public Color32(byte r, byte g, byte b,byte a)
        {
            R = r;
            G = g;
            B = b;

            A = a;
        }
    }
}
