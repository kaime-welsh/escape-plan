using System.Numerics;
using Raylib_cs;
using static Raylib_cs.Raylib;

namespace EscapePlan;

class Player
{
    Vector3 Position { get; set; }
    uint Owner { get; set; }
    
    void Update(float dt)
    {
        
    }
    
    void Draw()
    {
        DrawSphere(Position, 1.0f, Color.Green);
    }
}