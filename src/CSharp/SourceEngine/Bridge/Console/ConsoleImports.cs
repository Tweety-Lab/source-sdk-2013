using System.Runtime.InteropServices;

namespace SourceEngine.Bridge.Console
{
    // Exports from Bridge/Console/ConsoleExports.cpp
    internal static class ConsoleImports
    {
        [DllImport("Client", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_Msg(string message);

        [DllImport("Client", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_Warning(string message);

        [DllImport("Client", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_ExecuteCommand(string command);

        [DllImport("Client", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_SetConvar(string name, string value);

        [DllImport("Client", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CS_RegisterConVar(string name, string value, int flags, string description);
    }

}
