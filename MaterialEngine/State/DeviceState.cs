using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AIOEngine.Graphics
{
    public enum CULLMODE_STATE
    {	NONE	= 1,
	    FRONT	= 2,
	    BACK	= 3
    };

    public class DeviceState
    {
        /// <summary>
        /// Gets/Sets the depth buffer. Defaults true
        /// </summary>
        public bool DepthEnabled { get; set; }

        /// <summary>
        /// Gets/Sets the Fill mode. If disabled, the render will only render the triangle outline. Defaults true
        /// </summary>
        public bool FillModeEnabled { get; set; }

        /// <summary>
        /// Gets/Sets the Cull Mode(Indicates triangles facing a particular direction are not drawn). Defaults BACK
        /// </summary>
        public CULLMODE_STATE CullMode { get; set; }

        public DeviceState()
        {

            DepthEnabled = true;

            FillModeEnabled = true;

            CullMode = CULLMODE_STATE.BACK;
        }

    }
}
