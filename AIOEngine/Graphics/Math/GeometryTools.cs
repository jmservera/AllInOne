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
    public struct VerticesVector3Indices
    {
        public Vector3[] Vertices;
        public UInt32[] Indices;
    }

    /// <summary>
    /// This class gives you basic geometry elements. Remember that as this engine is OGL complaint, the Face-Winding is Counter ClockWise
    /// </summary>
    public static class GeometryTools
    {
        /// <summary>
        /// Hallar el boundingBox y el boundingSphere de un mesh.
        /// </summary>
        public static void ComputeBounding(MeshBasic mesh, BoundingBox boundingBox, BoundingSphere boundingEsfera)
        {
            boundingBox.Reset();

            Vector3 centro = new Vector3(0f, 0f, 0f); float radio = 0f;


                for (int c = 0; c < mesh.Position.Length; c++)
                {

                    boundingBox.Check(ref mesh.Position[c]);

                    centro += mesh.Position[c];

                }

                // Una vez que tenemos el centro, calculamos el punto que está más alejado

                centro.X /= mesh.NumVertices;
                centro.Y /= mesh.NumVertices;
                centro.Z /= mesh.NumVertices;

                Vector3 pto = new Vector3();

                for (int c = 0, p = 0; c < mesh.Position.Length; c++)
                {

                    pto =  mesh.Position[c] - centro;

                    float distancia = pto.LengthSquared();

                    if (distancia > radio) radio = distancia;

                }

            boundingEsfera.Center = centro;
            boundingEsfera.Radius = (float)Math.Sqrt((double)radio);
        }
         
    }
}
