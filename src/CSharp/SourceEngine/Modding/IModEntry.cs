namespace SourceEngine.Modding
{
    /// <summary>
    /// Entry point for a C# mod.
    /// </summary>
    public interface IMod
    {
        /// <summary>
        /// Called when the mod is first loaded.
        /// </summary>
        void OnLoad();
    }

}
