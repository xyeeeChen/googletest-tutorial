#include "mockNetworkInterface.h"

#include <stdio.h>

#include "network.h"

unique_ptr<MockNetwork> mockNetwork;

int sendToServer(char *buf, int size) {
  return mockNetwork->sendToServer(buf, size);
}
