using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using AIOEngine.MathSpace;

namespace AIOEngine.Graphics
{
    public class MeshBasic
    {
        /// <summary>
        /// Array of Vertices
        /// </summary>
        public Vector3[] Position;

        /// <summary>
        /// Array of Indices
        /// </summary>
        public uint[] Indices;

        public int NumVertices { get { return Position.Length; } }
        public int NumIndices { get { return Indices.Length; } }

        /// <summary>
        /// Array of Normals ( if present)
        /// </summary>
        public Vector3[] Normal;

        /// <summary>
        /// Array of tangents ( if present)
        /// </summary>
        public Vector3[] Tangent;

        /// <summary>
        /// Array of Colors ( if present)
        /// </summary>
        public Vector4[] VertexColor;

        /// <summary>
        /// Array of texture coords ( if present)
        /// </summary>
        public Vector2[,] TexCoord;

        public BoundingBox BoundingBox;
        public BoundingSphere BoundingSphere;

        /// <summary>
        /// Indicates that me have made some changes and we must rebuild the C++ object
        /// </summary>
        public bool Changed;

        public GraphicsEngineSpace._MeshBasic Internal;

        /// <summary>
        /// The render indices and count information for each part. This value is null when there is no meshparts. See <see cref="MeshMultiMaterial"/> for an example of using this field
        /// </summary>
        public List<RenderableMeshPart> MeshParts;

        VisualEngine engine;

        /// <summary>
        /// Constructs the instance without vertices/indices
        /// </summary>
        public MeshBasic(VisualEngine engine)
        {
            this.engine = engine;

            Internal = new GraphicsEngineSpace._MeshBasic(engine.Device);

            BoundingBox = new BoundingBox();

            BoundingSphere = new BoundingSphere();
        }

        public void Destroy()
        {
            if (Internal != null)
            {
                Internal.Destroy();

                Internal = null;
            }

        }
        /// <summary>
        /// Construct the instance with Vertices & Indices, but not Normals, neither Texture Coods
        /// </summary>
        /// <param name="numVertices"></param>
        /// <param name="numIndices"></param>
        public MeshBasic(VisualEngine engine,uint numVertices,uint numIndices):this(engine)
        {
            Position = new Vector3[numVertices];

            Indices = new uint[numIndices];
        }

        /// <summary>
        /// Computes the BoundingBox & BoundingSphere
        /// </summary>
        void ComputeBoundings()
        {
            GeometryTools.ComputeBounding(this, BoundingBox, BoundingSphere);
        }

        /// <summary>
        /// Update Boundings.
        /// </summary>
        public void Update()
        {
            if (!Changed) return;

            ComputeBoundings();
        }

        /// <summary>
        /// Update the internal buffers. This should be called before Draw
        /// </summary>
        void UpdateInternal()
        {
            if (!Changed) return;

            Internal.Update(Position, VertexColor, Normal, Tangent, TexCoord);
            
            Internal.Update(Indices);

            Changed = false;
        }

        /// <summary>
        /// Renders the mesh usign World matrix from Node, the View matrix from camera and the Material
        /// </summary>
        /// <param name="node"></param>
        /// <param name="camera"></param>
        /// <param name="material"></param>
        public void Draw(Node node, Camera camera, Material material, List<Light> lights = null)
        {
            Draw(ref node.WorldMatrix, ref camera.View, ref camera.Projection, material,camera,lights);
        }
        
        /// <summary>
        /// Renders the mesh usign World matrix from Node, the View matrix from camera and the Material
        /// </summary>
        /// <param name="node"></param>
        /// <param name="camera"></param>
        /// <param name="material"></param>
        public void Draw(Node node, Camera camera, Material material, RenderableMeshPart meshPart, List<Light> lights = null)
        {
            UpdateInternal();
            
            //Matrix matrix=Matrix.Identity;

            //Internal.Draw(ref matrix, ref matrix, ref matrix, material);

            Internal.Draw(ref node.WorldMatrix, ref camera.View, ref camera.Projection, material,camera, ref meshPart, lights);

        }

        /// <summary>
        /// Renders the mesh using the matrices and the material
        /// </summary>
        /// <param name="world"></param>
        /// <param name="view"></param>
        /// <param name="projection"></param>
        /// <param name="material"></param>
        public void Draw(ref Matrix world, ref Matrix view, ref Matrix projection, Material material, Camera camera, List<Light> lights = null)
        {
            UpdateInternal();

            Internal.Draw(ref world, ref view, ref projection, material,camera, lights);

        }

        /// <summary>
        /// Draws the Bounding Box and BoundingSphere
        /// </summary>
        /// <param name="world"></param>
        /// <param name="view"></param>
        /// <param name="projection"></param>
        public void DrawBounding(ref Matrix world, ref Matrix view, ref Matrix projection)
        {
            Vector3[] lines = BoundingBox.ToRenderLines();

            engine.DrawLines(lines, new Vector4(1, 0, 0, 1), ref world, ref view, ref projection);

            VerticesVector3Indices result = BoundingSphere.ToRenderLines(30);

            engine.DrawLines(result.Vertices, result.Indices, new AIOEngine.MathSpace.Vector4(0, 0, 1, 1), ref world, ref view, ref projection);

        }
    }
}
