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
    public class Sprite : IRenderable
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
        /// The Z Value for this sprite. 
        /// </summary>
        public Numeric Z;

        /// <summary>
        /// Sprite Width in pixels
        /// </summary>
        public Numeric Width;

        /// <summary>
        /// Sprite Height in pixels
        /// </summary>
        public Numeric Height;

        /// <summary>
        /// Indicates if the object is visible or not ( if it must be rendered or not)
        /// </summary>
        public bool Visible { get; set; }

        /// <summary>
        /// Indicates if we must Draw the Bounding Box and Bounding Sphere. 
        /// </summary>
        public bool DrawBounding { get; set; }

        Numeric lastWidth, lastHeight;

        /// <summary>
        /// Default constructor.
        /// </summary>
        /// <param name="engine"></param>
        /// <param name="material"></param>
        internal Sprite(VisualEngine engine,int width=100,int height=100)
            : base()
        {
            Width = (Numeric)width;

            Height = (Numeric)height;

            this.Mesh = GeometryFactory.CreateRectangleXY(engine, Width, Height);

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
            // Check if we have to change something!!
            if (lastHeight == Height && lastWidth == Width) return;

            lastWidth = Width;

            lastHeight = Height;

            float w = Width;
            float h = Height;
            /*
            w = 0.5f;
            h = 0.5f;
            */
            Numeric wdiv2 = w / 2;
            Numeric hdiv2 = h / 2;

            Mesh.Position[0] = new Vector3(0, h, Z);
            Mesh.Position[1] = new Vector3(w, h, Z);
            Mesh.Position[2] = new Vector3(w, 0, Z);
            Mesh.Position[3] = new Vector3(0, 0, Z);

            for (int i = 0; i < 4; i++)
            {
                Mesh.Position[i].X -= wdiv2;
                Mesh.Position[i].Y -= hdiv2;
            }

            Mesh.Changed = true;
        }

        public void Draw(Scene scene,Node node, Camera camera)
        {
            if (Visible)
            {
                if (scene != null)
                    Mesh.Draw(node, camera, Material, scene.Lights);
                else
                    Mesh.Draw(node, camera, Material);
            }

            if (DrawBounding)
            {
                Mesh.DrawBounding(ref node.WorldMatrix, ref camera.View, ref camera.Projection);
            }

        }
    }
}
