using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AIOEngine.Graphics
{
    /// <summary>
    /// We can have a mesh that has only one vertexbuffer but more than one material assigned. For example a cube, where each face has is own texture
    /// So me must define the information to the renderer, where to start each part
    /// </summary>
    public struct RenderableMeshPart
    {
        public uint IndexCount;
        public uint StartIndexLocation;
        public uint BaseVertexLocation;

        public RenderableMeshPart(uint indexCount,uint startIndexLocation,uint baseVertexLocation)
        {
            IndexCount = indexCount;

            StartIndexLocation = startIndexLocation;

            BaseVertexLocation = baseVertexLocation;
        }

    }
}
