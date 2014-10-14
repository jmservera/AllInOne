using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using AIOEngine;
using AIOEngine.MathSpace;

#region NUMERICAL DEFINITION FOR FLOAT OR DOUBLE

using Numeric = System.Single;

#endregion

namespace AIOEngine.Graphics
{

    /// <summary>
    /// An sprite is a 3d object ( it has Z!!!) but the coordinates are in Screen Space.
    /// The coordinates for X,Y,Z are based on the center object, not the top left corner
    /// </summary>
    public class RenderableMesh : IRenderable
    {
        /// <summary>
        /// The Mesh contains the vertices information
        /// </summary>
        public MeshBasic Mesh;

        /// <summary>
        /// The material used for rendering
        /// </summary>
        public Material Material;

        /// <summary>
        /// Indicates if the object is visible or not ( if it must be rendered or not)
        /// </summary>
        public bool Visible { get; set; }

        /// <summary>
        /// Indicates if we must Draw the Bounding Box and Bounding Sphere. 
        /// </summary>
        public bool DrawBounding { get; set; }

        /// <summary>
        /// Default constructor.
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="material"></param>
        public RenderableMesh(VisualEngine engine, MeshBasic mesh)
            : base()
        {
            this.Mesh = mesh;

            // Informs that the mesh has changed to update the internal buffers
            // We do not have to do it each frame if it's not necessary
            Mesh.Changed = true;

            Visible = true;
        }

        /// <summary>
        /// Destroy the internal objects and releases memory
        /// </summary>
        internal void Destroy()
        {
            if (Mesh != null)
            {
                Mesh.Destroy();

                Mesh = null;
            }
        }

        /// <summary>
        /// This is called by the scene class for each node
        /// </summary>
        /// <param name="node"></param>
        public virtual void Update(Node node)
        {
            Mesh.Update();
        }

        public void Draw(Scene scene,Node node, Camera camera)
        {
            if (Visible)
            {
                Mesh.Draw(node, camera, Material, scene.Lights);
            }

            if (DrawBounding)
            {
                Mesh.DrawBounding(ref node.WorldMatrix, ref camera.View, ref camera.Projection);
            }
        }

     }
}
