/**
 * Program that reads from stdin a stream of bitmaps and presents it onto a Nokia 5110 LCD *  
 * 
 * gcc -o Nokia_5110_bitmap_stream_from_stdin_V2 Nokia_5110_bitmap_stream_from_stdin_V2.c -lgpiod
 * python3 picamera_stream_to_stdout_V2.py | ./Nokia_5110_bitmap_stream_from_stdin_V2
 */

#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define GPIO_CHIP_NAME "gpiochip0"
#define GPIO_OFFSET_DC 17 	// DATA(high)/COMMAND(low)
#define GPIO_OFFSET_RST 27 	// RESET active low

#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_HZ 4000000
/**
 * SCE
 * MOSI
 * CLK
 */
         
void _spi_write(int fd, uint8_t* tx_buffer, int size){
	//usleep(1000*100);
	int ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx_buffer,
		.rx_buf = (unsigned long)NULL,
		.len = size,
		.speed_hz = SPI_HZ,
		.delay_usecs = 0,
		.bits_per_word = 8,
		.cs_change = 0,
	};
	//printf("write 0x%02X\n", tx_buffer[0]);
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1) {
		perror("Failed to send SPI message");
		close(fd);
		exit(EXIT_FAILURE);
	}
}
void _gpio_high(struct gpiod_line** line){
	gpiod_line_set_value(*line, 1);	
}
void _gpio_low(struct gpiod_line** line){
	gpiod_line_set_value(*line, 0);	
}

void init_gpio(struct gpiod_chip** chip, struct gpiod_line** dc, struct gpiod_line** rst){
	int ret;
	// Open GPIO chip
	*chip = gpiod_chip_open_by_name(GPIO_CHIP_NAME);
	if (!chip[0]) {
		perror("gpiod_chip_open_by_name");
		exit(EXIT_FAILURE);
	}
	// Get GPIO line
	*dc = gpiod_chip_get_line(*chip, GPIO_OFFSET_DC);
	if (!dc[0]) {
		perror("gpiod_chip_get_line");
		gpiod_chip_close(*chip);
		exit(EXIT_FAILURE);
	}
	// Get GPIO line
	*rst = gpiod_chip_get_line(*chip, GPIO_OFFSET_RST);
	if (!rst[0]) {
		perror("gpiod_chip_get_line");
		gpiod_chip_close(*chip);
		exit(EXIT_FAILURE);
	}
	// Request GPIO line as output
	ret = gpiod_line_request_output(*dc, "Nokia_5110_dc", 0);
	if (ret < 0) {
		perror("gpiod_line_request_output");
		gpiod_chip_close(*chip);
		exit(EXIT_FAILURE);
	}
	// Request GPIO line as output
	ret = gpiod_line_request_output(*rst, "Nokia_5110_rst", 0);
	if (ret < 0) {
		perror("gpiod_line_request_output");
		gpiod_chip_close(*chip);
		exit(EXIT_FAILURE);
	}
}

void init_spi(int* fd){
	// Open the SPI device
	*fd = open(SPI_DEVICE, O_RDWR);
	if (*fd < 0) {
		perror("Failed to open SPI device");
		exit(EXIT_FAILURE);
	}
}

void init(int fd, struct gpiod_line** dc, struct gpiod_line** rst){
	// reset
	printf("reseting\n");
	_gpio_low(rst);
	usleep(50000);
	_gpio_high(rst);
	usleep(50000);
	
	// setup commands
	printf("setting up\n");
	_gpio_low(dc);
	uint8_t cmd;
	
	cmd = (1<<5) + (1<<1) + (1<<0);
	_spi_write(fd, &cmd, 1);// H=1 V=1
	
	cmd = (1<<7) + (1<<4);
	_spi_write(fd, &cmd, 1);// set Vop

	cmd = (1<<5) + (1<<1);
	_spi_write(fd, &cmd, 1);// H=0 V=1

	cmd = (1<<3) + (1<<2) + (1<<0);
	_spi_write(fd, &cmd, 1);// D=1,E=1 (display mode set inverse mode)
}

void reset_cursor(int fd, struct gpiod_line** dc){
	_gpio_low(dc);// cmd mode
	uint8_t cmd;
	cmd = (0<<7) + (1<<6) + (0<<5) + (0<<4) + (0<<3) + (0<<2) + (0<<1) + (0<<0);
	_spi_write(fd, &cmd, 1);// set y=0 (0-5)
	
	cmd = (1<<7) + (0<<6) + (0<<5) + (0<<4) + (0<<3) + (0<<2) + (0<<1) + (0<<0);
	_spi_write(fd, &cmd, 1);// set x=0 (0-83)
}

void display_buffer(int fd, struct gpiod_line** dc, char bitmap[48][84]){
	// assuming vertical addressing
	//printf("displaying bitmap\n");
	//reset_cursor(fd, dc);

	// Buffer to hold the entire command set to be transferred
    uint8_t transfer_buffer[504]; // 84 columns * 6 rows = 504 bytes
    int buffer_index = 0;

	_gpio_high(dc);// data mode
	for(int x = 0; x < 84; x++){
		for(int y = 0; y < 6; y++){
			// read in bits
			uint8_t cmd = 0;
			for(int i = y*8+7; i >= y*8; i--){
				int bit = bitmap[i][x];
				cmd <<= 1;
				cmd += bit;
			}
			transfer_buffer[buffer_index++] = cmd;
			//_spi_write(fd, &cmd, 1);
		}
	}
	_spi_write(fd, transfer_buffer, 504);
}

void display_clear(int fd, struct gpiod_line** dc){
	//printf("clearing display\n");
	reset_cursor(fd, dc);
	_gpio_high(dc);// data mode
	for(int i = 0; i < 6*84; i++){
		uint8_t cmd = 0;
		_spi_write(fd, &cmd, 1);
	}
}

void load_buffer_from_stream_stdin(char bitmap[48][84]){
	for(int i = 0; i < 48; i++){
		/*
		for(int j = 0; j < 84; j++){
			read(0, &(bitmap[i][j]), 1);
		}*/
		read(0, &(bitmap[i][0]), 84);// image skewed?
	}
}

int main() {
	struct gpiod_chip* chip;
	struct gpiod_line* dc;
	struct gpiod_line* rst;
	
	init_gpio(&chip, &dc, &rst);
	
	int fd;// SPI file
	init_spi(&fd);

	init(fd, &dc, &rst);
	display_clear(fd, &dc);
	char bitmap[48][84] = {{0}};
	while(1){
		load_buffer_from_stream_stdin(bitmap);
		display_buffer(fd, &dc, bitmap);
		//usleep(100);
		//display_clear(fd, &dc);
	}

	if(dc)
		gpiod_line_release(dc);
	if(rst)
		gpiod_line_release(rst);
	if(chip)
		gpiod_chip_close(chip);

	close(fd);
	return EXIT_SUCCESS;
}
