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
    public class RenderViews
    {
        /// <summary>The Engine instance asociated to this instance</summary>
        public VisualEngine Engine { get; private set; }

        public List<RenderView> Items;

        /// <summary>
        /// Default constructor
        /// </summary>
        /// <param name="engine"></param>
        internal RenderViews(VisualEngine engine)
        {
            Engine = engine;

            Items = new List<RenderView>();
        }

        /// <summary>
        /// Creates a RenderView based in a RenderTarget
        /// </summary>
        /// <param name="name">The name assigned</param>
        /// <param name="renderTarget"></param>
        /// <returns></returns>
        public RenderView Create(StringBuilder name, RenderTarget renderTarget)
        {
            RenderView renderView = new RenderView(name, renderTarget);

            Items.Add(renderView);

            return renderView;
        }

        /// <summary>
        /// Creates a RenderView based in a RenderTarget, using the RenderTarget.Name as the RenderView name
        /// </summary>
        /// <param name="name">The name assigned</param>
        /// <param name="renderTarget"></param>
        /// <returns></returns>
        public RenderView Create(RenderTarget renderTarget)
        {
            RenderView renderView = new RenderView(renderTarget.Name, renderTarget);

            Items.Add(renderView);

            return renderView;
        }
        /// <summary>
        /// Destroy and removes the Render Target and Depth Stencil if exists
        /// </summary>
        /// <param name="renderView"></param>
        public void DestroyAndRemove(RenderView renderView)
        {
            Engine.RenderTargets.DestroyAndRemove(renderView.RenderTarget);

            if(renderView.DepthStencil!=null)
                Engine.DepthStencils.DestroyAndRemove(renderView.DepthStencil);

            Items.Remove(renderView);

        }
    }
}
