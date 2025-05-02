using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SourceEngine
{
    public static class Input
    {
        /// <summary>
        /// Returns True if the key is down this frame.
        /// </summary>
        public static bool KeyDown(Key key) => Bridge.Input.InputImports.CS_GetButtonDown(key);
    }

    public enum Key
    {
        Invalid = -1,
        None = 0,

        First = 0,
        NoneKey = First,
        Zero,
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        Pad0,
        Pad1,
        Pad2,
        Pad3,
        Pad4,
        Pad5,
        Pad6,
        Pad7,
        Pad8,
        Pad9,
        PadDivide,
        PadMultiply,
        PadMinus,
        PadPlus,
        PadEnter,
        PadDecimal,
        LBracket,
        RBracket,
        Semicolon,
        Apostrophe,
        Backquote,
        Comma,
        Period,
        Slash,
        Backslash,
        Minus,
        Equal,
        Enter,
        Space,
        Backspace,
        Tab,
        CapsLock,
        NumLock,
        Escape,
        ScrollLock,
        Insert,
        Delete,
        Home,
        End,
        PageUp,
        PageDown,
        Break,
        LShift,
        RShift,
        LAlt,
        RAlt,
        LControl,
        RControl,
        LWin,
        RWin,
        App,
        Up,
        Left,
        Down,
        Right,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        CapsLockToggle,
        NumLockToggle,
        ScrollLockToggle,

        Last = ScrollLockToggle,
        Count = (Last - First + 1),

        // Mouse
        MouseFirst = Last + 1,

        MouseLeft = MouseFirst,
        MouseRight,
        MouseMiddle,
        Mouse4,
        Mouse5,
        MouseWheelUp,    // A fake button which is 'pressed' and 'released' when the wheel is moved up
        MouseWheelDown,  // A fake button which is 'pressed' and 'released' when the wheel is moved down

        MouseLast = MouseWheelDown,
        MouseCount = (MouseLast - MouseFirst + 1),

        ButtonCodeLast,
        ButtonCodeCount = (ButtonCodeLast - First + 1),
    }

}
