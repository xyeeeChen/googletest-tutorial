#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mail.h"
#include "mockNetworkInterface.h"

using std::unique_ptr;
using ::testing::_;
using ::testing::Environment;
using ::testing::Return;

TEST(Send, send_a_notification_expected_success) {
  Notification notification = createNotification("Hello", "google test");
  EXPECT_CALL(*mockNetwork, sendToServer(_, _)).WillOnce(Return(0));

  int result = sendNotification(&notification);
  EXPECT_EQ(result, 0);
}

class TestEnvironment : public Environment {
 public:
  void SetUp() {
    mockNetwork = unique_ptr<MockNetwork>(new MockNetwork);
  }
  void TearDown() {
    mockNetwork = nullptr;
  }
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new TestEnvironment);
  return RUN_ALL_TESTS();
}
