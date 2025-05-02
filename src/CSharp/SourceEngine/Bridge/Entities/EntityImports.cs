using SourceEngine.Physics;
using System;
using System.Runtime.InteropServices;

namespace SourceEngine.Bridge.Entities
{
    // Exports from Bridge/Entities/EntityExports.cpp
    internal static class EntityImports
    {
        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_RegisterCSharpEntity(string entityName, IntPtr entityType);


        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_SetEntityModel(uint entityHandle, string modelName);


        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern string CS_GetEntityModel(uint entityHandle);


        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_SetEntityAbsOrigin(uint entityHandle, ref Vector3 position);


        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_GetEntityAbsOrigin(uint entityHandle, out Vector3 position);


        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_SetEntityLocalOrigin(uint entityHandle, ref Vector3 position);


        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_GetEntityLocalOrigin(uint entityHandle, out Vector3 position);


        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_SetEntityThinkInterval(uint entityHandle, float thinkInterval);

        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_GetEntityThinkInterval(uint entityHandle, out float thinkInterval);

        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_SetEntitySolidType(uint entityHandle, SolidType solidType);

        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_SetEntityMoveType(uint entityHandle, MoveType solidType);

        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_InitEntityPhysics(uint entityHandle);

        [DllImport("Server", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_SetEntityAbsVelocity(uint entityHandle, Vector3 velocity);

    }

}
