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
	public class Frustum : ICloneable
	{
		#region Fields

		/// <summary>Planos que conforman el frustum</summary>
		private List<Plane> planes;
		/// <summary>Device-compliant clipping planes</summary>
		private Plane[] devicePlanes;

		#endregion

		#region Propiedades

		/// <summary>
		/// Retorna los planos que conforman el frustum
		/// </summary>
		/// <value>Lista que contiene los planos que conforman el frustum</value>
		public List<Plane> Planes
		{
			get
			{
				return planes;
			}
		}

		#endregion

		#region Constructores

		/// <summary>
		/// Crea una instancia de HFrustum
		/// </summary>
		public Frustum()
		{
			planes = new List<Plane>();
		}

		/// <summary>
		/// Crea una instancia de HFrustum a partir de una cámara
		/// </summary>
		public Frustum(Camera camera)
		{
			planes = new List<Plane>(6);

			for (int i = 0; i < 6; i++)
				planes.Add(new Plane());

			UpdateFromCamera(camera);
		}

		/// <summary>
		/// Crea una instancia de HFrustum a partir de una luz de tipo spot
		/// </summary>
		public Frustum(SpotLight spot)
		{
			planes = new List<Plane>(6);

			for (int i = 0; i < 6; i++)
				planes.Add(new Plane());

			UpdateFromSpotLight(spot);
		}


		#endregion

		#region Métodos

		/// <summary>
		/// Actualiza el frustum en base a una cámara
		/// </summary>
		public void UpdateFromCamera(Camera camera)
		{
			//System.Diagnostics.Debug.Assert(planes.Count == 6);

			Matrix viewProjection = camera.ViewProjection;

			planes.Clear();

			float A, B, C, D;
			Plane p;

			// Left clipping plane
			A = -(viewProjection.M14 + viewProjection.M11);
			B = -(viewProjection.M24 + viewProjection.M21);
			C = -(viewProjection.M34 + viewProjection.M31);
			D = -(viewProjection.M44 + viewProjection.M41);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Right clipping plane
			A = -(viewProjection.M14 - viewProjection.M11);
			B = -(viewProjection.M24 - viewProjection.M21);
			C = -(viewProjection.M34 - viewProjection.M31);
			D = -(viewProjection.M44 - viewProjection.M41);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Top clipping plane
			A = -(viewProjection.M14 - viewProjection.M12);
			B = -(viewProjection.M24 - viewProjection.M22);
			C = -(viewProjection.M34 - viewProjection.M32);
			D = -(viewProjection.M44 - viewProjection.M42);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Bottom clipping plane
			A = -(viewProjection.M14 + viewProjection.M12);
			B = -(viewProjection.M24 + viewProjection.M22);
			C = -(viewProjection.M34 + viewProjection.M32);
			D = -(viewProjection.M44 + viewProjection.M42);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Near clipping plane
			A = -(viewProjection.M14 + viewProjection.M13);
			B = -(viewProjection.M24 + viewProjection.M23);
			C = -(viewProjection.M34 + viewProjection.M33);
			D = -(viewProjection.M44 + viewProjection.M43);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Far clipping plane
			A = -(viewProjection.M14 - viewProjection.M13);
			B = -(viewProjection.M24 - viewProjection.M23);
			C = -(viewProjection.M34 - viewProjection.M33);
			D = -(viewProjection.M44 - viewProjection.M43);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Create device clipping planes
			//CreateDeviceClipPlanes();
		}

		/// <summary>
		/// Actualiza el frustum en base a una luz spot
		/// </summary>
		public void UpdateFromSpotLight(SpotLight spot)
		{
			//System.Diagnostics.Debug.Assert(planes.Count == 6);

            Matrix viewProjection = Matrix.Invert(spot.LocalMatrix) * spot.ProjectorPerspective;

			planes.Clear();

			float A, B, C, D;
			Plane p;

			// Left clipping plane
			A = -(viewProjection.M14 + viewProjection.M11);
			B = -(viewProjection.M24 + viewProjection.M21);
			C = -(viewProjection.M34 + viewProjection.M31);
			D = -(viewProjection.M44 + viewProjection.M41);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Right clipping plane
			A = -(viewProjection.M14 - viewProjection.M11);
			B = -(viewProjection.M24 - viewProjection.M21);
			C = -(viewProjection.M34 - viewProjection.M31);
			D = -(viewProjection.M44 - viewProjection.M41);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Top clipping plane
			A = -(viewProjection.M14 - viewProjection.M12);
			B = -(viewProjection.M24 - viewProjection.M22);
			C = -(viewProjection.M34 - viewProjection.M32);
			D = -(viewProjection.M44 - viewProjection.M42);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Bottom clipping plane
			A = -(viewProjection.M14 + viewProjection.M12);
			B = -(viewProjection.M24 + viewProjection.M22);
			C = -(viewProjection.M34 + viewProjection.M32);
			D = -(viewProjection.M44 + viewProjection.M42);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Near clipping plane
			A = -(viewProjection.M14 + viewProjection.M13);
			B = -(viewProjection.M24 + viewProjection.M23);
			C = -(viewProjection.M34 + viewProjection.M33);
			D = -(viewProjection.M44 + viewProjection.M43);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Far clipping plane
			A = -(viewProjection.M14 - viewProjection.M13);
			B = -(viewProjection.M24 - viewProjection.M23);
			C = -(viewProjection.M34 - viewProjection.M33);
			D = -(viewProjection.M44 - viewProjection.M43);
			p = new Plane(A, B, C, D);
			p.Normalize();
			planes.Add(p);

			// Create device clipping planes
			//CreateDeviceClipPlanes();

		}

		/// <summary>
		/// Comprueba si un punto está dentro o fuera del frustum
		/// </summary>
		/// <param name="min">Punto del cual tendremos que averiguar si está dentro o fuera del frustum</param>
		/// <returns>Devuelve true si el punto está dentro del frustum, de lo contrario devuelve false</returns>
		public bool PointInside(Vector3 point)
		{
			if (planes.Count < 3)
				return false;

			foreach (Plane plane in planes)
				if (plane.Dot(point) > 0.0f)
					return false;

			return true;
		}


		/// <summary>
		/// Comprueba si un bounding box (BBox) está dentro o fuera del frustum
		/// </summary>
        /// <param name="bbox">el BBox que se desea comprobar</param>
		/// <param name="max">Punto máximo del BBox</param>
		/// <returns>Devuelve true si el BBox está dentro del frustum, de lo contrario devuelve false</returns>
        public bool BBoxInside(BoundingBox bbox)
        {
            return BBoxInside(bbox.Min, bbox.Max);
        }

		/// <summary>
		/// Comprueba si un bounding box (BBox) está dentro o fuera del frustum
		/// </summary>
		/// <param name="min">Punto mínimo del BBox</param>
		/// <param name="max">Punto máximo del BBox</param>
		/// <returns>Devuelve true si el BBox está dentro del frustum, de lo contrario devuelve false</returns>
		public bool BBoxInside(Vector3 min, Vector3 max)
		{
			if (planes.Count < 3)
				return false;

			Vector3 nearPoint;

			foreach (Plane plane in planes)
			{
				if (plane.A > 0.0f)
				{
					if (plane.B > 0.0f)
					{
						if (plane.C > 0.0f)
						{
							nearPoint.X = min.X;
							nearPoint.Y = min.Y;
							nearPoint.Z = min.Z;
						}
						else
						{
							nearPoint.X = min.X;
							nearPoint.Y = min.Y;
							nearPoint.Z = max.Z;
						}
					}
					else
					{
						if (plane.C > 0.0f)
						{
							nearPoint.X = min.X;
							nearPoint.Y = max.Y;
							nearPoint.Z = min.Z;
						}
						else
						{
							nearPoint.X = min.X;
							nearPoint.Y = max.Y;
							nearPoint.Z = max.Z;
						}
					}
				}
				else
				{
					if (plane.B > 0.0f)
					{
						if (plane.C > 0.0f)
						{
							nearPoint.X = max.X;
							nearPoint.Y = min.Y;
							nearPoint.Z = min.Z;
						}
						else
						{
							nearPoint.X = max.X;
							nearPoint.Y = min.Y;
							nearPoint.Z = max.Z;
						}
					}
					else
					{
						if (plane.C > 0.0f)
						{
							nearPoint.X = max.X;
							nearPoint.Y = max.Y;
							nearPoint.Z = min.Z;
						}
						else
						{
							nearPoint.X = max.X;
							nearPoint.Y = max.Y;
							nearPoint.Z = max.Z;
						}
					}
				}

				if (plane.Dot(nearPoint) > 0.0f)
					return false;
			}

			return true;
		}

		/// <summary>
		/// Comprueba si una esfera está dentro o fuera del frustum
		/// </summary>
		/// <param name="origin">Posición del origen de la esfera</param>
		/// <param name="radius">Radio de la esfera</param>
		/// <returns></returns>
		public bool BSphereInside(Vector3 origin, float radius)
		{
			if (planes.Count < 3)
				return false;

			foreach (Plane plane in planes)
				if (plane.Dot(origin) > radius)
					return false;

			return true;
		}
        /*
		public Vector3[] ClipToFrustum(Vector3[] vertices)
		{
			foreach (Plane plane in Planes)
			{
				vertices = HMath.ClipToPlane(vertices, Plane.Scale(plane, -1.0f));

				if (vertices == null)
					return null;
			}

			return vertices;
		}
        
		/// <summary>
		/// Create device compliant clipping planes for this frustum
		/// </summary>
		public void CreateDeviceClipPlanes()
		{
			if (Haddd.Scene.Camera == null)
				return;

			int numPlanes = planes.Count > 6 ? 6 : planes.Count;

			devicePlanes = new Plane[numPlanes];

			Matrix view = Haddd.Scene.Camera.View;
			view.Invert();
			view.Transpose(view);

			Matrix projection = Haddd.Scene.Camera.Projection;
			projection.Invert();
			projection.Transpose(projection);

			Matrix transform = view * projection;

			for (int i = 0; i < numPlanes; i++)
			{
				// Invert plane so normal points inwards
				Plane auxPlane = planes[i];
				auxPlane.A = -auxPlane.A;
				auxPlane.B = -auxPlane.B;
				auxPlane.C = -auxPlane.C;
				auxPlane.D = -auxPlane.D;

				devicePlanes[i] = Plane.Transform(auxPlane, transform);
			}
		}

		/// <summary>
		/// Enables device clipping planes
		/// </summary>
		public void EnableDeviceClipPlanes()
		{
			for (int i = 0; i < devicePlanes.Length; i++)
				Haddd.Video.Device.ClipPlanes[i].Plane = devicePlanes[i];

			Haddd.Video.Device.ClipPlanes.EnableAll();
		}

		/// <summary>
		/// Disables device clipping planes
		/// </summary>
		public void DisableDeviceClipPlanes()
		{
			Haddd.Video.Device.ClipPlanes.DisableAll();
		}
        */
		#endregion

		#region Implementación de la interface ICloneable

		/// <summary>
		/// Crea una copia exacta de esta instancia del frustum.
		/// </summary>
		/// <returns>Retorna una copia exacta de esta instancia del frustum.</returns>
		public object Clone()
		{
			Frustum oFrustum = new Frustum();

			oFrustum.planes = new List<Plane>(planes.Count);

			foreach (Plane plane in planes)
				oFrustum.planes.Add(plane);

			return oFrustum;
		}

		#endregion
	}
}