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
        Server?.Start(port, maxClients);
        Client?.Connect("127.0.0.1:" + port);
    }

    public void Join(String hostAddress, ushort port)
    {
        Client?.Connect(hostAddress + ":" + port);
    }

    public void Run()
    {
        if (Server != null) Server.Update();
        if (Client != null) Client.Update();
    }
}