#region License
/*
MIT License
Copyright © 2006 The Mono.Xna Team

All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#endregion License

using System;
using System.Collections.Generic;
using System.Text;

#region NUMERICAL DEFINITION FOR FLOAT OR DOUBLE

using Numeric = System.Single;

#endregion

namespace AIOEngine.MathSpace
{
    public static class MathHelper
    {
        public const Numeric E = (Numeric)Math.E;
        public const Numeric Log10E = 0.4342945f;
        public const Numeric Log2E = 1.442695f;
        public const Numeric Pi = (Numeric)Math.PI;
        public const Numeric PiOver2 = (Numeric)(Math.PI / 2.0);
        public const Numeric PiOver4 = (Numeric)(Math.PI / 4.0);
        public const Numeric TwoPi = (Numeric)(Math.PI * 2.0);
        
        public static void XMScalarSinCos(ref float sin,ref float cos,float angleInRadians)
        {
            sin = (float)Math.Sin(angleInRadians);
            cos = (float)Math.Cos(angleInRadians);
        }
        public static Numeric Barycentric(Numeric value1, Numeric value2, Numeric value3, Numeric amount1, Numeric amount2)
        {
            return value1 + (value2 - value1) * amount1 + (value3 - value1) * amount2;
        }

        public static Numeric CatmullRom(Numeric value1, Numeric value2, Numeric value3, Numeric value4, Numeric amount)
        {
            // Using formula from http://www.mvps.org/directx/articles/catmull/
            // Internally using Numerics not to lose precission
            Numeric amountSquared = amount * amount;
            Numeric amountCubed = amountSquared * amount;
            return (Numeric)(0.5 * (2.0 * value2 +
                (value3 - value1) * amount +
                (2.0 * value1 - 5.0 * value2 + 4.0 * value3 - value4) * amountSquared +
                (3.0 * value2 - value1 - 3.0 * value3 + value4) * amountCubed));
        }

        public static Numeric Clamp(Numeric value, Numeric min, Numeric max)
        {
            // First we check to see if we're greater than the max
            value = (value > max) ? max : value;

            // Then we check to see if we're less than the min.
            value = (value < min) ? min : value;

            // There's no check to see if min > max.
            return value;
        }
        
        public static Numeric Distance(Numeric value1, Numeric value2)
        {
            return Math.Abs(value1 - value2);
        }
        
        public static Numeric Hermite(Numeric value1, Numeric tangent1, Numeric value2, Numeric tangent2, Numeric amount)
        {
            // All transformed to Numeric not to lose precission
            // Otherwise, for high numbers of param:amount the result is NaN instead of Infinity
            Numeric v1 = value1, v2 = value2, t1 = tangent1, t2 = tangent2, s = amount, result;
            Numeric sCubed = s * s * s;
            Numeric sSquared = s * s;

            if (amount == 0f)
                result = value1;
            else if (amount == 1f)
                result = value2;
            else
                result = (2 * v1 - 2 * v2 + t2 + t1) * sCubed +
                    (3 * v2 - 3 * v1 - 2 * t1 - t2) * sSquared +
                    t1 * s +
                    v1;
            return (Numeric)result;
        }
        
        
        public static Numeric Lerp(Numeric value1, Numeric value2, Numeric amount)
        {
            return value1 + (value2 - value1) * amount;
        }

        public static Numeric Max(Numeric value1, Numeric value2)
        {
            return Math.Max(value1, value2);
        }
        
        public static Numeric Min(Numeric value1, Numeric value2)
        {
            return Math.Min(value1, value2);
        }
        
        public static Numeric SmoothStep(Numeric value1, Numeric value2, Numeric amount)
        {
            // It is expected that 0 < amount < 1
            // If amount < 0, return value1
            // If amount > 1, return value2
#if(USE_FARSEER)
            Numeric result = SilverSpriteMathHelper.Clamp(amount, 0f, 1f);
            result = SilverSpriteMathHelper.Hermite(value1, 0f, value2, 0f, result);
#else
            Numeric result = MathHelper.Clamp(amount, 0f, 1f);
            result = MathHelper.Hermite(value1, 0f, value2, 0f, result);
#endif
            return result;
        }
        
        public static Numeric ToDegrees(Numeric radians)
        {
            // This method uses Numeric precission internally,
            // though it returns single Numeric
            // Factor = 180 / pi
            return (Numeric)(radians * 57.295779513082320876798154814105);
        }
        
        public static Numeric ToRadians(Numeric degrees)
        {
            // This method uses Numeric precission internally,
            // though it returns single Numeric
            // Factor = pi / 180
            return (Numeric)(degrees * 0.017453292519943295769236907684886);
        }

	public static Numeric WrapAngle(Numeric angle)
	{
		angle = (Numeric)Math.IEEERemainder((Numeric)angle, 6.2831854820251465);
		if (angle <= -3.14159274f)
		{
			angle += 6.28318548f;
		}
		else
		{
		if (angle > 3.14159274f)
		{
			angle -= 6.28318548f;
		}
		}
		return angle;
	}

		public static bool IsPowerOfTwo(int value)
		{
			return (value > 0) && ((value & (value - 1)) == 0);
		}
    }
}
