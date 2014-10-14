using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AIOEngine.Graphics
{
    public class Scene
    {

        public RootNode Root;
        
        /// <summary>
        /// Lights list
        /// </summary>
        public List<Light> Lights;

        /// <summary>
        /// Initialize the scene and creates the root default node(name="Root")
        /// </summary>
        public Scene()
        {
            Root = new RootNode();

            Lights = new List<Light>();
        }

        /// <summary>
        /// Updates all the nodes in the scene
        /// </summary>
        public void Update()
        {
            Root.Update();
        }

        /// <summary>
        /// Draw all the nodes in the scene,using the <see cref="Camera"/> indicated as parameter
        /// </summary>
        public void Draw(Camera camera)
        {
            if (camera == null)
                return;

            Root.Draw(this, camera);
        }
    }
}
