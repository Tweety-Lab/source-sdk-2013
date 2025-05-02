using System.Runtime.InteropServices;

namespace SourceEngine.Bridge.Networking
{
    // Exports from Bridge/Networking/NetworkingExports.cpp
    internal static class NetworkingImports
    {
        [DllImport("Client", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool CS_IsRunningOnClient();
    }

}
