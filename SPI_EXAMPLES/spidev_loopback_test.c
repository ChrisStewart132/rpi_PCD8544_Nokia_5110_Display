#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE "/dev/spidev0.0"
#define BUFFER_SIZE 32

/*
    program using the SPI0 pins, MOSI and MISO should be connected for a write and read loopback test
	gcc -o spidev_loopback_test spidev_loopback_test.c
	./spidev_loopback_test
*/
int main() {
    int fd;
    int ret;
    uint8_t tx_buffer[BUFFER_SIZE];
    uint8_t rx_buffer[BUFFER_SIZE];
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx_buffer,
        .rx_buf = (unsigned long)rx_buffer,
        .len = BUFFER_SIZE,
        .speed_hz = 500000,
        .delay_usecs = 0,
        .bits_per_word = 8,
        .cs_change = 0,
    };

    // Initialize the transmit buffer with test data
    for (int i = 0; i < BUFFER_SIZE; i++) {
        tx_buffer[i] = i;
    }

    // Open the SPI device
    fd = open(SPI_DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open SPI device");
        exit(EXIT_FAILURE);
    }

    // Perform the SPI data transfer (loopback test)
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1) {
        perror("Failed to send SPI message");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Print the transmitted and received data
    printf("Transmitted data:\n");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("0x%02X ", tx_buffer[i]);
    }
    printf("\n");

    printf("Received data:\n");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("0x%02X ", rx_buffer[i]);
    }
    printf("\n");

    // Close the SPI device
    close(fd);
    return 0;
}