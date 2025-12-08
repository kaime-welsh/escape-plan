#include "playing_state.h"
#include "raylib.h"
#include "network.h"
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "engine.h"

static Camera3D camera;
static char last_net_msg[256] = "(no network events yet)";
static uintptr_t last_peer_id = 0;

void playing_state_load()
{
    camera = (Camera3D){
        .position = (Vector3){0.0f, 10.0f, 10.0f},
        .target = (Vector3){0.0f, 0.0f, 0.0f},
        .up = (Vector3){0.0f, 1.0f, 0.0f},
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE};
}

void playing_state_update()
{
    // If the local player (client) pressed space, send a PING to the server
    if (local_client && IsKeyPressed(KEY_SPACE))
    {
        const char ping[] = "PING";
        network_broadcast(ping, sizeof(ping) - 1, 0, true);
        snprintf(last_net_msg, sizeof(last_net_msg), "SENT PING to server");
    }

    NetMessage msg;
    while (network_poll_message(&msg))
    {
        switch (msg.type)
        {
        case NET_EVENT_CONNECT:
            snprintf(last_net_msg, sizeof(last_net_msg), "CONNECT from %s:%u (peer_id=%" PRIuPTR ")",
                     msg.peer_host, msg.peer_port, msg.peer_id);
            last_peer_id = msg.peer_id;
            break;
        case NET_EVENT_RECEIVE:
        {
            // For demo: show up to 128 bytes as a printable string (safely)
            size_t len = msg.dataLength;
            size_t show = len < 128 ? len : 128;
            char buf[129] = {0};
            if (msg.data && show > 0)
            {
                memcpy(buf, msg.data, show);
                // ensure printable
                for (size_t i = 0; i < show; ++i)
                    if (buf[i] < 32 || buf[i] > 126) buf[i] = '.';
            }
            snprintf(last_net_msg, sizeof(last_net_msg), "RECV %zu bytes from %s (peer_id=%" PRIuPTR "): '%s'",
                     msg.dataLength, msg.peer_host, msg.peer_id, buf);
            // If server received a PING, reply with PONG to that peer
            if (is_server && strcmp(buf, "PING") == 0)
            {
                const char pong[] = "PONG";
                network_send_to_peer(msg.peer_id, pong, sizeof(pong) - 1, 0, true);
            }
            // If client received a PONG, update display
            if (local_client && strcmp(buf, "PONG") == 0)
            {
                snprintf(last_net_msg, sizeof(last_net_msg), "RECV PONG from %s (peer_id=%" PRIuPTR ")",
                         msg.peer_host, msg.peer_id);
            }
            last_peer_id = msg.peer_id;
            break;
        }
        case NET_EVENT_DISCONNECT:
            snprintf(last_net_msg, sizeof(last_net_msg), "DISCONNECT %s (peer_id=%" PRIuPTR ")",
                     msg.peer_host, msg.peer_id);
            last_peer_id = msg.peer_id;
            break;
        default:
            snprintf(last_net_msg, sizeof(last_net_msg), "UNKNOWN event from %s (peer_id=%" PRIuPTR ")",
                     msg.peer_host, msg.peer_id);
            break;
        }

        network_free_message(&msg);
    }
}

void playing_state_render()
{
    ClearBackground(BLACK);
    DrawText("Playing State", 32, 32, 48, WHITE);
    DrawText(last_net_msg, 32, 100, 20, LIGHTGRAY);
}

void playing_state_unload()
{
}