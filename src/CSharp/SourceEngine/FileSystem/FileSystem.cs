using SourceEngine.Bridge.FileSystem;
using SourceEngine.Bridge.Utility;
using System;

namespace SourceEngine
{
    /// <summary>
    /// Interaction with the Source Engine File System.
    /// </summary>
    public static class FileSystem
    {
        /// <summary>
        /// Converts a relative path to a full path.
        /// </summary>
        public static string RelativePathToFullPath(string relativePath, PathID pathID)
        {
            IntPtr ptr = FileSystemImports.CS_RelativePathToFullPath(relativePath, Enum.GetName(typeof(PathID), pathID));
            return BridgeUtility.IntPtrToObject<string>(ptr);
        }
    }
}
