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
    /// The bounding box is an Axis aligned box surrounding the object
    /// Used for frustum culling, checking if the bounding box of an object
    /// is outside or inside the <see cref="Frustum"/> lets the engine discard
    /// the object on the render stage
    /// </summary>
	public class BoundingBox
	{
		#region Fields

		/// <summary>minimun point (left, down, near)</summary>
		private Vector3 min;
		/// <summary>maximun point (right, up, far)</summary>
		private Vector3 max;

		#endregion

		#region Properties

		/// <summary>
        /// Get/Set minimun point (left, down, near)
		/// </summary>
		public Vector3 Min
		{
			get
			{
				return min;
			}
			set
			{
				min = value;
			}
		}

		/// <summary>
        /// Get/Set maximun point (right, up, far)
		/// </summary>
		public Vector3 Max
		{
			get
			{
				return max;
			}
			set
			{
				max = value;
			}
		}

        /// <summary>Returns the Width of the box</summary>
		public float Width
		{
			get
			{
				return max.X - min.X;
			}
		}

        /// <summary>Returns the Height of the box</summary>
        public float Height
		{
			get
			{
				return max.Y - min.Y;
			}
		}

        /// <summary>Returns the Depth of the box</summary>
        public float Depth
		{
			get
			{
				return max.Z - min.Z;
			}
		}

		#endregion

		#region Constructors

		/// <summary>
		/// Default. The min/max values are not initialized
		/// </summary>
		public BoundingBox()
		{
		}

		/// <summary>
		/// Constructor setting the min and max values of the box
		/// </summary>
        /// <param name="min">minimun point (left, down, near)</param>
        /// <param name="max">maximun point (right, up, far)</param>
		public BoundingBox(Vector3 min, Vector3 max)
		{
			SetMinMax(min, max);
		}

        /// <summary>
        /// Buidls the box from another box
        /// </summary>
        /// <param name="bbox">the source BoundingBox</param>
		public BoundingBox(BoundingBox bbox)
		{
			min = bbox.Min;
			max = bbox.Max;
		}

		#endregion

		#region Methods

		/// <summary>
        /// Set the min and max values and <see cref="Normalize"/> the result
		/// </summary>
        /// <param name="min">minimun point (left, down, near)</param>
        /// <param name="max">maximun point (right, up, far)</param>
		public void SetMinMax(Vector3 min, Vector3 max)
		{
			this.min = min;
			this.max = max;

			Normalize();
		}

		/// <summary>
		/// Adds the offset to the min and max values of the bounding box
		/// </summary>
		/// <param name="offset">offset</param>
		public void AddOffset(Vector3 offset)
		{
			min -= offset;

			max += offset;
		}

		/// <summary>
		/// Checks the vertex with the Bounding Box and changes the boundings if necesary
		/// </summary>
		/// <param name="vertex">the vertex to check</param>
        public void Check(Vector3 vertex)
        {
            Check(ref vertex);
        }
		/// <summary>
		/// Checks the vertex with the Bounding Box and changes the boundings if necesary
		/// </summary>
		/// <param name="vertex">the vertex to check</param>
        public void Check(ref Vector3 vertex)
		{
            if (vertex.X < min.X)
                min.X = vertex.X;
            if (vertex.X > max.X)
                max.X = vertex.X;

            if (vertex.Y < min.Y)
                min.Y = vertex.Y;
            if (vertex.Y > max.Y)
                max.Y = vertex.Y;

            if (vertex.Z < min.Z)
                min.Z = vertex.Z;
            if (vertex.Z > max.Z)
                max.Z = vertex.Z;
		}

		/// <summary>
		/// Set the min to the max values and the max to the min values
        /// This way any vertex <see cref="CheckVertex"/Checked> with this BB
        /// will modify the Bounding Box
		/// </summary>
		public void Reset()
		{
			min = new Vector3(float.MaxValue, float.MaxValue, float.MaxValue);
			max = new Vector3(float.MinValue, float.MinValue, float.MinValue);
		}

		/// <summary>
		/// Checks the min and max so the values are coherent.
		/// </summary>
		public void Normalize()
		{
			float t;

			if (max.X < min.X)
			{
				t = min.X;
				min.X = max.X;
				max.X = t;
			}
			if (max.Y < min.Y)
			{
				t = min.Y;
				min.Y = max.Y;
				max.Y = t;
			}
			if (max.Z < min.Z)
			{
				t = min.Z;
				min.Z = max.Z;
				max.Z = t;
			}
		}

		/// <summary>
		/// Transforms the 3 floats by the matrix and check the vertex
		/// </summary>
		/// <param name="x">x value</param>
		/// <param name="y">y value</param>
		/// <param name="z">z value</param>
		/// <param name="matrix">matrix to transform</param>
		public void CheckVertex(float x, float y, float z, ref Matrix matrix)
		{
            Vector3 newVertex=new Vector3(x, y, z);
            CheckVertex(ref newVertex, ref matrix);
		}

		/// <summary>
        /// Transforms the vector by the matrix and check the vertex
		/// </summary>
        /// <param name="vertex">the vertex</param>
        /// <param name="matrix">matrix to transform</param>
        public void CheckVertex(ref Vector3 vertex, ref Matrix matrix)
		{
            Vector3 newVertex=Vector3.Transform(vertex, matrix);

            Check(ref newVertex);

		}

		/// <summary>
        /// Apply the matrix to the 8 corners of the boundingbox and gets the new Boundings
		/// </summary>
        /// <param name="matrix">matrix to transform</param>
		public void ApplyMatrix2(ref Matrix matrix)
		{
			// Si hay rotación tenemos que recalcular todas las esquinas
			// tenemos que ponernos en el peor caso posible!!
			float xi, yi, zi, xf, yf, zf;

			xi = min.X;
			yi = min.Y;
			zi = min.Z;
			xf = max.X;
			yf = max.Y;
			zf = max.Z;

			Reset();

			// Ahora calculamos las 8 esquinas

            CheckVertex(xi, yi, zi, ref matrix);
            CheckVertex(xi, yf, zi, ref matrix);
            CheckVertex(xf, yf, zi, ref matrix);
            CheckVertex(xf, yi, zi, ref matrix);

            CheckVertex(xi, yi, zf, ref matrix);
            CheckVertex(xi, yf, zf, ref matrix);
            CheckVertex(xf, yf, zf, ref matrix);
            CheckVertex(xf, yi, zf, ref matrix);
		}

		/// <summary>
		/// Transform the bounding box
		/// </summary>
		/// <param name="matrix">Matrix will be applied to the bounding box</param>
		/// <remarks>
		/// *
		/// * "Transforming Axis-Aligned Bounding Boxes",
		/// *  by Jim Arvo, in "Graphics Gems", Academic Press, 1990.
		/// *
		/// * This function transforms a 3D axis-aligned box using a 3x3 
		/// * matrix and a translation vector.  It returns the tightest
		/// * axis-aligned box that encloses the result. 
		/// *
		/// * The utility of this algorithm is that it performs the box
		/// * transformation with far fewer flops than the naive approach
		/// * of transforming the eight vertices of the original box,
		/// * then computing upper and lower bounds of the transformed
		/// * points.  By using all of the inherent symmetry of the boxes,
		/// * this algorithms is about four times as fast as the naive
		/// * algorithm.
		/// *
		/// * Note that computing axis-aligned boxes in this way is fast,
		/// * but NOT recommended.  Whenever possible, the bounding box
		/// * should be computed from the transformed object, NOT the
		/// * transformed bounding box of the object.  The latter approach
		/// * can produce very loose bounding boxes, which can hurt the
		/// * performance of ray tracers and other algorithms that make
		/// * use of bounding boxes.
		/// *
		/// </remarks>
		public void ApplyMatrix(Matrix matrix)
		{
			int i, j;
		    float  a, b;
			float[] aMin, aMax;
			float[] bMin, bMax;
			float[,] m;

			// Store the matrix into an array

			m = new float[3, 3];
			m[0, 0] = matrix.M11; m[1, 0] = matrix.M12; m[2, 0] = matrix.M13;
			m[0, 1] = matrix.M21; m[1, 1] = matrix.M22; m[2, 1] = matrix.M23;
			m[0, 2] = matrix.M31; m[1, 2] = matrix.M32; m[2, 2] = matrix.M33;

			// Copy box A into a min array and a max array for easy reference

			aMin = new float[3];
			aMax = new float[3];

			aMin[0] = min.X; aMax[0] = max.X;
			aMin[1] = min.Y; aMax[1] = max.Y;
			aMin[2] = min.Z; aMax[2] = max.Z;

			// Account for the translation by beginning at T

			bMin = new float[3];
			bMax = new float[3];

			bMin[0] = bMax[0] = matrix.M41;
			bMin[1] = bMax[1] = matrix.M42;
			bMin[2] = bMax[2] = matrix.M43;

			// Now find the extreme points by considering the product of the min and max with each component of M

			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 3; j++)
				{
					a = m[i, j] * aMin[j];
					b = m[i, j] * aMax[j];

					if (a < b)
					{
						bMin[i] += a;
						bMax[i] += b;
					}
					else
					{
						bMin[i] += b;
						bMax[i] += a;
					}
				}
			}

			// Copy the new min/max

			min.X = bMin[0]; max.X = bMax[0];
			min.Y = bMin[1]; max.Y = bMax[1];
			min.Z = bMin[2]; max.Z = bMax[2];
		}

        /// <summary>
        /// Computes the BoundingBox from the <see cref="BoundingSphere"/>
        /// </summary>
        /// <param name="boundingSphere">a valid HBoundingSphere</param>
        public void FromBoundingSphere(BoundingSphere boundingSphere)
        {
            min = new Vector3(boundingSphere.Center.X - boundingSphere.Radius, boundingSphere.Center.Y - boundingSphere.Radius, boundingSphere.Center.Z - boundingSphere.Radius);
            max = new Vector3(boundingSphere.Center.X + boundingSphere.Radius, boundingSphere.Center.Y + boundingSphere.Radius, boundingSphere.Center.Z + boundingSphere.Radius);
        }

        /// <summary>
        /// Gets the Bounding Box from a <see cref="MeshBasic"/>
        /// </summary>
        /// <param name="mesh">a valid mesh</param>
        public void FromMesh(MeshBasic mesh)
		{
            int numVertices = mesh.NumVertices;

			Reset();

			for (int c = 0; c < numVertices; c++)
			{
				Check(ref mesh.Position[c]);

			}

		}

        /// <summary>
        /// returns a formated string for the Bounding Box
        /// </summary>
        /// <returns>the formated string</returns>
		public override string ToString()
		{
			return String.Format("Min ({0}, {1}, {2}) Max ({3}, {4}, {5})", min.X, min.Y, min.Z, max.X, max.Y, max.Z);
		}

        /// <summary>
        /// return the center of the Box
        /// </summary>
        /// <returns>a vertex representing the center</returns>
		public Vector3 Center()
		{
			return new Vector3((min.X + max.X) / 2, (min.Y + max.Y) / 2, (min.Z + max.Z) / 2);
		}

		/// <summary>
		/// Converts the BB into an array of vertexs ready to Render Lines
		/// </summary>
		/// <returns>an array of vertices</returns>
		public Vector3[] ToRenderLines()
		{
			Vector3[] v = new Vector3[24];
			int n = 0;

			// Cara frontal

			v[n].X = min.X;
			v[n].Y = max.Y;
			v[n].Z = min.Z;
			n++;
			v[n].X = max.X;
			v[n].Y = max.Y;
			v[n].Z = min.Z;
			n++;

			v[n].X = min.X;
			v[n].Y = max.Y;
			v[n].Z = min.Z;
			n++;
			v[n].X = min.X;
			v[n].Y = min.Y;
			v[n].Z = min.Z;
			n++;

			v[n].X = min.X;
			v[n].Y = min.Y;
			v[n].Z = min.Z;
			n++;
			v[n].X = max.X;
			v[n].Y = min.Y;
			v[n].Z = min.Z;
			n++;

			v[n].X = max.X;
			v[n].Y = max.Y;
			v[n].Z = min.Z;
			n++;
			v[n].X = max.X;
			v[n].Y = min.Y;
			v[n].Z = min.Z;
			n++;

			// Cara trasera

			v[n].X = min.X;
			v[n].Y = max.Y;
			v[n].Z = max.Z;
			n++;
			v[n].X = max.X;
			v[n].Y = max.Y;
			v[n].Z = max.Z;
			n++;

			v[n].X = min.X;
			v[n].Y = max.Y;
			v[n].Z = max.Z;
			n++;
			v[n].X = min.X;
			v[n].Y = min.Y;
			v[n].Z = max.Z;
			n++;

			v[n].X = min.X;
			v[n].Y = min.Y;
			v[n].Z = max.Z;
			n++;
			v[n].X = max.X;
			v[n].Y = min.Y;
			v[n].Z = max.Z;
			n++;

			v[n].X = max.X;
			v[n].Y = max.Y;
			v[n].Z = max.Z;
			n++;
			v[n].X = max.X;
			v[n].Y = min.Y;
			v[n].Z = max.Z;
			n++;

			// Cara superior

			v[n].X = min.X;
			v[n].Y = max.Y;
			v[n].Z = min.Z;
			n++;
			v[n].X = min.X;
			v[n].Y = max.Y;
			v[n].Z = max.Z;
			n++;

			v[n].X = max.X;
			v[n].Y = max.Y;
			v[n].Z = min.Z;
			n++;
			v[n].X = max.X;
			v[n].Y = max.Y;
			v[n].Z = max.Z;
			n++;

			// lateral derecho

			v[n].X = max.X;
			v[n].Y = min.Y;
			v[n].Z = min.Z;
			n++;
			v[n].X = max.X;
			v[n].Y = min.Y;
			v[n].Z = max.Z;
			n++;

			// lateral izq

			v[n].X = min.X;
			v[n].Y = min.Y;
			v[n].Z = min.Z;
			n++;
			v[n].X = min.X;
			v[n].Y = min.Y;
			v[n].Z = max.Z;
			n++;

			return v;
		}

        /// <summary>
        /// Checks if a point is inside the BoundingBox
        /// </summary>
        /// <param name="point">a point to ckeck</param>
        /// <returns>true if it's inside</returns>
		public bool PointInside(Vector3 point)
		{
			if (point.X >= min.X && point.X <= max.X)
				if (point.Y >= min.Y && point.Y <= max.Y)
					if (point.Z >= min.Z && point.Z <= max.Z)
						return true;

			return false;
		}

        /// <summary>
        /// Checks if a point is inside the BoundingBox
        /// </summary>
        /// <param name="point">a reference to the point to ckeck</param>
        /// <returns>true if it's inside</returns>
        public bool PointInside(ref Vector3 point)
		{
			if (point.X >= min.X && point.X <= max.X)
				if (point.Y >= min.Y && point.Y <= max.Y)
					if (point.Z >= min.Z && point.Z <= max.Z)
						return true;

			return false;
		}

        /// <summary>
        /// Checks if a point is inside the BoundingBox
        /// </summary>
        /// <param name="x">x value from the point</param>
        /// <param name="y">y value from the point</param>
        /// <param name="z">z value from the point</param>
        /// <returns>true if it's inside</returns>
		public bool PointInside(float x, float y, float z)
		{
			if (x >= min.X && x <= max.X)
				if (y >= min.Y && y <= max.Y)
					if (z >= min.Z && z <= max.Z)
						return true;

			return false;
		}

        /// <summary>
        /// Checks if the 8 points of the BB are inside the BB passed as parameter
        /// </summary>
        /// <param name="bb">the bounding box to check if it's inside</param>
        /// <returns>true if the bounding box it's inside</returns>
		public bool Inside(BoundingBox bb)
		{
			// Basta que uno de los 8 vértices esté dentro 

			// tenemos que ponernos en el peor caso posible!!
			float xi, yi, zi, xf, yf, zf;

			xi = min.X;
			yi = min.Y;
			zi = min.Z;
			xf = max.X;
			yf = max.Y;
			zf = max.Z;

			if (bb.PointInside(xi, yi, zi)) return true;
			if (bb.PointInside(xi, yf, zi)) return true;
			if (bb.PointInside(xf, yf, zi)) return true;
			if (bb.PointInside(xf, yi, zi)) return true;

			if (bb.PointInside(xi, yi, zf)) return true;
			if (bb.PointInside(xi, yf, zf)) return true;
			if (bb.PointInside(xf, yi, zf)) return true;
			if (bb.PointInside(xf, yf, zf)) return true;

			return false;

		}

 		#endregion

		#region Operators

        /// <summary>
        /// Builds a new BB thats surrounds the 2 bounding boxes
        /// </summary>
        /// <param name="lhs"></param>
        /// <param name="rhs"></param>
        /// <returns></returns>
		public static BoundingBox operator +(BoundingBox lhs, BoundingBox rhs)
		{
            Vector3 min = Vector3.Zero, max = Vector3.Zero;

			min.X = lhs.Min.X < rhs.Min.X ? lhs.Min.X : rhs.Min.X;
			min.Y = lhs.Min.Y < rhs.Min.Y ? lhs.Min.Y : rhs.Min.Y;
			min.Z = lhs.Min.Z < rhs.Min.Z ? lhs.Min.Z : rhs.Min.Z;

			max.X = lhs.Max.X > rhs.Max.X ? lhs.Max.X : rhs.Max.X;
			max.Y = lhs.Max.Y > rhs.Max.Y ? lhs.Max.Y : rhs.Max.Y;
			max.Z = lhs.Max.Z > rhs.Max.Z ? lhs.Max.Z : rhs.Max.Z;

			return new BoundingBox(min, max);
		}

        /// <summary>
        /// Builds a new BB thats surrounds the bounding box and the vextex
        /// </summary>
        /// <param name="lhs"></param>
        /// <param name="rhs"></param>
        /// <returns></returns>
		public static BoundingBox operator +(BoundingBox lhs, Vector3 rhs)
		{
            Vector3 min = Vector3.Zero, max = Vector3.Zero;

			min.X = lhs.Min.X < rhs.X ? lhs.Min.X : rhs.X;
			min.Y = lhs.Min.Y < rhs.Y ? lhs.Min.Y : rhs.Y;
			min.Z = lhs.Min.Z < rhs.Z ? lhs.Min.Z : rhs.Z;

			max.X = lhs.Max.X > rhs.X ? lhs.Max.X : rhs.X;
			max.Y = lhs.Max.Y > rhs.Y ? lhs.Max.Y : rhs.Y;
			max.Z = lhs.Max.Z > rhs.Z ? lhs.Max.Z : rhs.Z;

			return new BoundingBox(min, max);
		}

        /// <summary>
        /// Adds a value to the limits, the min and the max
        /// </summary>
        /// <param name="lhs"></param>
        /// <param name="size"></param>
        /// <returns></returns>
		public static BoundingBox operator +(BoundingBox lhs, float size)
		{
			return new BoundingBox(lhs.Min - new Vector3(size, size, size), lhs.Max + new Vector3(size, size, size));
		}

		#endregion
	}
}
