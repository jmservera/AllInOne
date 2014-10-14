using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using AIOEngine.MathSpace;

#region NUMERICAL DEFINITION FOR FLOAT OR DOUBLE

using Numeric = System.Single;

#endregion

namespace AIOEngine.Graphics
{
    /// <summary>
    /// Represents an element of the graph with Scale, Rotation and Translation
    /// </summary>
    public class NodeBase
    {
        #region Fields

        bool worldMatrixIsIdentity;

        /// <summary>matrix computed from AngleX,Y,Z</summary>
        Matrix rotationMatrix;

        /// <summary>Flag to informs the Update method if need to create the <see cref="rotationMatrix"/></summary>
        bool updateRotation;

        /// <summary>Flag to informs the Update method that the <see cref="rotationMatrix"/> is identity</summary>
        bool rotationIsIdentity;

        /// <summary>Rotation on X, in radians</summary>
        Numeric angleX;
        /// <summary>Rotation on Y, in radians</summary>
        Numeric angleY;
        /// <summary>Rotation on Z, in radians</summary>
        Numeric angleZ;

        /// <summary>matrix computed from ScaleX,Y,Z</summary>
        Matrix scaleMatrix;

        /// <summary>Flag to informs the Update method if need to create the <see cref="scaleMatrix"/></summary>
        bool updateScale;

        /// <summary>Flag to informs the Update method that the <see cref="scaleMatrix"/> is identity</summary>
        bool scaleIsIdentity;

        /// <summary>Scale on X</summary>
        Numeric scaleX;
        /// <summary>Scale on Y</summary>
        Numeric scaleY;
        /// <summary>Scale on Z</summary>
        Numeric scaleZ;

        /// <summary>matrix computed from PositionX,Y,Z</summary>
        Matrix positionMatrix;

        /// <summary>Flag to informs the Update method if need to create the <see cref="rotationMatrix"/></summary>
        bool updateTranslation;

        /// <summary>Flag to informs the Update method that the <see cref="positionMatrix"/> is identity</summary>
        bool positionIsIdentity;

        /// <summary>Position in local space</summary>
        Vector3 position;

        bool matrixChanged;

        #endregion

        #region Public fields

        /// <summary>Local Matrix=Scale*Rotation*Translation</summary>
        public Matrix LocalMatrix;

        /// <summary>World Matrix=LocalMatrix*Parent.WorldMatrix</summary>
        public Matrix WorldMatrix;

        /// <summary>Parent Node</summary>
        public NodeBase Parent { get; set; }

        /// <summary>Node name</summary>
        public StringBuilder Name { get; set; }

        public List<NodeBase> Children;

        #endregion

        #region Properties

        /// <summary>Returns the position in world space. This value is matrix based, so be sure to call update before!!</summary>
        public Vector3 PositionWorldSpace { get { return LocalMatrix.Translation; } }

        /// <summary>Gets/Sets scale on X</summary>
        public Numeric ScaleX { get { return scaleX; } set { updateScale = (scaleX != value); scaleX = value; } }
        /// <summary>Gets/Sets scale on Y</summary>
        public Numeric ScaleY { get { return scaleY; } set { updateScale = (scaleY != value); scaleY = value; } }
        /// <summary>Gets/Sets scale on Z</summary>
        public Numeric ScaleZ { get { return scaleZ; } set { updateScale = (scaleZ != value); scaleZ = value; } }

        /// <summary>Gets/Sets position in local space</summary>
        public Vector3 Position { get { return position; } set { updateTranslation = (position != value); position = value; } }

        /// <summary>Gets/Sets position on X</summary>
        public Numeric X { get { return position.X; } set { updateTranslation = (position.X != value); position.X = value; } }
        /// <summary>Gets/Sets position on Y</summary>
        public Numeric Y { get { return position.Y; } set { updateTranslation = (position.Y != value); position.Y = value; } }
        /// <summary>Gets/Sets position on Z</summary>
        public Numeric Z { get { return position.Z; } set { updateTranslation = (position.Z != value); position.Z = value; } }

        /// <summary>Gets/Sets angle on X in radians</summary>
        public Numeric AngleX { get { return angleX; } set { updateRotation = (angleX != value); angleX = value; } }
        /// <summary>Gets/Sets angle on Y in radians</summary>
        public Numeric AngleY { get { return angleY; } set { updateRotation = (angleY != value); angleY = value; } }
        /// <summary>Gets/Sets angle on Z in radians</summary>
        public Numeric AngleZ { get { return angleZ; } set { updateRotation = (angleZ != value); angleZ = value; } }

        /// <summary>Gets/Sets angle on X in degrees</summary>
        public Numeric AngleXInDegrees { get { return MathHelper.ToDegrees(angleX); } set { Numeric newRadians = MathHelper.ToRadians(value); updateRotation = (angleX != newRadians); angleX = newRadians; } }
        /// <summary>Gets/Sets angle on Y in degrees</summary>
        public Numeric AngleYInDegrees { get { return MathHelper.ToDegrees(angleY); } set { Numeric newRadians = MathHelper.ToRadians(value); updateRotation = (angleY != newRadians); angleY = newRadians; } }
        /// <summary>Gets/Sets angle on Z in degrees</summary>
        public Numeric AngleZInDegrees { get { return MathHelper.ToDegrees(angleZ); } set { Numeric newRadians = MathHelper.ToRadians(value); updateRotation = (angleZ != newRadians); angleZ = newRadians; } }

        #endregion

        #region Constructor

        public NodeBase()
        {
            Initialize();
        }

        public NodeBase(StringBuilder name)
        {
            Initialize();

            Name = name;
        }

        void Initialize()
        {
            matrixChanged = false;

            worldMatrixIsIdentity = true;

            LocalMatrix = Matrix.Identity;

            WorldMatrix = Matrix.Identity;

            // Scale

            scaleX = scaleY = scaleZ = 1;

            scaleMatrix = Matrix.Identity;

            scaleIsIdentity = true;

            updateScale = false;

            // Rotation

            rotationMatrix = Matrix.Identity;

            rotationIsIdentity = true;

            updateRotation = false;

            // Translation

            positionMatrix = Matrix.Identity;

            positionIsIdentity = true;

            updateTranslation = false;
        }

        #endregion

        #region Methods

        /// <summary>
        /// This method is virtual, but you do not need to call to base.Update 
        /// </summary>
        public virtual void OnUpdate()
        {

        }

       /// <summary>
        /// Updates changes and build the matrices. Then call the children to update
        /// </summary>
        public void Update()
        {
            UpdateMatrices();

            OnUpdate();

            UpdateChildren();
        }

         /// <summary>
        /// Update matrices if necessary
        /// </summary>
        void UpdateMatrices()
        {
            //Quaternion = Quaternion.RotationYawPitchRoll(AngleY, AngleX, AngleZ);
            bool update = false;

            if (updateRotation)
            {
                rotationIsIdentity = false;
                if (angleX == 0)
                    if (angleY == 0)
                        if (angleZ == 0)
                            rotationIsIdentity = true;

                rotationMatrix = Matrix.CreateFromYawPitchRoll(angleY, angleX, angleZ);

                updateRotation = false;

                update = true;
            }

            if (updateScale)
            {
                // This should work, but....it doesn't work!!!!
                //scaleIsIdentity = (scaleX == 1f && scaleY == 1f && scaleZ == 1f);

                scaleIsIdentity = false;
                if (scaleX == 1)
                    if (scaleY == 1)
                        if (scaleZ == 1)
                            scaleIsIdentity = true;

                scaleMatrix = Matrix.CreateScale(scaleX, scaleY, scaleZ);

                updateScale = false;

                update = true;

            }

            if (updateTranslation)
            {
                positionIsIdentity = position.IsEmpty();

                positionMatrix = Matrix.CreateTranslation(position);

                updateTranslation = false;

                update = true;

            }

            if (update)
            {
                if (scaleIsIdentity)
                {
                    if (rotationIsIdentity)
                        LocalMatrix = positionMatrix;
                    else
                        LocalMatrix = rotationMatrix * positionMatrix;
                }
                else
                {
                    if (rotationIsIdentity)
                        LocalMatrix = scaleMatrix * positionMatrix;
                    else
                        LocalMatrix = scaleMatrix * rotationMatrix * positionMatrix;
                }
            }
            else
            {
                // Check if parent matrix has changed
                if (Parent != null && Parent.matrixChanged) update = true;
            }

            if (update)
            {
                if (Parent != null && !Parent.worldMatrixIsIdentity)
                {
                    WorldMatrix = LocalMatrix * Parent.WorldMatrix;
                }
                else
                {
                    WorldMatrix = LocalMatrix;
                }

                worldMatrixIsIdentity = WorldMatrix.IsIdentity();

                // Force children to update matrices too
                matrixChanged = true;
            }
            else
            {
                matrixChanged = false;
            }

        }


        public void UpdateChildren()
        {
            if (Children == null) return;

            foreach (NodeBase node in Children)
            {
                node.Update();
            }
        }

        /// <summary>
        /// Adds the node to the children list
        /// </summary>
        /// <param name="node"></param>
        public void AddChildren(NodeBase node)
        {
            if(Children==null)
                Children = new List<NodeBase>();

            Children.Add(node);

            node.Parent = this;
        }
        
        #endregion
    }
}
