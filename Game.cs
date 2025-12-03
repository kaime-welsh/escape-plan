using Raylib_cs;
using static Raylib_cs.Raylib;
using rlImGui_cs;
using ImGuiNET;
using Riptide;
using Riptide.Utils;

namespace EscapePlan;

class Game
{
    public static Server? Server { get; set; }
    public static Client? Client { get; set; }

    public Game()
    {
        SetConfigFlags(ConfigFlags.MaximizedWindow | ConfigFlags.ResizableWindow | ConfigFlags.VSyncHint);
        InitWindow(800, 600, "Escape Plan");
        SetTargetFPS(500);
        rlImGui.Setup(true);
        ImGui.GetIO().ConfigFlags |= ImGuiConfigFlags.DockingEnable;

        RiptideLogger.Initialize(Console.WriteLine, true);

        Console.WriteLine("Creating server...");
        Server = new Server();

        Console.WriteLine("Creating local client...");
        Client = new Client();
    }

    public void Run()
    {
        while (!WindowShouldClose())
        {
            if (Server != null) Server.Update();
            if (Client != null) Client.Update();

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