using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;

namespace SourceEngine.Bridge.NETHost
{
    /// <summary>
    /// Utilities for bridging with native .NET Host code.
    /// </summary>
    public static class HostBridge
    {
        [UnmanagedCallersOnly(EntryPoint = "InvokeDotNetMethod")]
        public static void InvokeDotNetMethod(IntPtr methodNamePtr, uint arg)
        {
            string methodName = Marshal.PtrToStringAnsi(methodNamePtr);

            try
            {
                // Expected format: Namespace.Type:Method
                var parts = methodName.Split(':');
                if (parts.Length != 2)
                {
                    Console.ConsoleImports.CS_Msg("Invalid method name format");
                    return;
                }

                string typeName = parts[0];
                string method = parts[1];

                Type type = Type.GetType(typeName);
                if (type == null)
                {
                    Console.ConsoleImports.CS_Msg($"Type not found: {typeName}");
                    return;
                }

                // Find method with matching signature
                MethodInfo methodInfo = type.GetMethod(method,
                    BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic,
                    null,
                    new Type[] { typeof(uint) },
                    null);

                if (methodInfo == null)
                {
                    // Invoke with no argument
                    methodInfo = type.GetMethod(method,
                        BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic,
                        null,
                        Type.EmptyTypes,
                        null);

                    if (methodInfo == null)
                    {
                        Console.ConsoleImports.CS_Msg($"Method not found: {method}");
                        return;
                    }
                    // Invoke with no argument
                    methodInfo.Invoke(null, Array.Empty<object>());
                    return;
                }

                // Invoke with the uint argument
                methodInfo.Invoke(null, new object[] { arg });
            }
            catch (Exception ex)
            {
                Console.ConsoleImports.CS_Msg($"Exception in InvokeDotNetMethod: {ex}");
            }
        }


        [UnmanagedCallersOnly(EntryPoint = "InvokeDotNetMethod_Str")]
        public static void InvokeDotNetMethod_Str(IntPtr methodNamePtr, IntPtr strArgPtr)
        {
            string methodName = Marshal.PtrToStringAnsi(methodNamePtr);
            string strArg = Marshal.PtrToStringAnsi(strArgPtr);

            try
            {
                var parts = methodName.Split(':');
                if (parts.Length != 2)
                {
                    Console.ConsoleImports.CS_Msg("Invalid method name format");
                    return;
                }

                string typeName = parts[0];
                string method = parts[1];

                Type type = Type.GetType(typeName);
                if (type == null)
                {
                    Console.ConsoleImports.CS_Msg($"Type not found: {typeName}");
                    return;
                }

                MethodInfo methodInfo = type.GetMethod(method,
                    BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic,
                    null,
                    new Type[] { typeof(string) },
                    null);

                if (methodInfo == null)
                {
                    Console.ConsoleImports.CS_Msg($"Method not found: {method}");
                    return;
                }

                methodInfo.Invoke(null, new object[] { strArg });
            }
            catch (Exception ex)
            {
                Console.ConsoleImports.CS_Msg($"Exception in InvokeDotNetMethod_Str: {ex}");
            }
        }

    }
}