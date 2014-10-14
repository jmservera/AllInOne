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
    /// Represents an element of the graph with Scale, Rotation and Translation
    /// </summary>
    public class Node:NodeBase
    {
 
        #region Public fields
 
        /// <summary>List of renderables. The <see cref=" Update"/> method will call Update for each renderable </summary>
        public List<IRenderable> Renderables;

        /// <summary>
        /// CustomData to add all you need!!
        /// </summary>
        public object CustomData;

        #endregion

        #region Constructor

        public Node():base()
        {
        }

        public Node(StringBuilder name):base(name)
        {
        }

        #endregion

        #region Methods

        /// <summary>
        /// This method is virtual, but you do not need to call to base.Update 
        /// </summary>
        public override void OnUpdate()
        {
            // Update the renderables
            if (Renderables != null)
            {
                foreach (IRenderable renderable in Renderables)
                {
                    renderable.Update(this);
                }
            }

        }

        public void Draw(Scene scene,Camera camera)
        {
            if (Renderables != null)
            {
                foreach (IRenderable renderable in Renderables)
                {
                    renderable.Draw(scene, this, camera);
                }
            }

            DrawChildren(scene,camera);
        }

        public void DrawChildren(Scene scene, Camera camera)
        {
            if (Children == null) return;

            foreach (NodeBase node in Children)
            {
                if(node is Node)
                    ((Node)node).Draw(scene,camera);
            }
        }

        public void AddRenderable(IRenderable renderable)
        {
            if (Renderables == null) 
                Renderables = new List<IRenderable>();
            
            Renderables.Add(renderable);

        }


        #endregion
    }
}
