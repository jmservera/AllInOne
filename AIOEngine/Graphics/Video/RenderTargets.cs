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
    public class RenderTargets
    {
        /// <summary>The Engine instance asociated to this instance</summary>
        public VisualEngine Engine { get; private set; }

        public List<RenderTarget> Items;

        /// <summary>
        /// Default constructor
        /// </summary>
        /// <param name="engine"></param>
        internal RenderTargets(VisualEngine engine)
        {
            Engine = engine;

            Items = new List<RenderTarget>();
        }

        /// <summary>
        /// Creates a RenderTarget form a previously created C++ RenderTarget
        /// </summary>
        /// <param name="name"></param>
        /// <param name="internalRenderTarget"></param>
        /// <returns></returns>
        internal RenderTarget Create(StringBuilder name, GraphicsEngineSpace._RenderTarget internalRenderTarget)
        {
            RenderTarget renderTarget = new RenderTarget(Engine, name, internalRenderTarget);

            Items.Add(renderTarget);

            return renderTarget;
        }

        /// <summary>
        /// Creates a RenderTarget
        /// </summary>
        /// <param name="name"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        public RenderTarget Create(StringBuilder name, int width, int height)
        {
            RenderTarget renderTarget = new RenderTarget(Engine, name, width, height);

            Items.Add(renderTarget);

            return renderTarget;
        }

        /// <summary>
        /// Used when you want to destroy a RenderTarget
        /// </summary>
        /// <param name="rt"></param>
        public void DestroyAndRemove(RenderTarget rt)
        {
            rt.Destroy();

            Items.Remove(rt);
        }

        /// <summary>
        /// Destroy & Releases memory and resources
        /// </summary>
        public void Destroy()
        {
            foreach (RenderTarget rt in Items)
            {
                rt.Destroy();
            }

            Items.Clear();
        }

    }
}
