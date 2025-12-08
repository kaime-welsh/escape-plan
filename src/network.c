#include "network.h"
#include "enet/enet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ENetHost *server_host = NULL;
static ENetPeer *client_peer = NULL;

#define NET_QUEUE_CAPACITY 256
static NetMessage net_queue[NET_QUEUE_CAPACITY];
static int net_q_head = 0;
static int net_q_tail = 0;
static int net_q_count = 0;

static void enqueue_message(NetMessage *m)
{
    if (net_q_count >= NET_QUEUE_CAPACITY)
    {
        NetMessage *old = &net_queue[net_q_head];
        if (old->data) free(old->data);
        net_q_head = (net_q_head + 1) % NET_QUEUE_CAPACITY;
        net_q_count--;
    }
    NetMessage *slot = &net_queue[net_q_tail];
    slot->type = m->type;
    strncpy(slot->peer_host, m->peer_host, sizeof(slot->peer_host));
    slot->peer_host[sizeof(slot->peer_host)-1] = '\0';
    slot->peer_port = m->peer_port;
    slot->channel = m->channel;
    slot->dataLength = m->dataLength;
    slot->data = m->data;
    slot->peer_id = m->peer_id;

    net_q_tail = (net_q_tail + 1) % NET_QUEUE_CAPACITY;
    net_q_count++;
}

bool network_poll_message(NetMessage *out)
{
    if (net_q_count == 0) return false;
    NetMessage *slot = &net_queue[net_q_head];

    out->type = slot->type;
    strncpy(out->peer_host, slot->peer_host, sizeof(out->peer_host));
    out->peer_host[sizeof(out->peer_host)-1] = '\0';
    out->peer_port = slot->peer_port;
    out->channel = slot->channel;
    out->dataLength = slot->dataLength;
    out->data = slot->data;
    out->peer_id = slot->peer_id;

    slot->data = NULL;

    net_q_head = (net_q_head + 1) % NET_QUEUE_CAPACITY;
    net_q_count--;
    return true;
}

void network_free_message(NetMessage *msg)
{
    if (!msg) return;
    if (msg->data) free(msg->data);
    msg->data = NULL;
    msg->dataLength = 0;
}

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
        return;
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
    if (server_host != NULL)
    {
        ENetEvent event;
        while (enet_host_service(server_host, &event, 0) > 0)
        {
            NetMessage m = {0};
            char host_name[64] = {0};
            enet_address_get_host(&event.peer->address, host_name, sizeof(host_name));
            m.type = (event.type == ENET_EVENT_TYPE_CONNECT) ? NET_EVENT_CONNECT :
                     (event.type == ENET_EVENT_TYPE_RECEIVE) ? NET_EVENT_RECEIVE :
                     (event.type == ENET_EVENT_TYPE_DISCONNECT) ? NET_EVENT_DISCONNECT : NET_EVENT_DISCONNECT;
            strncpy(m.peer_host, host_name, sizeof(m.peer_host));
            m.peer_host[sizeof(m.peer_host)-1] = '\0';
            m.peer_port = event.peer->address.port;
            m.channel = (uint8_t)event.channelID;
            m.peer_id = (uintptr_t)event.peer;

            if (event.type == ENET_EVENT_TYPE_RECEIVE)
            {
                m.dataLength = event.packet->dataLength;
                m.data = malloc(m.dataLength);
                if (m.data && event.packet->dataLength > 0)
                    memcpy(m.data, event.packet->data, m.dataLength);
                enet_packet_destroy(event.packet);
            }

            enqueue_message(&m);
        }
    }

    // service client peer
    if (client_peer != NULL && client_peer->host != NULL)
    {
        ENetEvent event;
        while (enet_host_service(client_peer->host, &event, 0) > 0)
        {
            NetMessage m = {0};
            char host_name[64] = {0};
            if (event.peer)
                enet_address_get_host(&event.peer->address, host_name, sizeof(host_name));
            m.type = (event.type == ENET_EVENT_TYPE_CONNECT) ? NET_EVENT_CONNECT :
                     (event.type == ENET_EVENT_TYPE_RECEIVE) ? NET_EVENT_RECEIVE :
                     (event.type == ENET_EVENT_TYPE_DISCONNECT) ? NET_EVENT_DISCONNECT : NET_EVENT_DISCONNECT;
            strncpy(m.peer_host, host_name, sizeof(m.peer_host));
            m.peer_host[sizeof(m.peer_host)-1] = '\0';
            if (event.peer)
                m.peer_port = event.peer->address.port;
            m.channel = (uint8_t)event.channelID;
            m.peer_id = (event.peer) ? (uintptr_t)event.peer : 0;

            if (event.type == ENET_EVENT_TYPE_RECEIVE)
            {
                m.dataLength = event.packet->dataLength;
                m.data = malloc(m.dataLength);
                if (m.data && event.packet->dataLength > 0)
                    memcpy(m.data, event.packet->data, m.dataLength);
                enet_packet_destroy(event.packet);
            }

            enqueue_message(&m);
        }
    }
}

void network_deinitialize()
{
    // free any queued message payloads
    while (net_q_count > 0)
    {
        NetMessage *slot = &net_queue[net_q_head];
        if (slot->data) free(slot->data);
        slot->data = NULL;
        net_q_head = (net_q_head + 1) % NET_QUEUE_CAPACITY;
        net_q_count--;
    }
    enet_deinitialize();
}

static ENetPeer *peer_from_id(uintptr_t peer_id)
{
    if (peer_id == 0) return NULL;
    return (ENetPeer *)peer_id;
}

bool network_send_to_peer(uintptr_t peer_id, const void *data, size_t len, uint8_t channel, bool reliable)
{
    ENetPeer *peer = peer_from_id(peer_id);
    if (peer == NULL) return false;

    if (peer->state != ENET_PEER_STATE_CONNECTED) return false;

    enet_uint32 flags = reliable ? ENET_PACKET_FLAG_RELIABLE : 0;
    ENetPacket *packet = enet_packet_create(data, (size_t)len, flags);
    if (packet == NULL) return false;

    if (enet_peer_send(peer, (enet_uint8)channel, packet) != 0)
    {
        enet_packet_destroy(packet);
        return false;
    }

    if (peer->host) enet_host_flush(peer->host);
    return true;
}

bool network_broadcast(const void *data, size_t len, uint8_t channel, bool reliable)
{
    enet_uint32 flags = reliable ? ENET_PACKET_FLAG_RELIABLE : 0;

    // If server, broadcast to all connected peers
    if (server_host != NULL)
    {
        ENetPacket *packet = enet_packet_create(data, (size_t)len, flags);
        if (packet == NULL) return false;
        enet_host_broadcast(server_host, (enet_uint8)channel, packet);
        enet_host_flush(server_host);
        return true;
    }

    // If client, send to connected server peer
    if (client_peer != NULL)
    {
        return network_send_to_peer((uintptr_t)client_peer, data, len, channel, reliable);
    }

    return false;
}