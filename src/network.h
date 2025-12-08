#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    NET_EVENT_CONNECT,
    NET_EVENT_RECEIVE,
    NET_EVENT_DISCONNECT
} NetEventType;

typedef struct
{
    NetEventType type;
    char peer_host[64]; // printable IP (IPv4 dotted or hostname)
    uint16_t peer_port;
    uintptr_t peer_id; // opaque identifier for the peer (caller shouldn't dereference)
    uint8_t channel;
    uint8_t *data;     // pointer to payload (owned by caller after poll)
    size_t dataLength; // payload length
} NetMessage;

// Core network lifecycle
void network_initialize();
void network_deinitialize();
void network_host();
void network_connect(const char *hostAddress);
void network_update();

// Message API (decouples ENet from application code)
// - `network_poll_message` returns true if a message/event was available
//   and fills the provided `out` structure. The caller takes ownership
//   of `out->data` and must call `network_free_message` when done.
bool network_poll_message(NetMessage *out);
void network_free_message(NetMessage *msg);

// Send API
// - `peer_id` is the opaque identifier returned in `NetMessage.peer_id`.
// - `channel` is the ENet channel to send on (0..).
// - `reliable` controls whether the packet is reliable.
// Returns true on success (packet queued for send).
bool network_send_to_peer(uintptr_t peer_id, const void *data, size_t len, uint8_t channel, bool reliable);

// Broadcast to all peers. On a server this sends to all connected peers.
// On a client this will send to the connected server peer (if any).
bool network_broadcast(const void *data, size_t len, uint8_t channel, bool reliable);

#endif // NETWORK_H