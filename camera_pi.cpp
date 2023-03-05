// To get the address of a USB camera connected to a Raspberry Pi using C++, you can use the V4L2 (Video for Linux 2) API
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

int main() {
    int fd;
    struct v4l2_capability cap;

    // Open the camera device
    fd = open("/dev/video0", O_RDWR);
    if (fd == -1) {
        perror("Failed to open camera");
        exit(EXIT_FAILURE);
    }

    // Query the camera capabilities
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
        perror("Failed to query camera capabilities");
        exit(EXIT_FAILURE);
    }

    // Print the camera address
    printf("Camera address: %s\n", cap.bus_info);

    // Close the camera device
    close(fd);

    return 0;
}
