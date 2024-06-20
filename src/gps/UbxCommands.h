//
// Created by Ashot on 6/20/2024.
//

#ifndef RACE_COMPUTER_UBXCOMMANDS_H
#define RACE_COMPUTER_UBXCOMMANDS_H

static unsigned char UBX_CFG_UART_BAUD_115200[] = {
        0xB5, 0x62,  // UBX header
        0x06, 0x8A,  // Message class and ID for CFG-VALSET
        0x0C, 0x00,  // Payload length (19 bytes)
        0x01,        // Version
        0x01,        // Layer 0 - RAM, 1 - BBR, 2 - Flash
        0x00, 0x00,  // Reserved
        0x01, 0x00, 0x52, 0x40,
        0x00, 0xC2, 0x01, 0x00,
        0xF4, 0xB1
};

#endif //RACE_COMPUTER_UBXCOMMANDS_H
