using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Game.Entities;
using SourceEngine;
using SourceEngine.Entities;
using SourceEngine.Modding;

namespace Game
{
    // Game Entry Point
    public class Game : IMod
    {
        public void OnLoad()
        {
            SourceEngine.Console.Msg("Game Loaded.");
        }
    }
}
