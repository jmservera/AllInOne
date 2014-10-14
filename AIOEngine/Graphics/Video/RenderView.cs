using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

#region NUMERICAL DEFINITION FOR FLOAT OR DOUBLE

using Numeric = System.Single;

#endregion

using AIOEngine.MathSpace;
using GraphicsEngineSpace;

namespace AIOEngine.Graphics
{
    /// <summary>
    /// A RenderView sets the <see cref="RenderTarget"/>, <see cref="DepthStencil"/> and <see cref="ViewPort"/>, needed to Render to a buffer
    /// </summary>
    public class RenderView
    {
        readonly StringBuilder TAG = new StringBuilder("RenderView");

        public StringBuilder Name { get; private set; }
        
        public RenderTarget RenderTarget;

        public DepthStencil DepthStencil;

        /// <summary>
        /// If this value is null, the Viewport will get the Width and Height RenderTarget values
        /// </summary>
        public ViewPort ViewPort;

        /// <summary>
        /// The device states used when the render view is activated
        /// </summary>
        public DeviceState State;

        /// <summary>
        /// Creates a RenderView with a RenderTarget but without a DepthStencil
        /// </summary>
        /// <param name="name"></param>
        /// <param name="renderTarget"></param>
        internal RenderView(StringBuilder name,RenderTarget renderTarget)
        {
            Name = name;

            RenderTarget = renderTarget;

            RenderTarget.Engine.Log.AddInformation(TAG, new StringBuilder("Created " + Name));

            State = new DeviceState();

        }

        public void SetToDevice()
        {
            GraphicsDevice device = RenderTarget.Engine.Device;

            // Sets the states for this render
            device.SetDeviceState(State);

            if (DepthStencil == null)
            {
                device.SetRenderTargetAndStencil(RenderTarget.Internal, null);
                
            }
            else
            {
                device.SetRenderTargetAndStencil(RenderTarget.Internal, DepthStencil.Internal);
            }

            if (ViewPort == null)
            {
                device.SetViewPort(0, 0, RenderTarget.Width, RenderTarget.Height);
            }
            else
            {
                device.SetViewPort(ViewPort.X,ViewPort.Y,ViewPort.Width,ViewPort.Height,ViewPort.ZMin,ViewPort.ZMax);
            }
        }

        /// <summary>
        /// Clears the RenderTarget and DepthStencil ( if exists) with values Depth=1 and Stencil=0
        /// </summary>
        /// <param name="r"></param>
        /// <param name="g"></param>
        /// <param name="b"></param>
        /// <param name="a"></param>
        public void Clear(Numeric r, Numeric g, Numeric b, Numeric a)
        {
            RenderTarget.Clear(r, g, b, a);

            if(DepthStencil!=null)
                DepthStencil.Clear(1, 0);

        }

        /// <summary>
        /// Clears RenderTarget And DepthStencil
        /// </summary>
        /// <param name="r"></param>
        /// <param name="g"></param>
        /// <param name="b"></param>
        /// <param name="a"></param>
        /// <param name="depth">Defaults 1</param>
        /// <param name="stencil">Defaults 0</param>
        public void ClearColorAndDepthStencil(Numeric r, Numeric g, Numeric b, Numeric a, Numeric depth,int stencil)
        {
            RenderTarget.Clear(r, g, b, a);

            DepthStencil.Clear(depth,stencil);
        }

        /// <summary>
        /// Set the RenderTarget and Stencill to null
        /// </summary>
        public void SetToNull()
        {
            GraphicsDevice device = RenderTarget.Engine.Device;

            device.SetRenderTargetAndStencil(null, null);

        }

        /// <summary>
        /// YOU MUST call to this method when the backbuffer is resized
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        public void Resize(int width, int height)
        {
            if(RenderTarget!=null)
                RenderTarget.Resize(width, height);

            if (DepthStencil != null)
                DepthStencil.Resize(width, height);
        }
    }
}
