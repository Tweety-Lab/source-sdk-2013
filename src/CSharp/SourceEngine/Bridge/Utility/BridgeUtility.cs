using System;
using System.Runtime.InteropServices;

namespace SourceEngine.Bridge.Utility
{
    /// <summary>
    /// Utility class for C++ / C# bridging.
    /// </summary>
    internal static class BridgeUtility
    {
        /// <summary>
        /// Convert IntPtr to an object.
        /// </summary>
        public static T IntPtrToObject<T>(IntPtr ptr)
        {
            if (ptr == IntPtr.Zero)
                return default;

            // String Conversion
            if (typeof(T) == typeof(string))
            {
                // Default to ANSI string
                object result = Marshal.PtrToStringAnsi(ptr);
                return (T)result;
            }

            // Default to Marshal.PtrToStructure
            return Marshal.PtrToStructure<T>(ptr);
        }
    }
}

