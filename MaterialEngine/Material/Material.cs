using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AIOEngine.Graphics
{
    public class Material
    {
        public string Name { get; private set; }

        public List<MaterialLayer> Layers { get; private set; }

        public Material(string name)
        {
            Name = name;

            Layers = new List<MaterialLayer>();
        }

        public MaterialLayer CreateLayer()
        {
            MaterialLayer layer = new MaterialLayer();

            Layers.Add(layer);

            return layer;
        }
    }
}
