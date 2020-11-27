#include <gmock/gmock.h>

#include "network.h"

class MockNetwork {
 public:
  MOCK_METHOD2(sendToServer, int(char *, int));
};

using std::unique_ptr;

extern unique_ptr<MockNetwork> mockNetwork;
