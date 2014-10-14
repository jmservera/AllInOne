using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AIOEngine.Graphics
{
    /// <summary>
    /// Represents an element of the graph with Scale, Rotation and Translation
    /// </summary>
    public class RootNode:Node
    {
        #region Constructor

        public RootNode(StringBuilder name)
            : base(name)
        {
        }

        /// <summary>
        /// Creates the root node setting Name="Root"
        /// </summary>
        public RootNode()
            : base()
        {
            this.Name = new StringBuilder("Root");
        }

        #endregion
    }
}
