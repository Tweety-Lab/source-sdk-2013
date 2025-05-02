using SourceEngine.Bridge.Entities;
using SourceEngine.Physics;
using System;
using System.Numerics;

namespace SourceEngine.Entities
{
    /// <summary>
    /// Base Class for all entities
    /// </summary>
    public class BaseEntity
    {
        // Handle reference to the native c++ entity
        protected uint NativeHandle;

        // Set the native handle, this gets called from engine
        internal void SetNativeHandle(uint handle)
        {
            NativeHandle = handle;
        }

        /// <summary>
        /// Set the model of the entity.
        /// </summary>
        public void SetModel(string modelName) => EntityImports.CS_SetEntityModel(NativeHandle, modelName);

        /// <summary>
        /// Set the solid type of the entity.
        /// </summary>
        public void SetSolidType(SolidType solidType) => EntityImports.CS_SetEntitySolidType(NativeHandle, solidType);

        /// <summary>
        /// Set the movetype of the entity.
        /// </summary>
        public void SetMoveType(MoveType moveType) => EntityImports.CS_SetEntityMoveType(NativeHandle, moveType);

        /// <summary>
        /// Initialize the physics of the entity.
        /// </summary>
        public void InitPhysics() => EntityImports.CS_InitEntityPhysics(NativeHandle);

        /// <summary>
        /// The Absolute Position of the entity.
        /// </summary>
        public Vector3 AbsolutePosition
        {
            get
            {
                EntityImports.CS_GetEntityAbsOrigin(NativeHandle, out Vector3 pos);
                return pos;
            }
            set
            {
                EntityImports.CS_SetEntityAbsOrigin(NativeHandle, ref value);
            }
        }

        /// <summary>
        /// The Local Position of the entity.
        /// </summary>
        public Vector3 LocalPosition
        {
            get
            {
                EntityImports.CS_GetEntityLocalOrigin(NativeHandle, out Vector3 pos);
                return pos;
            }
            set
            {
                EntityImports.CS_SetEntityLocalOrigin(NativeHandle, ref value);
            }
        }

        /// <summary>
        /// The Velocity of the entity.
        /// </summary>
        public Vector3 Velocity
        {
            set
            {
                EntityImports.CS_SetEntityAbsVelocity(NativeHandle, value);
            }
        }

        /// <summary>
        /// How often the Entities Think function is called.
        /// </summary>
        public float ThinkInterval
        {
            get
            {
                EntityImports.CS_GetEntityThinkInterval(NativeHandle, out float thinkInterval);
                return thinkInterval;
            }
            set
            {
                EntityImports.CS_SetEntityThinkInterval(NativeHandle, value);
            }
        }

        public virtual void Spawn() { }

        public virtual void Think() { }
    }

}