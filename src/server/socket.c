#include "common.h"

/********************************************
 *  SOCKET FUNCTIONS
 ****************************************************/

int sockwrite(int sockfd, char *pattern, ...) {
  char buf[512];
  int total = 0;
  int bytesleft;
  int n = 0;

  va_list ap;
  va_start(ap, pattern);
  vsprintf(buf, pattern, ap);
  va_end(ap);

  bytesleft = strlen(buf);

  printf_("%lu SEND[%d]: %s", servertime, sockfd, buf);

  // if (send(sockfd, buf, strlen(buf), 0) == -1)
  //	perror("send");

  while ((unsigned)total < strlen(buf)) {
    n = send(sockfd, &buf[total], bytesleft, 0);
    if (n == -1) {
      break;
    }
    total += n;
    bytesleft -= n;
  }

  // *len = total; // return number actually sent here

  return n == -1 ? -1 : 0;  // return -1 on failure, 0 on success

  return 0;
}

int sockwrite_all(char *pattern, ...) {
  char buf[512];
  int total;
  int bytesleft;
  int n;
  LINK current; /* arghl kansloos gedoe */

  va_list ap;
  va_start(ap, pattern);
  vsprintf(buf, pattern, ap);
  va_end(ap);

  current = head;

  while (current != NULL) {
    if (current->bullet != 1 && current->enabled == 1 && current->bot != 1) {
      bytesleft = strlen(buf);
      total = 0;

      printf_("%lu SEND[%d]: %s\n", servertime, current->id, buf);

      while ((unsigned)total < strlen(buf)) {
        n = send(current->id, &buf[total], bytesleft, 0);
        if (n == -1) {
          break;
          printf_("friggen ERROR!");
        }
        total += n;
        bytesleft -= n;
      }
    }
    current = current->next;
  }

  return 0;
}

/* send to all except one */
int sockwrite_notall(int except_id, char *pattern, ...) {
  char buf[512];
  int total;
  int bytesleft;
  int n;
  LINK current;

  va_list ap;
  va_start(ap, pattern);
  vsprintf(buf, pattern, ap);
  va_end(ap);

  current = head;

  while (current != NULL) {
    if ((except_id != current->id) && (current->bullet != 1) && (current->enabled != 0) && (current->bot != 1)) {
      printf_("to sockid: %d\n", current->id);
      bytesleft = strlen(buf);
      total = 0;
      printf_("%lu SEND[%d]: %s\n", servertime, current->id, buf);

      while ((unsigned)total < strlen(buf)) {
        n = send(current->id, &buf[total], bytesleft, 0);
        if (n == -1) {
          printf_("friggen ERROR!\n");
          break;
        }
        total += n;
        bytesleft -= n;
      }
    }
    current = current->next;
  }

  return 0;
}

int sendall(int s, char *buf, int *len) {
  int total = 0;         // how many bytes we've sent
  int bytesleft = *len;  // how many we have left to send
  int n = 0;

  while (total < *len) {
    n = send(s, buf + total, bytesleft, 0);
    if (n == -1) {
      break;
    }
    total += n;
    bytesleft -= n;
  }

  *len = total;  // return number actually sent here

  return n == -1 ? -1 : 0;  // return -1 on failure, 0 on success
}
