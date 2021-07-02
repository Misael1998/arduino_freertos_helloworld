#include <errno.h> /* Error number definitions */
#include <fcntl.h> /* File control definitions */
#include <stdio.h>
#include <string.h>  /* String function definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <unistd.h>  /* UNIX standard function definitions */

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "taskHandle.h"

unsigned long ulIdleCycleCount = 0UL;

void vTask2(void *pvParameters);

void vApplicationIdleHook(void) { ulIdleCycleCount++; }

void vSenderTask(void *pvParameters) {
  long lValueToSend;
  portBASE_TYPE xStatus;

  lValueToSend = (long)pvParameters;

  for (;;) {
    xStatus = xQueueSendToBack(xQueue, &lValueToSend, 0);

    if (xStatus != pdPASS) {
      printf("Could not send to the queue \r\n");
    }

    taskYIELD();
  }
}

void vRecieverTask(void *pvParameters) {
  long lReceivedValue;
  portBASE_TYPE xStatus;
  const portTickType xTickToWait = 100 / portTICK_RATE_MS;

  for (;;) {
    if (uxQueueMessagesWaiting(xQueue) != 0) {
      printf("Queue should have been empty\r\n");
    }

    xStatus = xQueueReceive(xQueue, &lReceivedValue, xTickToWait);

    if (xStatus == pdPASS) {
      printf("Received = %li\r\n", lReceivedValue);
    }
  }
}

void vPortTask(void *pvParameters) {
  int *fd;
  char buffer[255];
  char *bufptr; /* Current char in buffer */
  int nbytes;   /* Number of bytes read */
  int tries;
  int count = 0;
  fd = (int *)pvParameters;
  bzero(buffer, sizeof(buffer));

  char *msg = "misa";

  for (;;) {
    write(*fd, msg, sizeof(msg));
    bufptr = buffer;
    while ((nbytes = read(*fd, bufptr, buffer + sizeof(buffer) - bufptr - 1)) >
           0) {
      bufptr += nbytes;
      count++;
      printf("%d%s\n", count, buffer);
      if (bufptr[-1] == '\n' || bufptr[-1] == '\r') break;
    }
    bzero(buffer, sizeof(buffer));
    vTaskDelay(1000);
  }
}
