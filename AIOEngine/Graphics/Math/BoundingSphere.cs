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
    /// The bounding sphere is an sphere surrounding the object
    /// Used for frustum culling, checking if the bounding sphere of an object
    /// is outside or inside the <see cref="Frustum"/> lets the engine discard
    /// the object on the render stage
    /// </summary>
    public class BoundingSphere
	{
		#region Fields

		/// <summary>the center of the BoundingSphere</summary>
		private Vector3 center;

		/// <summary>the radius of the BoundingSphere</summary>
		private float radius;

		#endregion

		#region Properties

		/// <summary>
		/// Get/Set the center of the sphere
		/// </summary>
		public Vector3 Center
		{
			get
			{
				return center;
			}
			set
			{
				center = value;
			}
		}

		/// <summary>
		/// Get/Set the radius
		/// </summary>
		public float Radius
		{
			get
			{
				return radius;
			}
			set
			{
				radius = value;
			}
		}

		/// <summary>
		/// Get/Set the diameter
		/// </summary>
		public float Diameter
		{
			get
			{
				return radius * 2;
			}
			set
			{
				radius = value * 0.5f;
			}
		}

		#endregion

		#region Constructor

		/// <summary>
		/// Default Constructor. Do not initilize any value
		/// </summary>
		public BoundingSphere()
		{
		}

		/// <summary>
		/// Constructor for the center and radius
		/// </summary>
		/// <param name="centro">center of the sphere</param>
		/// <param name="radio">radius of the sphere</param>
		public BoundingSphere(Vector3 centro, float radio)
		{
			this.center = centro;
			this.radius = radio;
		}

		/// <summary>
		/// Constructor from another Bounding Sphere
		/// </summary>
		/// <param name="bSphere">source Bounding sphere</param>
		public BoundingSphere(BoundingSphere bSphere)
		{
			center = bSphere.Center;
			radius = bSphere.Radius;
		}

		#endregion

        #region Methods

        /// <summary>
        /// Returns the <see cref="BoundingBox"/> of the sphere
        /// </summary>
        /// <returns>a HBoundingBox</returns>
        public BoundingBox ToBoundingBox()
        {
            BoundingBox b = new BoundingBox();
            b.Reset();

            b.Check(center + new Vector3(radius, 0, 0));
            b.Check(center + new Vector3(0,radius,0));
            b.Check(center + new Vector3(0,0,radius));

            b.Check(center + new Vector3(-radius, 0, 0));
            b.Check(center + new Vector3(0, -radius, 0));
            b.Check(center + new Vector3(0, 0, -radius));

            return b;
        }

        /// <summary>
        /// Set the center and radius from a boundingbox
        /// </summary>
        /// <param name="box">A valid HBoundingBox</param>
        public void FromBoundingBox(BoundingBox box)
        {
            Vector3 c = box.Center();
            Vector3 vd= box.Max - c;
            float r = vd.X;

            if (vd.Y > r) r = vd.Y;
            if (vd.Z > r) r = vd.Z;

            radius = r;

            center = c;
        }

        /// <summary>
        /// Checks if a point is inside the sphere
        /// </summary>
        /// <param name="pto">the point to check</param>
        /// <returns>true if it's inside</returns>
		public bool PointInside(Vector3 pto)
		{
			Vector3 v = (pto - center);
			if (v.Length() <= radius) return true;
			return false;
		}

        public VerticesVector3Indices ToRenderLines(int numSubdivisions)
        {
            VerticesVector3Indices r=new VerticesVector3Indices();
            
            // Reservamos memoria para los vértices
            int numVerticesPerCircle = numSubdivisions;
            int numVertices = numVerticesPerCircle * 3;
            r.Vertices = new Vector3[numVertices];

            // Reservamos memoria para los índices
            int numIndicesPerCircle = numVerticesPerCircle * 2;
            int numIndices = numIndicesPerCircle * 3;
            r.Indices = new uint[numIndices];

            int c1_offs = 0;
            int c2_offs = numVerticesPerCircle;
            int c3_offs = numVerticesPerCircle * 2;

            // Definimos los índices
            for (int i = 0; i < numVerticesPerCircle; i++)
            {
                int idx1, idx2;

                idx1 = i;

                if (i < numVerticesPerCircle - 1)
                    idx2 = i + 1;
                else
                    idx2 = 0;

                r.Indices[c1_offs * 2 + i * 2 + 0] = (ushort)(idx1 + c1_offs);
                r.Indices[c1_offs * 2 + i * 2 + 1] = (ushort)(idx2 + c1_offs);

                r.Indices[c2_offs * 2 + i * 2 + 0] = (ushort)(idx1 + c2_offs);
                r.Indices[c2_offs * 2 + i * 2 + 1] = (ushort)((idx2 > 0 ? idx2 : 0) + c2_offs);

                r.Indices[c3_offs * 2 + i * 2 + 0] = (ushort)(idx1 + c3_offs);
                r.Indices[c3_offs * 2 + i * 2 + 1] = (ushort)((idx2 > 0 ? idx2 : 0) + c3_offs);
            }

            float delta = (float)(2.0 * Math.PI) / numVerticesPerCircle;

            for (int i = 0; i < numVerticesPerCircle; i++)
            {
                float radians = delta * i;
                float cos = (float)Math.Cos(radians) * Radius;
                float sin = (float)Math.Sin(radians) * Radius;

                r.Vertices[c1_offs + i] = new Vector3(cos + center.X, sin + center.Y, center.Z);
                r.Vertices[c2_offs + i] = new Vector3(cos + center.X, center.Y, sin + center.Z);
                r.Vertices[c3_offs + i] = new Vector3(center.X, cos + center.Y, sin + center.Z);
            }

            return r;
        }


        #endregion

		#region Operators

        /// <summary>
        /// Buils a New HBoundingSphere that encapsulates the two ones.
        /// </summary>
        /// <param name="lhs"></param>
        /// <param name="rhs"></param>
        /// <returns></returns>
		public static BoundingSphere operator +(BoundingSphere lhs, BoundingSphere rhs)
		{
			BoundingSphere newBSphere = null;

			Vector3 centerDiff = rhs.Center - lhs.Center;
			float centersDist = centerDiff.LengthSquared();

			float radiusDiff = rhs.Radius - lhs.Radius;
			float radiusDiffSq = radiusDiff * radiusDiff;

            if (radiusDiffSq >= centersDist)
			{
				if (radiusDiff >= 0.0f)
					newBSphere = new BoundingSphere(rhs.Center, rhs.Radius);
				else
					newBSphere = new BoundingSphere(lhs.Center, lhs.Radius);
			}
			else
			{
				float dist = (float)Math.Sqrt(centersDist);
				float t = (dist + rhs.Radius - lhs.Radius) / (2 * dist);
				newBSphere = new BoundingSphere(lhs.Center + t * centerDiff, (dist + rhs.Radius + lhs.Radius) / 2);
			}

			return newBSphere;
		}

		#endregion
    }
}
