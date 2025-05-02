using System;

namespace SourceEngine
{
    // TODO: Verify these actually line up with the source flags

    [Flags]
    public enum ConVarFlag
    {
        None = 0,
        Unregistered = (1 << 0),
        DevelopmentOnly = (1 << 1),
        GameDLL = (1 << 2),
        ClientDLL = (1 << 3),
        Hidden = (1 << 4),
        Protected = (1 << 5),
        SPOnly = (1 << 6),
        Archive = (1 << 7),
        Notify = (1 << 8),
        UserInfo = (1 << 9),
        Cheat = (1 << 14),
        PrintableOnly = (1 << 10),
        Unlogged = (1 << 11),
        NeverAsString = (1 << 12),
        Replicated = (1 << 13),
        Demo = (1 << 16),
        DontRecord = (1 << 17),
        ReloadMaterials = (1 << 20),
        ReloadTextures = (1 << 21),
        NotConnected = (1 << 22),
        MaterialSystemThread = (1 << 23),
        ArchiveXbox = (1 << 24),
        AccessibleFromThreads = (1 << 25),
        ServerCanExecute = (1 << 28),
        ServerCannotQuery = (1 << 29),
        ClientCmdCanExecute = (1 << 30),
        ExecDespiteDefault = (1 << 31),
        InternalUse = (1 << 15),
        AllowedInCompetitive = (1 << 18)
    }

}
