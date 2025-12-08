#include "enet/enet.h"
#include "stdio.h"

static ENetHost *server_host = NULL;
static ENetPeer *client_peer = NULL;

void network_initialize()
{
    if (enet_initialize() != 0)
    {
        printf("An error occurred while initializing ENet.\n");
    }
}

void network_host()
{
    ENetAddress address;
    ENetHost *server;

    address.host = ENET_HOST_ANY;
    address.port = 5000;

    server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == NULL)
    {
        printf("An error occurred while trying to create an ENet server host.\n");
    }
    server_host = server;
    printf("[SERVER] Hosting on port %u.\n", address.port);
}

void network_connect(const char *hostAddress)
{
    ENetHost *client;
    ENetAddress address;
    ENetPeer *peer;

    client = enet_host_create(NULL, 1, 2, 0, 0);
    if (client == NULL)
    {
        printf("An error occurred while trying to create an ENet client host.\n");
        return;
    }

    enet_address_set_host(&address, hostAddress);
    address.port = 5000;

    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == NULL)
    {
        printf("No available peers for initiating an ENet connection.\n");
        enet_host_destroy(client);
        return;
    }
    client_peer = peer;
    printf("[CLIENT] Connection to %s initiated.\n", hostAddress);
}

void network_update()
{
    if (server_host != nullptr)
    {
        ENetEvent event;
        if (enet_host_service(server_host, &event, 0) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                char host_name[256];
                enet_address_get_host(&event.peer->address, host_name, sizeof(host_name));
                printf("[SERVER] A new client connected from %s:%u.\n",
                       host_name,
                       event.peer->address.port);
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                printf("[SERVER] A packet of length %zu was received from %x:%u on channel %u.\n",
                       event.packet->dataLength,
                       event.peer->address.host,
                       event.peer->address.port,
                       event.channelID);
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                printf("[SERVER] Client disconnected.\n");
                break;
            default:
                break;
            }
        }
    }

    if (client_peer != nullptr)
    {
        ENetEvent event;
        if (enet_host_service(client_peer->host, &event, 0) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                printf("[CLIENT] Successfully connected to server.\n");
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                printf("[CLIENT] A packet of length %zu was received from server on channel %u.\n",
                       event.packet->dataLength,
                       event.channelID);
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                printf("[CLIENT] Disconnected from server.\n");
                break;
            default:
                break;
            }
        }
    }
}

void network_deinitialize()
{
    enet_deinitialize();
}