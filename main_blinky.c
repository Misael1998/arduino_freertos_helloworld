#include <errno.h> /* Error number definitions */
#include <fcntl.h> /* File control definitions */
#include <stdio.h>
#include <string.h>  /* String function definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <unistd.h>  /* UNIX standard function definitions */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "taskHandle.h"

void vPortTask(void *pvParameters);

void main_blinky(void) {
  char *port = "/dev/cu.usbserial-1420";
  struct termios options;

  int *fd;

  *fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if (*fd == -1) {
    /*
     * Could not open the port.
     */

    perror("open_port: Unable to open /dev/ttyf1 - ");
  } else {
    fcntl(*fd, F_SETFL, FNDELAY);
  }

  /* set raw input, 1 second timeout */
  options.c_cflag |= (CLOCAL | CREAD);
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_oflag &= ~OPOST;
  options.c_cc[VMIN] = 0;
  options.c_cc[VTIME] = 10;

  /* set the options */
  tcsetattr(*fd, TCSANOW, &options);

  xTaskCreate(vPortTask, "port task", 1000, (int *)fd, 1, NULL);
  vTaskStartScheduler();

  for(;;){}

}
