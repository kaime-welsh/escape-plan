#ifndef NETWORK_H
#define NETWORK_H

void network_initialize();
void network_deinitialize();
void network_host();
void network_connect(const char* hostAddress);
void network_update();

#endif // NETWORK_H