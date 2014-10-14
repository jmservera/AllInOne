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
    public partial class TestRenderControl : UserControl
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
        PerspectiveCamera camera;
        Node node,node2;
        RenderableMeshMultiMaterial cubeMesh;
        OmniLight light;
        SpotLight spot;
        RenderableMesh sphereMesh;

        public TestRenderControl(VisualEngine engine, StringBuilder renderName)
        {
            this.editorEngine = engine;

            this.renderName = renderName;

            AIOEngine.MathSpace.Matrix matrix = AIOEngine.MathSpace.Matrix.Identity;
            AIOEngine.MathSpace.Vector3 forward = matrix.Forward;
            AIOEngine.MathSpace.Vector3 up = matrix.Up;
            AIOEngine.MathSpace.Vector3 left = matrix.Left;

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

            // A new Scene!!
            scene = new Scene();

            camera = new PerspectiveCamera( new StringBuilder("camera"));

            camera.Position = new AIOEngine.MathSpace.Vector3(0, 10, 16);

            // Each scene is composed of nodes

            //AddSphere();
            AddTorus();

            AddCube();

            //AddOmniLight();

            AddSpotLight();

         }

        void AddCube()
        {
            scene.Root.AddChildren(node = new Node(new StringBuilder("Cube")));

            // A cube with six faces, each one with its own material

            cubeMesh = GeometryFactory.CreateCube(editorEngine);

            for (int numFace = 0; numFace < 6; numFace++)
            {
                Material material = new Material("standard");

                MaterialLayer layer = material.CreateLayer();

                layer.ShaderID = 0;

                layer.DiffuseMap = editorEngine.Textures.CreateTexture2DFromFile(new StringBuilder((numFace + 1).ToString()));

                if (numFace == 0)
                    layer.NormalMap = editorEngine.Textures.CreateTexture2DFromFile(new StringBuilder("bump_NM_height"));

                cubeMesh.Materials.Add(material);


            }
            node.ScaleX = 30;
            node.ScaleZ = 30;
            node.ScaleY = 0.01f;
            //mesh.Visible = false;

            node.AddRenderable(cubeMesh);

            // A new node
            scene.Root.AddChildren(node2 = new Node(new StringBuilder("clone")));

            node2.Position = new AIOEngine.MathSpace.Vector3(2, 1, 0);

            RenderableMeshMultiMaterial mesh2 = cubeMesh.Clone();

            //node2.AddRenderable(mesh2);

            mesh2.Visible = false;

            mesh2.DrawBounding = true;
        }

        void AddTorus()
        {
            scene.Root.AddChildren(node = new Node(new StringBuilder("Torus")));

            sphereMesh = new RenderableMesh(editorEngine, GeometryFactory.CreateTorus(editorEngine, 5, 2, 15));

            node.AddRenderable(sphereMesh);

            Material material = new Material("earth");

            MaterialLayer layer = material.CreateLayer();

            layer.DiffuseMap = editorEngine.Textures.CreateTexture2DFromFile(new StringBuilder("earth"));

            sphereMesh.Material = material;
                
        }

        void AddSphere()
        {
            scene.Root.AddChildren(node = new Node(new StringBuilder("Sphere")));

            sphereMesh = new RenderableMesh(editorEngine, GeometryFactory.CreateSphere(editorEngine, 3, 5));

            node.AddRenderable(sphereMesh);

            Material material = new Material("earth");

            MaterialLayer layer = material.CreateLayer();

            layer.DiffuseMap = editorEngine.Textures.CreateTexture2DFromFile(new StringBuilder("earth"));

            sphereMesh.Material = material;

        }

        void AddOmniLight()
        {
            light = new OmniLight(15);

            light.Attenuation = Attenuation.None;

            scene.Root.AddChildren(light);

            light.Position = new AIOEngine.MathSpace.Vector3(0, 1, 6);

            light.Color = new AIOEngine.MathSpace.Vector4(1, 1, 1, 1);

            scene.Lights.Add(light);

        }

        void AddSpotLight()
        {
            spot = new SpotLight(500);

            spot.Attenuation = Attenuation.None;

            spot.FallOff = 50;
            spot.HotSpot = 20;

            scene.Root.AddChildren(spot);

            spot.Position = new AIOEngine.MathSpace.Vector3(0, 6, 0);

            spot.Color = new AIOEngine.MathSpace.Vector4(1, 1, 1, 1);

            //spot.Direction = new AIOEngine.MathSpace.Vector3(0, -1, 0);
            // As the default Direction is poinint towards -Z, we rotate to get Vector3(0, -1, 0)
            spot.AngleXInDegrees = -90;
            spot.Update();
            
            scene.Lights.Add(spot);
        }

        /// <summary>
        /// This method will be called when the control is unloaded
        /// </summary>
        private void OnDestroy()
        {
            if(sphereMesh!=null)
                sphereMesh.Mesh.Destroy();

            if(cubeMesh!=null)
                cubeMesh.Mesh.Destroy();

            editorEngine.RenderViews.DestroyAndRemove(renderView);

            editorEngine.RenderViews.DestroyAndRemove(backBufferView);
        }

        float x=0,x2;
        int att;

        private void OnUpdate()
        {

            node.AngleYInDegrees = x; x += 1.0f;
            //node.ScaleY = (float)(Math.Sin(x) * 5); x++;

            if (x == 360 && light!=null)
            {
                x = 0;

                att++;
                if (att == 4) att = 0;

                if (att == 0) light.Attenuation = Attenuation.None;
                if (att == 1) light.Attenuation = Attenuation.Inverse;
                if (att == 2) light.Attenuation = Attenuation.DualRadius;
                if (att == 3) light.Attenuation = Attenuation.InverseSquare;
            }
            
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

            // Fill it whith a green color and Clear Depth with 1 and stencil 0
            renderView.Clear(0, 1, 0, 1);

            // Draw the scene
            scene.Draw(camera);
            /*
            AIOEngine.MathSpace.Vector3[] lines=mesh.Mesh.BoundingBox.ToRenderLines();
            
            editorEngine.DrawLines(lines,new AIOEngine.MathSpace.Vector4(1,0,0,1),ref node.WorldMatrix,ref camera.View,ref camera.Projection);
         
            VerticesVector3Indices result=mesh.Mesh.BoundingSphere.ToRenderLines(15);

            editorEngine.DrawLines(result.Vertices,result.Indices, new AIOEngine.MathSpace.Vector4(1, 0, 0, 1), ref node.WorldMatrix, ref camera.View, ref camera.Projection);
             */
        }

        #region Internal methods for rendering

        private void RenderControl_Closed(object sender, EventArgs e)
        {
            editorEngine.Log.AddInformation(renderName,new StringBuilder("control closed"));

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
            onResizeMethods.Add((w,h)=>backBufferView.Resize(w,h));

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
            UpdateAndRender();
        }

        void UpdateAndRender()
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

            foreach(Action<int,int> action in onResizeMethods)
                action((int)e.NewSize.Width, (int)e.NewSize.Height);

            /* Set the backbuffer, which is a ID3D10Texture2D pointer */
            m_d3DImageEx.SetBackBufferEx(D3DResourceTypeEx.ID3D10Texture2D, backBufferRenderTarget.Texture.InternalTexture2D.GetTexturePtr());

        }

        private void UserControl_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            d3dScene_SizeChanged(sender, e);
        }

        #endregion

        /// <summary>
        /// To get key events, remember that you must call before to Keyboard.Focus(this). Usually you call to this method on MouseDown.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void UserControl_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Up) spot.AngleXInDegrees+=10;
            if (e.Key == Key.Down) spot.AngleYInDegrees+=10;

            spot.Update();
        }

        private void UserControl_FocusableChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            int a = 0;
            
        }

        private void d3dScene_MouseDown(object sender, MouseButtonEventArgs e)
        {
            // We need to set the focus to receive keyboard events

            Keyboard.Focus(this);
        }


    }
}
