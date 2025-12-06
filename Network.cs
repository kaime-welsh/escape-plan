namespace EscapePlan;

using Riptide;
using Riptide.Utils;

enum ServerMessages { }

enum ClientMessages { }

class Network
{
    public Server Server { get; set; }
    public Client Client { get; set; }

    public Network()
    {
        RiptideLogger.Initialize(Console.WriteLine, true);

        Console.WriteLine("Creating server...");
        Server = new Server();
        Console.WriteLine("Server created.");

        Console.WriteLine("Creating local client...");
        Client = new Client();
        Console.WriteLine("Client created.");
    }

    public void Host(ushort port = 5000, ushort maxClients = 1)
    {
        if (Server != null) return;
        Server.Start(port, maxClients);
        Join("127.0.0.1", port);

        Server.ClientConnected += ServerHandleClientJoin;
    }

    public void Join(String hostAddress = "127.0.0.1", ushort port = 5000)
    {
        if (Client != null) return;
        Client.Connect(hostAddress + ":" + port);
        Client.ClientConnected += ClientHandleClientJoin;
    }

    private void ServerHandleClientJoin(object? sender, ServerConnectedEventArgs e)
    {
        Console.WriteLine("[SERVER] Client connected.");
    }

    private void ClientHandleClientJoin(object? sender, ClientConnectedEventArgs e)
    {
        Console.WriteLine("[CLIENT] Client connected.");
    }

    public void Run()
    {
        if (Server != null) Server.Update();
        if (Client != null) Client.Update();
    }
}