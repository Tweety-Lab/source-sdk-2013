using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace SourceEngine.Bridge.Input
{
    internal static class InputImports
    {
        [DllImport("Client", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool CS_GetButtonDown(Key keyCode);
    }
}
