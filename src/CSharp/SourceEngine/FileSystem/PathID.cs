namespace SourceEngine
{
    public enum PathID
    {
        /// <summary>
        /// The first SearchPath only.
        /// </summary>
        Mod,

        /// <summary>
        /// All SearchPaths, including those inside GCFs.
        /// </summary>
        Game,

        /// <summary>
        /// Xbox 360 version of Game.
        /// </summary>
        XGame,

        /// <summary>
        /// The Game binaries folder (client, server).
        /// </summary>
        GameBin,

        /// <summary>
        /// The Engine binaries folder.
        /// </summary>
        ExecutablePath,

        /// <summary>
        /// Wherever the Game is currently configured to write out to. 
        /// Defaults to the first SearchPath.
        /// </summary>
        DefaultWritePath
    }

}
