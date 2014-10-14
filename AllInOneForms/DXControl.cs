using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using AIOEngine;
using AIOEngine.Graphics;
using AIOEngine.MathSpace;

namespace AllInOneForms
{
    public partial class DXControl : UserControl
    {
        VisualEngine editorEngine;

        /// <summary>
        /// The renderView that will contain all the renders and will be presented. This is the one the developer must use to show the render
        /// </summary>
        RenderView renderView;
        /// <summary>
        /// The renderTarget that will contain all the renders and will be presented. This is the one the developer must use to show the render
        /// </summary>
        RenderTarget renderRenderTarget;

        /// <summary>
        /// This  list of actions will be called each time a resize happens
        /// </summary>
        List<Action<int, int>> onResizeMethods = new List<Action<int, int>>();

        Scene scene;

        OrthoCamera camera;
        Node node;
        Sprite sp;

        public DXControl()
        {
            InitializeComponent();
        }

        private void DXControl_Load(object sender, EventArgs e)
        {
            if (this.DesignMode) return;

            editorEngine = new VisualEngine(new StringBuilder("Editor"));

            // Add a relative path where the textures are located
            editorEngine.Textures.AddRelativePath("Textures"); 

            // The RenderTarget used for rendering

            renderRenderTarget = editorEngine.CreateSwapChainRenderTarget("SwapChain",this.Handle);

            renderView = editorEngine.RenderViews.Create(renderRenderTarget);

            renderView.DepthStencil = editorEngine.DepthStencils.Create(new StringBuilder("MainDepthStencil"), renderView.RenderTarget.Width, renderView.RenderTarget.Height);

            // When a resize of this control happens, i need to recreate this view
            onResizeMethods.Add((w, h) => renderView.Resize(w, h));

            this.ParentForm.FormClosed += ParentForm_FormClosed;

            OnInitializeRender();
        }

        void ParentForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            OnDestroy();
        }
 
        /// <summary>
        /// This method will be called when the RenderTarget and views are created.
        /// Here you must create the scene, cameras and all the elements that you need
        /// </summary>
        private void OnInitializeRender()
        {
            // To render sprites, we do not need the depth
            renderView.State.DepthEnabled = false;
            //renderView.State.CullMode = CULLMODE_STATE.NONE;

            // A new Scene!!
            scene = new Scene();

            camera = new OrthoCamera(new StringBuilder("camera"));

            //return;

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

            //return;
            Timer timer = new Timer();
            timer.Tick += timer_Tick;
            timer.Enabled = true;
            timer.Start();
            timer.Interval = 1000;
        }

        void timer_Tick(object sender, EventArgs e)
        {
            node.AngleZ ++;

            OnUpdate();
            OnRender();
        }

        /// <summary>
        /// This method will be called when the control is unloaded
        /// </summary>
        private void OnDestroy()
        {
            if (sp != null)
                sp.Mesh.Destroy();
            
            editorEngine.RenderTargets.DestroyAndRemove(renderRenderTarget);

            editorEngine.DepthStencils.DestroyAndRemove(renderView.DepthStencil);

            editorEngine.Destroy();
        }

        float x = 0;

        private void OnUpdate()
        {

            //node.Position = new AIOEngine.MathSpace.Vector3(x, 0, 0);

            //camera.Position = new AIOEngine.MathSpace.Vector3(x, 0, 0);

            x += 0.5f;

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

            renderRenderTarget.Present();
        }

        private void DXControl_Paint(object sender, PaintEventArgs e)
        {
            if (this.DesignMode) return;
            
            OnUpdate();
            OnRender();
        }

        private void DXControl_Resize(object sender, EventArgs e)
        {
            if (this.DesignMode) return;

            // Informs the methods that a resize has ocurred!!

            foreach (Action<int, int> action in onResizeMethods)
                action(this.Width, this.Height);

        }
    }
}
