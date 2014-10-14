using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;

using GraphicsEngineSpace;

namespace AIOEngine.Graphics
{
    public class VisualEngine
    {
        StringBuilder LogTag;

        /// <summary>The base path used for resources. This string finalices with a \</summary>
        static string basePath;

        /// <summary>The internal directory name used for internal resources</summary>
        const string InternalDirectoryName = "_Internal_";

        /// <summary>The fullpath internal directory name used for internal resources. This string finalices with a \</summary>
        public static string InternalPath { get; private set; }

        /// <summary>The base path used for resources. This string finalices with a \</summary>
        public string BasePath { get { return basePath; } }

        /// <summary>The Physical graphics device</summary>
        public GraphicsDevice Device { get; private set; }

        /// <summary>List of all the <see cref="Texture"/>  used for this instance</summary>
        public Textures Textures { get; private set; }

        /// <summary>List of all the <see cref="RenderTarget"/> used for this instance</summary>
        public RenderTargets RenderTargets { get; private set; }

        /// <summary>List of all the <see cref="DepthStencil"/> used for this instance</summary>
        public DepthStencils DepthStencils { get; private set; }

        /// <summary>List of all the <see cref="RenderView"/> used for this instance</summary>
        public RenderViews RenderViews { get; private set; }

        /// <summary>Log linked to this instance</summary>
        public Log Log { get; private set; }

        /// <summary>List of all the <see cref="Sprite"/></summary>
        public Sprites Sprites { get; private set; }

        #region Static Methods

        /// <summary>
        /// When working with VStudio, the currentdirectory is in the Debug folder. 
        /// To standarize, we go up until we find the '_Internal_' dir wich has the needed stuff for the engine
        /// </summary>
        public static void FindInternalDirectory()
        {
            string currentDir = Directory.GetCurrentDirectory();

            while (!Directory.Exists(InternalDirectoryName))
            {
                // Go up!!

                Directory.SetCurrentDirectory("..\\");
            }
            
            // Get the base path
            basePath = Directory.GetCurrentDirectory() + @"\";

            InternalPath = Directory.GetCurrentDirectory() + @"\" + InternalDirectoryName + @"\";

            // Restore back the initial directory

            Directory.SetCurrentDirectory(currentDir);
        }

        #endregion

        /// <summary>
        /// Main constructor. Each instance of this class is a potential 'device', with its own screen size and events
        /// </summary>
        /// <param name="tag">The tag that will be asociated to the log messages of this instance</param>
        public VisualEngine(StringBuilder tag)
        {
            LogTag = tag;

            Log = new Log();

            // Assign InternalPath
            if(InternalPath==null)
                FindInternalDirectory();

            Log.SetFileOutput(InternalPath + "Log\\Log_" + LogTag + ".txt");

            Log.AddInformation(LogTag,new StringBuilder("Creating the device"));

            Device = new GraphicsDevice();

            Device.Initialize(tag.ToString(),InternalPath);

            Log.AddInformation(LogTag, new StringBuilder("Creating the Textures"));

            Textures = new Textures(this);

            RenderTargets = new RenderTargets(this);

            DepthStencils = new DepthStencils(this);

            RenderViews = new RenderViews(this);

            Sprites = new Sprites(this);

        }

        /// <summary>
        /// Creates a <see cref="RenderTarget"/> compatible with DX9 and WPF
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        public RenderTarget CreateDX9SharedRenderTarget(string name, int width, int height)
        {
            _RenderTarget renderTarget=Device.CreateDX9SharedRenderTarget(name, width, height);

            return RenderTargets.Create(new StringBuilder(name), renderTarget);
        }

        /// <summary>
        /// Creates a <see cref="RenderTarget"/> compatible with DX9 and WPF
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        public RenderTarget CreateSwapChainRenderTarget(string name,IntPtr hwnd)
        {
            _RenderTarget renderTarget = Device.CreateSwapChainRenderTarget(name,(int)hwnd);

            return RenderTargets.Create(new StringBuilder("DXBackBuffer"), renderTarget);
        }

        public void DrawLines(AIOEngine.MathSpace.Vector3[] lines, AIOEngine.MathSpace.Vector4 color, ref AIOEngine.MathSpace.Matrix world, ref AIOEngine.MathSpace.Matrix view, ref AIOEngine.MathSpace.Matrix projection)
        {
            Device.DrawLines(lines,color,ref world,ref view, ref projection);
        }

        public void DrawLines(AIOEngine.MathSpace.Vector3[] lines,uint[] indices, AIOEngine.MathSpace.Vector4 color, ref AIOEngine.MathSpace.Matrix world, ref AIOEngine.MathSpace.Matrix view, ref AIOEngine.MathSpace.Matrix projection)
        {
            Device.DrawLines(lines,indices, color, ref world, ref view, ref projection);
        }
        /// <summary>
        /// Destroy and free all the resources
        /// </summary>
        public void Destroy()
        {
            DepthStencils.Destroy();

            RenderTargets.Destroy();

            Sprites.Destroy();

            Textures.Destroy();

            Device.Destroy();

            Log.Destroy();

        }

    }
}
