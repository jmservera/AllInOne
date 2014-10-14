using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

#region NUMERICAL DEFINITION FOR FLOAT OR DOUBLE

using Numeric = System.Single;

#endregion

using AIOEngine.MathSpace;

namespace AIOEngine.Graphics
{
    public class DepthStencils
    {
        /// <summary>The Engine instance asociated to this instance</summary>
        public VisualEngine Engine { get; private set; }

        public List<DepthStencil> Items { get; private set; }

        public DepthStencils(VisualEngine engine)
        {
            Engine = engine;

            Items = new List<DepthStencil>();
        }

        /// <summary>
        /// Used when you want to destroy a DepthStencil
        /// </summary>
        /// <param name="ds"></param>
        public void DestroyAndRemove(DepthStencil ds)
        {
            ds.Destroy();

            Items.Remove(ds);
        }

        /// <summary>
        /// Destroy & Releases memory and resources
        /// </summary>
        public void Destroy()
        {
            foreach (DepthStencil ds in Items)
            {
                ds.Destroy();
            }

            Items.Clear();
        }

        /// <summary>
        /// Creates a new Depth Stencil
        /// </summary>
        /// <param name="name"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        public DepthStencil Create(StringBuilder name, int width, int height)
        {
            DepthStencil depthStencil = new DepthStencil(Engine,name, width, height);

            Items.Add(depthStencil);

            return depthStencil;
        }

    }
}
