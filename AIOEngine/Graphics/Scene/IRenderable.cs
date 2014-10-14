using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AIOEngine.Graphics
{
    public interface IRenderable
    {
        bool DrawBounding { get; set; }
        bool Visible { get; set; }
        void Update(Node node);
        void Draw(Scene scene,Node node, Camera camera);
    }
}
