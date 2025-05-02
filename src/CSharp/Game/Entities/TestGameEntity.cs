using SourceEngine;
using SourceEngine.Bridge.Events;
using SourceEngine.Entities;
using SourceEngine.Physics;

namespace Game.Entities;

[Entity("csharp_entity")]
public class TestGameEntity : BaseEntity
{
    [ConVar("csharp_convar", ConVarFlag.None, "C# ConVar registered from Game")]
    public static string TestConvar { get; set; } = "Test Convar";

    public override void Spawn()
    {
        Console.Msg("C# Entity Spawned.");
        SetModel("models/props_c17/oildrum001.mdl"); // Set model

        // Setup physics parameters
        SetSolidType(SolidType.VPhysics);
        SetMoveType(MoveType.VPhysics);

        // Initialize physics
        InitPhysics();

        // Run an event when the player dies
        GameEvents.OnPlayerDeath += () =>
        {
            Console.Msg("####################################");
            Console.Msg("C# GAME EVENT REGISTERED!");
            Console.Msg("Player Died!");
            Console.Msg("####################################");
        };
    }

    public override void Think()
    {
        // Hover Entity upwards whilst holding X
        if (Input.KeyDown(Key.X))
        {
            Console.Msg("Player holding X");

            Velocity = new Vector3(0, 0, 100); // Set upward velocity
        }

        // Hover Entity downwards whilst holding C
        if (Input.KeyDown(Key.C))
        {
            Console.Msg("Player holding C");

            Velocity = new Vector3(0, 0, -100); // Set downward velocity
        }
    }
}
