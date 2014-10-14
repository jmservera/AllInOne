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
    public class RenderableMeshMultiMaterial : IRenderable
    {
        /// <summary>
        /// The Mesh contains the vertices information
        /// </summary>
        public MeshBasic Mesh;

        /// <summary>
        /// The materials used for rendering
        /// </summary>
        public List<Material> Materials;

        /// <summary>
        /// Indicates if the object is visible or not ( if it must be rendered or not)
        /// </summary>
        public bool Visible { get; set; }
        
        /// <summary>
        /// Indicates if we must Draw the Bounding Box and Bounding Sphere. 
        /// </summary>
        public bool DrawBounding { get; set; }

        /// <summary>
        /// Default constructor. Automatically creates the <see cref="MeshParts"/> and <see cref="Materials"/> List with 0 elements.
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="material"></param>
        public RenderableMeshMultiMaterial(MeshBasic mesh)
        {
            this.Mesh = mesh;

            // Informs that the mesh has changed to update the internal buffers
            // We do not have to do it each frame if it's not necessary
            Mesh.Changed = true;

            Materials = new List<Material>();

            Visible = true;
        }

        /// <summary>
        /// Default constructor. Automatically creates the <see cref="MeshParts"/> and <see cref="Materials"/> List with 0 elements.
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="material"></param>
        public RenderableMeshMultiMaterial(RenderableMeshMultiMaterial source)
        {
            this.Mesh = source.Mesh;

            Materials = new List<Material>(source.Materials);

            Visible = source.Visible;
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

                int index = 0;

                //Mesh.Draw(node, camera, Materials[index], MeshParts[index]); return;

                foreach (Material material in Materials)
                {
                    Mesh.Draw(node, camera, material, Mesh.MeshParts[index], scene.Lights);

                    index++;
                }
            }
            
            if(DrawBounding)
            {
                Mesh.DrawBounding(ref node.WorldMatrix, ref camera.View, ref camera.Projection);
            }

        }
        /// <summary>
        /// Clones this object. All the properties will be cloned
        /// </summary>
        /// <returns></returns>
        public RenderableMeshMultiMaterial Clone()
        {
            RenderableMeshMultiMaterial newObject = new RenderableMeshMultiMaterial(this);

            return newObject;

        }
    }
}
