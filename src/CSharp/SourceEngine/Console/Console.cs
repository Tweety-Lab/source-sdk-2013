namespace SourceEngine
{
    /// <summary>
    /// Interaction with the Developer Console.
    /// </summary>
    public static class Console
    {
        /// <summary>
        /// Send a message to the console.
        /// </summary>
        public static void Msg(string message)
        {
            Bridge.Console.ConsoleImports.CS_Msg(message);
        }

        /// <summary>
        /// Send a warning message to the console.
        /// </summary>
        public static void Warning(string message)
        {
            Bridge.Console.ConsoleImports.CS_Warning(message);
        }

        /// <summary>
        /// Execute a console command.
        /// </summary>
        public static void ExecuteCommand(string command)
        {
            Bridge.Console.ConsoleImports.CS_ExecuteCommand(command);
        }

        /// <summary>
        /// Set a console variable to a value.
        /// </summary>
        public static void SetConvar(string name, string value)
        {
            Bridge.Console.ConsoleImports.CS_SetConvar(name, value);
        }

        /// <summary>
        /// Register a console variable.
        /// </summary>
        public static void RegisterConVar(string name, string value, ConVarFlag flags, string description)
        {
            Bridge.Console.ConsoleImports.CS_RegisterConVar(name, value, (int)flags, description);
        }
    }
}