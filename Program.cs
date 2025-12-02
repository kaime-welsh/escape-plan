using Raylib_cs;
using Riptide;
using Riptide.Utils;
using static Raylib_cs.Raylib;

namespace EscapePlan;

class Program
{
    public static void Main(string[] args)
    {
        Server? server = null;
        Client? client = null;

        SetConfigFlags(ConfigFlags.MaximizedWindow | ConfigFlags.ResizableWindow | ConfigFlags.VSyncHint);
        InitWindow(800, 600, "Escape Plan");
        SetTargetFPS(500);

        RiptideLogger.Initialize(Console.WriteLine, true);

        if (args[0] == "server")
        {
            server = new Server();
            server.Start(7777, 10);
        }

        else if (args[0] == "client")
        {
            client = new Client();
            client.Connect("127.0.0.1:7777");
        }

        while (!WindowShouldClose())
        {
            if (server != null) server.Update();
            if (client != null) client.Update();

            BeginDrawing();
            ClearBackground(Color.Black);
            DrawText("Hello!", 32, 32, 32, Color.White);
            EndDrawing();
        }
        CloseWindow();
    }
}