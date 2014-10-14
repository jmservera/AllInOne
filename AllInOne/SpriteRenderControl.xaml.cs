using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using GraphicsEngineSpace;
using AIOEngine.Graphics;
using System.ComponentModel;

namespace AllInOne
{
    /// <summary>
    /// Lógica de interacción para TestRenderControl.xaml
    /// </summary>
    public partial class SpriteRenderControl : UserControl
    {
        #region Internal fields for Rendering

        private D3DImageEx m_d3DImageEx;
        private VisualEngine editorEngine;

        /// <summary>
        /// This is the backBuffer view linked to the DX9 Resource compatible with WPF.
        /// You must RENDER to renderView, and then when all the render is finished, the control will copy that View to backBufferView
        /// </summary>
        private RenderView backBufferView;

        /// <summary>
        /// This is the backBuffer Render Target linked to the DX9 Resource compatible with WPF.
        /// You must RENDER to renderView, and then when all the render is finished, the control will copy that View to backBufferView
        /// </summary>
        private RenderTarget backBufferRenderTarget;

        /// <summary>
        /// The renderView that will contain all the renders and will be presented. This is the one the developer must use to show the render
        /// </summary>
        RenderView renderView;
        /// <summary>
        /// The renderTarget that will contain all the renders and will be presented. This is the one the developer must use to show the render
        /// </summary>
        RenderTarget renderRenderTarget;
        
        /// <summary>
        /// The name that will be used for debugging purposes
        /// </summary>
        StringBuilder renderName;

        /// <summary>
        /// This  list of actions will be called each time a resize happens
        /// </summary>
        List<Action<int, int>> onResizeMethods = new List<Action<int, int>>();

        /// <summary>
        /// Timer used for the engine to refresh the control
        /// </summary>
        System.Windows.Threading.DispatcherTimer dispatcherTimer;

        #endregion

        Scene scene;
        OrthoCamera camera;
        Node node;
        Sprite sp;

        public SpriteRenderControl(VisualEngine engine, StringBuilder renderName)
        {
            this.editorEngine = engine;

            this.renderName = renderName;

            InitializeComponent();

            if (DesignerProperties.GetIsInDesignMode(this))
                return;

            Loaded += Window1_Loaded;
            Unloaded += RenderControl_Unloaded;
        }

        /// <summary>
        /// This method will be called when the RenderTarget and views are created.
        /// Here you must create the scene, cameras and all the elements that you need
        /// </summary>
        private void OnInitializeRender()
        {
            // To render sprites, we do not need the depth
            renderView.State.DepthEnabled = false;

            // A new Scene!!
            scene = new Scene();

            camera = new OrthoCamera(new StringBuilder("camera"));
            
            renderView.State.CullMode = CULLMODE_STATE.NONE;

            // Each scene is composed of nodes

            scene.Root.AddChildren(node=new Node());

            // We need a material for this sprite

            Material material = new Material("standard");
            MaterialLayer layer = material.CreateLayer();

            layer.DiffuseMap = editorEngine.Textures.CreateTexture2DFromFile(new StringBuilder("stone"));

            // Creates a new sprite
            sp = editorEngine.Sprites.Create(100, 100);
            sp.Material = material;

            // Add the sprite to the node
            node.AddRenderable(sp);

        }

        /// <summary>
        /// This method will be called when the control is unloaded
        /// </summary>
        private void OnDestroy()
        {
            if(sp!=null)
                sp.Mesh.Destroy();

            editorEngine.RenderViews.DestroyAndRemove(renderView);

            editorEngine.RenderViews.DestroyAndRemove(backBufferView);
        }

        float x=0;

        private void OnUpdate()
        {
            
            //node.Position = new AIOEngine.MathSpace.Vector3(x, 0, 0);
            
            //camera.Position = new AIOEngine.MathSpace.Vector3(x, 0, 0);

            node.AngleZInDegrees = x;

            //x ++;
            
            camera.Update();
            
            // Update the scene
            scene.Update();
        }

        /// <summary>
        /// This is the Render method
        /// </summary>
        private void OnRender()
        {
            // Set this renderView for rendering
            renderView.SetToDevice();

            // Fill it whith a Red color
            renderRenderTarget.Clear(1, 0, 0, 1);

            // We need to update the camera each frame

            camera.UpdateProjection(renderView.RenderTarget.Width, renderView.RenderTarget.Height);

            // Draw the scene
            scene.Draw(camera);

        }

        #region Internal methods for rendering

        private void RenderControl_Closed(object sender, EventArgs e)
        {
            RenderControl_Unloaded(sender, null);
        }

        void RenderControl_Unloaded(object sender, RoutedEventArgs e)
        {
            dispatcherTimer.Stop();

            CompositionTarget.Rendering -= CompositionTarget_Rendering;

            OnDestroy();

        }

        void Window1_Loaded(object sender, RoutedEventArgs e)
        {
            // The unloaded method is not working, so we attach to the parent window closing event the code to Destroy the resources
            Window.GetWindow(this).Closing += (s1, e1) => RenderControl_Closed(s1, e1);

            // We can use any resolution, because the OnResize event will be fired and the right dimensions will be set

            int anyWidth = 640;

            int anyHeight = 480;

            // For WPF we need to create a DX9RenderTarget. 
            backBufferRenderTarget = editorEngine.CreateDX9SharedRenderTarget(renderName+".DX9BackBuffer",anyWidth, anyHeight);

            // We need a View for Each RenderTarget. A View is the sum of a ViewPort, a RenderTarget and a DepthStencil
            // We do not need a DepthStencil for the Backbuffer has it's only used to copy the contents of the Render to a DX9 shader resource

            backBufferView = editorEngine.RenderViews.Create(backBufferRenderTarget);

            // We do not need to use the Depth because when all the render is ready, the renderview is copied to the backbufferview
            backBufferView.State.DepthEnabled = false;

            // When a resize of this control happens, i need to recreate this view
            onResizeMethods.Add((w, h) => backBufferView.Resize(w, h));

            // The RenderTarget used for rendering

            renderRenderTarget = editorEngine.RenderTargets.Create(renderName, anyWidth, anyHeight);

            renderView = editorEngine.RenderViews.Create(renderRenderTarget);

            renderView.DepthStencil = editorEngine.DepthStencils.Create(new StringBuilder("MainDepthStencil"), renderView.RenderTarget.Width, renderView.RenderTarget.Height);

            // When a resize of this control happens, i need to recreate this view
            onResizeMethods.Add((w, h) => renderView.Resize(w, h));

            /* Create a new D3DImageEx class */
            m_d3DImageEx = new D3DImageEx();

            /* Set our image's source to our D3DImage9Ex */
            d3dScene.Source = m_d3DImageEx;

            /* Set the backbuffer, which is a ID3D10Texture2D pointer */
            m_d3DImageEx.SetBackBufferEx(D3DResourceTypeEx.ID3D10Texture2D, backBufferRenderTarget.Texture.InternalTexture2D.GetTexturePtr());

            OnInitializeRender();

            // Create the timer

            dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = TimeSpan.FromMilliseconds(33);
            dispatcherTimer.Start();

            // Add the OnDraw event for this control
            CompositionTarget.Rendering += CompositionTarget_Rendering;

        }

        /// <summary>
        /// Timer event. It only calls InvalidateVisual
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            InvalidateVisual();
        }

        /// <summary>
        /// Invalidates entire D3DImage area
        /// </summary>
        private void InvalidateD3DImage()
        {
            m_d3DImageEx.Lock();
            m_d3DImageEx.AddDirtyRect(new Int32Rect()
            {
                X = 0,
                Y = 0,
                Height = m_d3DImageEx.PixelHeight,
                Width = m_d3DImageEx.PixelWidth
            });
            m_d3DImageEx.Unlock();
        }

        private void CompositionTarget_Rendering(object sender, EventArgs e)
        {
            OnUpdate();

            OnRender();

            // Set the BackBuffer to Present the results
            backBufferView.SetToDevice();

            // Draw a Quad usign the render as a texture
            editorEngine.Device.DrawQuad(renderRenderTarget.Texture);

            /* Invalidate our D3DImage */
            InvalidateD3DImage();
        }

        private void d3dScene_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (DesignerProperties.GetIsInDesignMode(this))
                return;

            if (m_d3DImageEx == null) return;

            // Informs the methods that a resize has ocurred!!

            foreach (Action<int, int> action in onResizeMethods)
                action((int)e.NewSize.Width, (int)e.NewSize.Height);

            /* Set the backbuffer, which is a ID3D10Texture2D pointer */
            m_d3DImageEx.SetBackBufferEx(D3DResourceTypeEx.ID3D10Texture2D, backBufferRenderTarget.Texture.InternalTexture2D.GetTexturePtr());

        }

        #endregion

        private void UserControl_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            d3dScene_SizeChanged(sender, e);
        }
    }
}
