using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

using GraphicsEngineSpace;

namespace AIOEngine.Graphics
{
    public class Textures
    {
        readonly static StringBuilder TAG = new StringBuilder("Textures");

        public List<Texture> Items;

        /// <summary>The Engine instance asociated to this texture</summary>
        public VisualEngine Engine { get; private set; }

        /// <summary>All paths where the texture load will search</summary>
        public List<string> Paths;

        /// <summary>List of extensions that the loader will search for a texture</summary>
        public static string[] Extensions= { ".dds",".png",".jpg",".bmp" };

        public Textures(VisualEngine engine)
        {
            Engine = engine;

            Items = new List<Texture>();

            Paths = new List<string>();

            // Add the internal path for textures used by the engine
            Paths.Add(VisualEngine.InternalPath+"Graphics\\");
        }

        /// <summary>
        /// Destroy and free all textures
        /// </summary>
        public void Destroy()
        {
            foreach (Texture texture in Items)
            {
                texture.Destroy();
            }

            Items=null;
        }

        /// <summary>
        /// Adds a path relative to <see cref="VisualEngine.BasePath"/>
        /// </summary>
        /// <param name="path"></param>
        public void AddRelativePath(string path)
        {
            // Adds a bar at the end of the path
            if (path[path.Length - 1] != '\\') path += "\\";

            Paths.Add(Engine.BasePath+path);
        }

        /// <summary>
        /// Adds a full path. This method should not be used, because on mobile, there is no acces to full paths
        /// </summary>
        /// <param name="path"></param>
        public void AddFullPath(string path)
        {
            Engine.Log.AddWarning(TAG, new StringBuilder("Adding a FullPath will not work on mobile!!!"));

            // Adds a bar at the end of the path
            if (path[path.Length - 1] != '\\') path += "\\";

            Paths.Add(path);
        }

        /// <summary>
        /// Checks all the paths and extension til the file is found
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        StringBuilder FindFileWithExtension(StringBuilder name)
        {
            // Check if this file exist. This is not the correct way of doing this, because that means that the name is the fullPath!!
            if (File.Exists(name.ToString())) return name;

            foreach (string path in Paths)
            {
                foreach(string extension in Extensions)
                {
                    string fullPath=path+name+extension;

                    if (File.Exists(fullPath)) return new StringBuilder(fullPath);
                }
            }

            return null;
        }

        /// <summary>
        /// If the texture does not exists, creates and load it. If exists, return the instance
        /// </summary>
        /// <param name="name">The full path WITHOUTH the extension. The engine will try the compatible extensions</param>
        /// <returns></returns>
        public Texture2D CreateTexture2DFromFile(StringBuilder name)
        {
            Texture2D texture = (Texture2D)CheckIfExists(name);

            if (texture == null)
            {
                StringBuilder fullPath = FindFileWithExtension(name);

                texture = new Texture2D(Engine,name,fullPath);

                Items.Add(texture);
            }

            return texture;
        }

        /// <summary>
        /// Checks if this textures already exists with this name
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        Texture CheckIfExists(StringBuilder name)
        {
            foreach (Texture texture in Items)
            {
                if (name == texture.Name) return texture;
            }

            return null;
        }
    }
}
