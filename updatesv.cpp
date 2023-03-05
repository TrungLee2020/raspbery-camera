#include <iostream>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

int main() {
    const char* server_address = "";  // Set the server IP address
    const int server_port = 8080;  // Set the server port

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

        // Create a socket
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            std::cerr << "Error creating socket" << std::endl;
            return -1;
        }

        // Set up the server address
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(server_address);
        server_addr.sin_port = htons(server_port);

        // Connect to the server
        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            std::cerr << "Error connecting to server" << std::endl;
            close(sock);
            return -1;
        }

        // Send the camera's capabilities to the server
        std::string message = "Device: " + std::string(camera_device) + "\n";
        message += "Driver: " + std::string(cap.driver) + "\n";
        message += "Card: " + std::string(cap.card) + "\n";
        message += "Bus info: " + std::string(cap.bus_info) + "\n";
        if (send(sock, message.c_str(), message.size(), 0) == -1) {
            std::cerr << "Error sending message to server" << std::endl;
            close(sock);
            return -1;
        }

        // Close the socket and camera device
        close(sock);
        close(fd);

        camera_index++;
    }

    return 0;
}