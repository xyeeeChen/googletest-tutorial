#ifndef MAIL_H_
#define MAIL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char title[16];
  char message[256];
  long timestamp;
} Notification;

Notification createNotification(const char *title, const char *message);

int sendNotification(Notification *notification);

#ifdef __cplusplus
}
#endif

#endif
