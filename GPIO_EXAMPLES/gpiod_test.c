#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GPIO_CHIP_NAME "gpiochip0"
#define GPIO_LINE_OFFSET 17 // Example GPIO pin number

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int ret;

    // Open GPIO chip
    chip = gpiod_chip_open_by_name(GPIO_CHIP_NAME);
    if (!chip) {
        perror("gpiod_chip_open_by_name");
        return EXIT_FAILURE;
    }

    // Get GPIO line
    line = gpiod_chip_get_line(chip, GPIO_LINE_OFFSET);
    if (!line) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Request GPIO line as output
    ret = gpiod_line_request_output(line, "example", 0);
    if (ret < 0) {
        perror("gpiod_line_request_output");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Toggle GPIO line
    while (1) {
        gpiod_line_set_value(line, 1);
        sleep(1);
        gpiod_line_set_value(line, 0);
        sleep(1);
    }

    // Release GPIO line and close chip
    gpiod_line_release(line);
    gpiod_chip_close(chip);

    return EXIT_SUCCESS;
}
