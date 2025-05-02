using System;
using System.Runtime.InteropServices;

namespace SourceEngine.Bridge.FileSystem
{
    // Exports from Bridge/FileSystem/FileSystemExports.cpp
    internal static class FileSystemImports
    {
        [DllImport("Client", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CS_RelativePathToFullPath(string relativePath, string pathID);
    }

}