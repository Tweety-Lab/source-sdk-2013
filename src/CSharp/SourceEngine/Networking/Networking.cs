using SourceEngine.Bridge.Networking;

namespace SourceEngine
{
    public static class Networking
    {
        /// <summary>
        /// Returns true if the code is running on the client.
        /// </summary>
        public static bool IsClient => NetworkingImports.CS_IsRunningOnClient();

        /// <summary>
        /// Returns true if the code is running on the server.
        /// </summary>
        public static bool IsServer => !NetworkingImports.CS_IsRunningOnClient();
    }

}
