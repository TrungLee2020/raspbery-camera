#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

int main() {
    int fd_usb, fd_pi;
    struct v4l2_capability cap_usb, cap_pi;

    // Open the USB camera device
    fd_usb = open("/dev/video0", O_RDWR);
    if (fd_usb == -1) {
        perror("Failed to open USB camera");
        exit(EXIT_FAILURE);
    }

    // Query the USB camera capabilities
    if (ioctl(fd_usb, VIDIOC_QUERYCAP, &cap_usb) == -1) {
        perror("Failed to query USB camera capabilities");
        exit(EXIT_FAILURE);
    }

    // Print the USB camera address
    printf("USB camera address: %s\n", cap_usb.bus_info);

    // Close the USB camera device
    close(fd_usb);

    // Open the Raspberry Pi camera device
    fd_pi = open("/dev/video1", O_RDWR);
    if (fd_pi == -1) {
        perror("Failed to open Raspberry Pi camera");
        exit(EXIT_FAILURE);
    }

    // Query the Raspberry Pi camera capabilities
    if (ioctl(fd_pi, VIDIOC_QUERYCAP, &cap_pi) == -1) {
        perror("Failed to query Raspberry Pi camera capabilities");
        exit(EXIT_FAILURE);
    }

    // Print the Raspberry Pi camera address
    printf("Raspberry Pi camera address: %s\n", cap_pi.bus_info);

    // Close the Raspberry Pi camera device
    close(fd_pi);

    return 0;
}
/* In this code, we open both cameras using their device paths
 (/dev/video0 for the USB camera and /dev/video1 for the Raspberry Pi camera)
  and query their capabilities using the VIDIOC_QUERYCAP argument to the ioctl() function. 
  We then print the camera addresses, which are stored in the bus_info field of the v4l2_capability struct, 
  using the printf() function. Finally, we close both camera devices using the close() function. */