using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SourceEngine.Physics
{
    public enum SolidType
    {
        None,
        BSP,
        BBox,
        OBB,
        OBBYaw,
        Custom,
        VPhysics,
        Last
    }

    public enum MoveType
    {
        None,
        Isometric,
        Walk,
        Step,
        Fly,
        FlyGravity,
        VPhysics,
        Push,
        NoClip,
        Ladder,
        Observer,
        Custom
    }
}
