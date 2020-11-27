#include "mail.h"

#include <string.h>
#include <time.h>

#include "network.h"

Notification createNotification(const char *title, const char *message) {
  Notification notification = {};
  strncpy(notification.title, title, sizeof(notification.title));
  strncpy(notification.message, message, sizeof(notification.message));
  notification.timestamp = time(NULL);
  return notification;
}

int sendNotification(Notification *notification) {
  int sent = sendToServer((void *)notification, sizeof(Notification));
  return sent > 0;
}
