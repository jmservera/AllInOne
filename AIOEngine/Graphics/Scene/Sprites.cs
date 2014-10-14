using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

using GraphicsEngineSpace;

namespace AIOEngine.Graphics
{
    public class Sprites
    {
        public List<Sprite> Items;

        /// <summary>The Engine instance asociated to this texture</summary>
        public VisualEngine Engine { get; private set; }

        public Sprites(VisualEngine engine)
        {
            Engine = engine;

            Items = new List<Sprite>();

        }

        /// <summary>
        /// Destroy and free all the sprites
        /// </summary>
        public void Destroy()
        {
            foreach (Sprite sprite in Items)
            {
                sprite.Destroy();
            }

            Items=null;
        }

        /// <summary>
        /// Destroy and removes the sprite from the list
        /// </summary>
        /// <param name="sprite"></param>
        public void Remove(Sprite sprite)
        {
            sprite.Destroy();

            Items.Remove(sprite);
        }

        /// <summary>
        /// Creates a new Sprite
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <returns></returns>
        public Sprite Create(int width, int height)
        {
            Sprite sprite = new Sprite(Engine,width, height);

            Items.Add(sprite);

            return sprite;
        }

    }
}
