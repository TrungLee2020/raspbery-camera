#include <iostream>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main() {
    const char* camera_device_template = "/dev/video%d";  // Set the camera device path template
    int camera_index = 0;  // Set the initial camera index

    // Loop over camera devices until one is not found
    while (true) {
        // Build the camera device path
        char camera_device[20];
        sprintf(camera_device, camera_device_template, camera_index);

        // Open the camera device
        int fd = open(camera_device, O_RDWR);
        if (fd == -1) {
            // Camera not found, exit the loop
            break;
        }

        // Query the camera's capabilities
        struct v4l2_capability cap;
        if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
            std::cerr << "Error querying camera capabilities for device: " << camera_device << std::endl;
            return -1;
        }

        // Output the camera's capabilities
        std::cout << "Device: " << camera_device << std::endl;
        std::cout << "Driver: " << cap.driver << std::endl;
        std::cout << "Card: " << cap.card << std::endl;
        std::cout << "Bus info: " << cap.bus_info << std::endl;

        close(fd);
        camera_index++;
    }

    return 0;
}