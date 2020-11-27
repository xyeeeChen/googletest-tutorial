#include <gtest/gtest.h>

#include <stdio.h>

#include "mail.h"

TEST(Create, create_a_notification) {
  Notification notification = createNotification("Hello", "google test");
  EXPECT_STREQ(notification.title, "Hello");
  EXPECT_STREQ(notification.message, "google test");
  EXPECT_GT(notification.timestamp, 0);
}

class NotificationTest : public testing::Test {
 protected:
  const char *title = "Hi";
  void SetUp() override {
    printf("SetUp runs before each case.\n");
  }

  void TearDown() override {
    printf("TearDown runs after each case.\n");
  }
};

TEST_F(NotificationTest, create_a_notification2) {
  Notification notification = createNotification(title, "google test2");
  EXPECT_STREQ(notification.title, title);
  EXPECT_STREQ(notification.message, "google test2");
}

TEST_F(NotificationTest, create_a_notification3) {
  Notification notification = createNotification(title, "google test3");
  EXPECT_STREQ(notification.title, title);
  EXPECT_STREQ(notification.message, "google test3");
}
