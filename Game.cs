using Raylib_cs;
using static Raylib_cs.Raylib;
using rlImGui_cs;
using ImGuiNET;

namespace EscapePlan;

class Game
{
    Network Network;

    public Game()
    {
        SetConfigFlags(ConfigFlags.MaximizedWindow | ConfigFlags.ResizableWindow | ConfigFlags.VSyncHint);
        InitWindow(800, 600, "Escape Plan");
        SetTargetFPS(500);
        rlImGui.Setup(true);
        ImGui.GetIO().ConfigFlags |= ImGuiConfigFlags.DockingEnable;

        Network = new Network();
        Network.Host();
    }

    public void Run()
    {
        while (!WindowShouldClose())
        {
            Network.Run();

            BeginDrawing();
            ClearBackground(Color.Black);
            rlImGui.Begin();
            rlImGui.End();
            EndDrawing();
        }
        rlImGui.Shutdown();
        CloseWindow();
    }
}