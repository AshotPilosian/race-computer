#include <iostream>
#include <wiringx.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "gps/GPS.h"
#include "display/Display.h"

int init_wiringX() {
    printf("Setting up wiringX\n");

    system("duo-pinmux -w GP2/UART1_TX && duo-pinmux -w GP3/UART1_RX");

    if (wiringXSetup("milkv_duo256m", NULL) == -1) {
        wiringXGC();
        return 1;
    }
    sleep(1);
    printf("wiringX setup completed\n");

    return 0;
}

Display display;
GPS gps;

void setup() {
    printf("Setup started\n");

    display.setup();
    init_wiringX();
    gps.setup();

    printf("Setup finished\n");
}

void shutdown() {
    printf("Shutdown started\n");

    display.shutdown();
    init_wiringX();
    gps.shutdown();

    printf("Shutdown finished\n");
}

int main() {
    setup();

    printf("Application started\n");
    gps.start();
    printf("Application finished\n");

    shutdown();

    return 0;
}