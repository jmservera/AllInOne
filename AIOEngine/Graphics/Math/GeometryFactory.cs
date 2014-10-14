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
    /// This class gives you basic geometry elements. Remember that as this engine is OGL complaint, the Face-Winding is Counter ClockWise
    /// </summary>
    public static class GeometryFactory
    {
        /// <summary>
        /// Creates a vertex buffer and an IndexBuffer for a 1 unit Cube without Normals and TexCoord
        /// </summary>
        /// <param name="device"></param>
        /// <param name="vb"></param>
        /// <param name="ib"></param>
        public static MeshBasic CreateCubeBasic(VisualEngine engine)
        {
            MeshBasic mesh = new MeshBasic(engine, 8, 36);

            Vector3[] vb = mesh.Position;

            vb[0] = new Vector3(-0.5f, 0.5f, -0.5f);
            vb[1] = new Vector3(0.5f, 0.5f, -0.5f);
            vb[2] = new Vector3(0.5f, -0.5f, -0.5f);
            vb[3] = new Vector3(-0.5f, -0.5f, -0.5f);

            vb[4] = new Vector3(-0.5f, 0.5f, 0.5f);
            vb[5] = new Vector3(0.5f, 0.5f, 0.5f);
            vb[6] = new Vector3(0.5f, -0.5f, 0.5f);
            vb[7] = new Vector3(-0.5f, -0.5f, 0.5f);

            uint[] ib = mesh.Indices;

            ib = new uint[36];

            // Front 
            ib[0] = 0;
            ib[1] = 1;
            ib[2] = 2;
            ib[3] = 0;
            ib[4] = 2;
            ib[5] = 3;

            // rear 
            ib[6] = 4;
            ib[7] = 6;
            ib[8] = 5;
            ib[9] = 4;
            ib[10] = 7;
            ib[11] = 6;

            // left 
            ib[12] = 4;
            ib[13] = 0;
            ib[14] = 3;
            ib[15] = 4;
            ib[16] = 3;
            ib[17] = 7;

            // right 
            ib[18] = 1;
            ib[19] = 5;
            ib[20] = 2;
            ib[21] = 2;
            ib[22] = 5;
            ib[23] = 6;

            // top 
            ib[24] = 0;
            ib[25] = 4;
            ib[26] = 1;
            ib[27] = 1;
            ib[28] = 4;
            ib[29] = 5;

            // bottom 
            ib[30] = 3;
            ib[31] = 2;
            ib[32] = 7;
            ib[33] = 7;
            ib[34] = 2;
            ib[35] = 6;

            return mesh;
        }

        /// <summary>
        /// Creates a centered rectangle using X,Y planes
        /// </summary>
        /// <param name="device"></param>
        /// <param name="material"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <param name="zvalue"></param>
        /// <returns></returns>
        public static MeshBasic CreateRectangleXY(VisualEngine engine, Numeric width, Numeric height, Numeric zvalue = 0)
        {
            MeshBasic mesh = new MeshBasic(engine, 4, 6);

            Vector3[] vb = mesh.Position;

            Vector3[] normal = mesh.Normal = new Vector3[mesh.NumVertices];

            Vector2[,] texCoord = mesh.TexCoord = new Vector2[1, mesh.NumVertices];

            uint[] ib = mesh.Indices;

            float wdiv2 = width / 2;
            float hdiv2 = height / 2;

            int i = 0;

            // Front 0
            vb[i] = new Vector3(0, 1, zvalue); normal[i] = new Vector3(0, 0, -1); texCoord[0, i] = new Vector2(0, 0); i++;
            vb[i] = new Vector3(1, 1, zvalue); normal[i] = new Vector3(0, 0, -1); texCoord[0, i] = new Vector2(1, 0); i++;
            vb[i] = new Vector3(1, 0, zvalue); normal[i] = new Vector3(0, 0, -1); texCoord[0, i] = new Vector2(1, 1); i++;
            vb[i] = new Vector3(0, 0, zvalue); normal[i] = new Vector3(0, 0, -1); texCoord[0, i] = new Vector2(0, 1); i++;

            for (i = 0; i < 4; i++)
            {
                vb[i].X *= width;
                vb[i].Y *= height;
                vb[i].X -= wdiv2;
                vb[i].Y -= hdiv2;
            }

            // OGL COUNTER-ClockWISE order

            ib[0] = 2;
            ib[1] = 1;
            ib[2] = 0;
            ib[3] = 3;
            ib[4] = 2;
            ib[5] = 0;

            return mesh;
        }

        /// <summary>
        /// Creates a RenderableMeshMultiMaterial for a 1 unit Cube with 6 faces
        /// Each face has is own TextureCoords, so you could assign one texture to each face
        /// Face 0 is Rear Face
        /// Face 1 is Front
        /// Face 2 is Top
        /// Face 3 is Bottom
        /// Face 4 is Left
        /// Face 5 is Right
        /// </summary>
        /// <param name="device"></param>
        public static RenderableMeshMultiMaterial CreateCube(VisualEngine engine)
        {

            MeshBasic mesh = new MeshBasic(engine, 24, 36);

            Vector3[] vb = mesh.Position;

            Vector3[] normal = mesh.Normal = new Vector3[mesh.NumVertices];

            Vector2[,] texCoord = mesh.TexCoord = new Vector2[1, mesh.NumVertices];

            mesh.Tangent = new Vector3[mesh.NumVertices];

            mesh.VertexColor = new Vector4[mesh.NumVertices];

            uint[] ib = mesh.Indices;

            uint i = 0;

            // Rear 0
            vb[i] = new Vector3(0.5f, 0.5f, -0.5f); normal[i] = new Vector3(0, 0, -1); texCoord[0, i] = new Vector2(0, 0); i++;
            vb[i] = new Vector3(-0.5f, 0.5f, -0.5f); normal[i] = new Vector3(0, 0, -1); texCoord[0, i] = new Vector2(1, 0); i++;
            vb[i] = new Vector3(-0.5f, -0.5f, -0.5f); normal[i] = new Vector3(0, 0, -1); texCoord[0, i] = new Vector2(1, 1); i++;
            vb[i] = new Vector3(0.5f, -0.5f, -0.5f); normal[i] = new Vector3(0, 0, -1); texCoord[0, i] = new Vector2(0, 1); i++;

            // Front 4
            vb[i] = new Vector3(-0.5f, 0.5f, 0.5f); normal[i] = new Vector3(0, 0, 1); texCoord[0, i] = new Vector2(0, 0); i++;
            vb[i] = new Vector3(0.5f, 0.5f, 0.5f); normal[i] = new Vector3(0, 0, 1); texCoord[0, i] = new Vector2(1, 0); i++;
            vb[i] = new Vector3(0.5f, -0.5f, 0.5f); normal[i] = new Vector3(0, 0, 1); texCoord[0, i] = new Vector2(1, 1); i++;
            vb[i] = new Vector3(-0.5f, -0.5f, 0.5f); normal[i] = new Vector3(0, 0, 1); texCoord[0, i] = new Vector2(0, 1); i++;

            // Top 8
            vb[i] = new Vector3(-0.5f, 0.5f, -0.5f); normal[i] = new Vector3(0, 1, 0); texCoord[0, i] = new Vector2(0, 0); i++;
            vb[i] = new Vector3(0.5f, 0.5f, -0.5f); normal[i] = new Vector3(0, 1, 0); texCoord[0, i] = new Vector2(1, 0); i++;
            vb[i] = new Vector3(0.5f, 0.5f, 0.5f); normal[i] = new Vector3(0, 1, 0); texCoord[0, i] = new Vector2(1, 1); i++;
            vb[i] = new Vector3(-0.5f, 0.5f, 0.5f); normal[i] = new Vector3(0, 1, 0); texCoord[0, i] = new Vector2(0, 1); i++;

            // Bottom 12
            vb[i] = new Vector3(0.5f, -0.5f, -0.5f); normal[i] = new Vector3(0, -1, 0); texCoord[0, i] = new Vector2(0, 0); i++;
            vb[i] = new Vector3(-0.5f, -0.5f, -0.5f); normal[i] = new Vector3(0, -1, 0); texCoord[0, i] = new Vector2(1, 0); i++;
            vb[i] = new Vector3(-0.5f, -0.5f, 0.5f); normal[i] = new Vector3(0, -1, 0); texCoord[0, i] = new Vector2(1, 1); i++;
            vb[i] = new Vector3(0.5f, -0.5f, 0.5f); normal[i] = new Vector3(0, -1, 0); texCoord[0, i] = new Vector2(0, 1); i++;

            // Left 16
            vb[i] = new Vector3(-0.5f, 0.5f, -0.5f); normal[i] = new Vector3(-1, 0, 0); texCoord[0, i] = new Vector2(0, 0); i++;
            vb[i] = new Vector3(-0.5f, 0.5f, 0.5f); normal[i] = new Vector3(-1, 0, 0); texCoord[0, i] = new Vector2(1, 0); i++;
            vb[i] = new Vector3(-0.5f, -0.5f, 0.5f); normal[i] = new Vector3(-1, 0, 0); texCoord[0, i] = new Vector2(1, 1); i++;
            vb[i] = new Vector3(-0.5f, -0.5f, -0.5f); normal[i] = new Vector3(-1, 0, 0); texCoord[0, i] = new Vector2(0, 1); i++;

            // Right 20
            vb[i] = new Vector3(0.5f, 0.5f, 0.5f); normal[i] = new Vector3(1, 0, 0); texCoord[0, i] = new Vector2(0, 0); i++;
            vb[i] = new Vector3(0.5f, 0.5f, -0.5f); normal[i] = new Vector3(1, 0, 0); texCoord[0, i] = new Vector2(1, 0); i++;
            vb[i] = new Vector3(0.5f, -0.5f, -0.5f); normal[i] = new Vector3(1, 0, 0); texCoord[0, i] = new Vector2(1, 1); i++;
            vb[i] = new Vector3(0.5f, -0.5f, 0.5f); normal[i] = new Vector3(1, 0, 0); texCoord[0, i] = new Vector2(0, 1); i++;

            for (int k = 0; k < i; k++)
            {

                // Tangent vectors are always the same for a cube

                mesh.Tangent[k] = new Vector3(1, 0, 0);

                mesh.VertexColor[k] = new Vector4(1, 1, 1, 1);
            }

            // OGL COUNTER-ClockWISE order

            i = 0;

            for (uint face = 0; face < 6; face++)
            {
                uint vertex = face * 4;

                ib[i++] = vertex + 2;
                ib[i++] = vertex + 1;
                ib[i++] = vertex + 0;
                ib[i++] = vertex + 2;
                ib[i++] = vertex + 0;
                ib[i++] = vertex + 3;
            }

            // Create the RenderableMeshPart, six parts, one part for each face
            RenderableMeshMultiMaterial renderable = new RenderableMeshMultiMaterial(mesh);

            renderable.Mesh.MeshParts = new List<RenderableMeshPart>(6);

            for (i = 0; i < 6; i++)
            {
                RenderableMeshPart meshPart = new RenderableMeshPart(6, i * 6, 0);

                renderable.Mesh.MeshParts.Add(meshPart);
            }

            return renderable;

        }

        public static MeshBasic CreateSphere(VisualEngine engine, float diameter, int tessellation)
        {
            if (tessellation < 3)
                throw new Exception("tesselation parameter out of range");

            int verticalSegments = tessellation;
            int horizontalSegments = tessellation * 2;

            float radius = diameter / 2;

            MeshBasic mesh = new MeshBasic(engine, (uint)((verticalSegments + 1) * (horizontalSegments + 1)), 6 * (uint)(verticalSegments * (horizontalSegments + 1)));

            Vector3[] vertices = mesh.Position;

            Vector3[] normal = mesh.Normal = new Vector3[mesh.NumVertices];

            Vector2[,] texCoord = mesh.TexCoord = new Vector2[1, mesh.NumVertices];

            mesh.Tangent = new Vector3[mesh.NumVertices];

            mesh.VertexColor = new Vector4[mesh.NumVertices];

            uint[] indices = mesh.Indices;
            uint vertexCount = 0;

            // Create rings of vertices at progressively higher latitudes.
            for (int i = 0; i <= verticalSegments; i++)
            {
                float v = 1 - (float)i / verticalSegments;

                float latitude = (i * MathHelper.Pi / verticalSegments) - MathHelper.PiOver2;
                float dy = 0, dxz = 0;

                MathHelper.XMScalarSinCos(ref dy, ref dxz, latitude);

                // Create a single ring of vertices at this latitude.
                for (int j = 0; j <= horizontalSegments; j++)
                {
                    float u = (float)j / horizontalSegments;

                    float longitude = j * MathHelper.TwoPi / horizontalSegments;
                    float dx = 0, dz = 0;

                    MathHelper.XMScalarSinCos(ref dx, ref dz, longitude);

                    dx *= dxz;
                    dz *= dxz;

                    normal[vertexCount] = new Vector3(dx, dy, dz);
                    vertices[vertexCount] = normal[vertexCount] * radius;
                    texCoord[0, vertexCount++] = new Vector2(u, v);
                }
            }

            // Fill the index buffer with triangles joining each pair of latitude rings.
            int stride = horizontalSegments + 1;

            int indexCount = 0;

            for (int i = 0; i < verticalSegments; i++)
            {
                for (int j = 0; j <= horizontalSegments; j++)
                {
                    int nextI = i + 1;
                    int nextJ = (j + 1) % stride;
                    /* Left Handled */
                    /*
                    indices[indexCount++] = (uint)(i * stride + j);
                    indices[indexCount++] = (uint)(nextI * stride + j);
                    indices[indexCount++] = (uint)(i * stride + nextJ);

                    indices[indexCount++] = (uint)(i * stride + nextJ);
                    indices[indexCount++] = (uint)(nextI * stride + j);
                    indices[indexCount++] = (uint)(nextI * stride + nextJ);
                    */
                    /* Right Handled */
                    indices[indexCount + 2] = (uint)(i * stride + j);
                    indices[indexCount + 1] = (uint)(nextI * stride + j);
                    indices[indexCount] = (uint)(i * stride + nextJ);
                    indexCount += 3;
                    indices[indexCount + 2] = (uint)(i * stride + nextJ);
                    indices[indexCount + 1] = (uint)(nextI * stride + j);
                    indices[indexCount] = (uint)(nextI * stride + nextJ);
                    indexCount += 3;
                }
            }

            for (int k = 0; k < mesh.NumVertices; k++)
            {

                // Tangent vectors are always the same for a cube

                mesh.Tangent[k] = new Vector3(1, 0, 0);

                mesh.VertexColor[k] = new Vector4(1, 1, 1, 1);
            }

            return mesh;
        }

        public static MeshBasic CreateTorus(VisualEngine engine, float diameter, float thickness, int tessellation)
        {
            if (tessellation < 3)
                throw new Exception("tesselation parameter out of range");

            int verticalSegments = tessellation;
            int horizontalSegments = tessellation;

            uint numVertices=(uint)((verticalSegments + 1) * (horizontalSegments + 1));

            float radius = diameter / 2;

            MeshBasic mesh = new MeshBasic(engine, numVertices, 6 * numVertices);

            Vector3[] vertices = mesh.Position;

            mesh.Normal = new Vector3[mesh.NumVertices];

            mesh.TexCoord = new Vector2[1, mesh.NumVertices];

            mesh.Tangent = new Vector3[mesh.NumVertices];

            mesh.VertexColor = new Vector4[mesh.NumVertices];

            uint[] indices = mesh.Indices;
            uint vertexCount = 0;
            uint indexCount = 0;

            int stride = tessellation + 1;

            // First we loop around the main ring of the torus.
            for (int i = 0; i <= tessellation; i++)
            {
                float u = (float)i / tessellation;

                float outerAngle = i * MathHelper.TwoPi / tessellation - MathHelper.PiOver2;

                // Create a transform matrix that will align geometry to
                // slice perpendicularly though the current ring position.
                Matrix transform = Matrix.CreateTranslation(diameter / 2, 0, 0) * Matrix.CreateRotationY(outerAngle);

                // Now we loop along the other axis, around the side of the tube.
                for (int j = 0; j <= tessellation; j++)
                {
                    float v = 1 - (float)j / tessellation;

                    float innerAngle = j * MathHelper.TwoPi / tessellation + MathHelper.PiOver2;
                    float dx = 0, dy = 0;

                    MathHelper.XMScalarSinCos(ref dy, ref dx, innerAngle);

                    // Create a vertex.
                    Vector3 normal = new Vector3(dx, dy, 0);
                    Vector3 position = normal * thickness / 2;

                    mesh.Position[vertexCount] = Vector3.Transform(position, transform);
                    mesh.Normal[vertexCount] = Vector3.TransformNormal(normal, transform);
                    mesh.TexCoord[0, vertexCount] = new Vector2(u, v);

                    vertexCount++;

                    // And create indices for two triangles.
                    int nextI = (i + 1) % stride;
                    int nextJ = (j + 1) % stride;

                    mesh.Indices[indexCount + 2] = (uint)(i * stride + j);
                    mesh.Indices[indexCount + 1] = (uint)(i * stride + nextJ);
                    mesh.Indices[indexCount + 0] = (uint)(nextI * stride + j);

                    indexCount += 3;

                    mesh.Indices[indexCount + 2] = (uint)(i * stride + nextJ);
                    mesh.Indices[indexCount + 1] = (uint)(nextI * stride + nextJ);
                    mesh.Indices[indexCount + 0] = (uint)(nextI * stride + j);

                    indexCount += 3;
                }
            }

            for (int k = 0; k < mesh.NumVertices; k++)
            {

                // Tangent vectors are always the same for a cube

                mesh.Tangent[k] = new Vector3(1, 0, 0);

                mesh.VertexColor[k] = new Vector4(1, 1, 1, 1);
            }

            return mesh;

        }
    }
}
