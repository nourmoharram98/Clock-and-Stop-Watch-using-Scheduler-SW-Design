#include <stdio.h>
#include <stdint.h>

#define DATA_SIZE 4 // Size of data in bits

// Communication Manager function
void Communication_Manager(uint8_t RAW_DATA) {
    uint8_t processed_data = append_crc(RAW_DATA);
    printf("Transmitted data with CRC: %X\n", processed_data);

    // Simulating receiving data
    process_received_data(processed_data);
}

// Function to append CRC to data
uint8_t append_crc(uint8_t data) {
    uint8_t crc = calculate_crc(data);
    return (data << 4) | crc; // Append CRC to data
}

// Function to calculate CRC checksum
uint8_t calculate_crc(uint8_t data) {
    // CRC polynomial: x^4 + x^3 + 1, represented as 0b11011
    uint8_t crc_poly = 0b1101;
    uint8_t crc = 0; // Initialize CRC to 0

    // Perform CRC calculation
    for (int i = 0; i < DATA_SIZE; i++) {
        // XOR CRC with current data bit
        crc ^= (data >> (DATA_SIZE - 1 - i)) & 0x01;
        if (crc & 0x01) 
        {
            crc ^= crc_poly;
        }
        crc >>= 1; // Shift CRC to the right
    }

    return crc & 0x0F; // Ensure CRC is 4 bits long
}

// Function to process received data and verify CRC
void process_received_data(uint8_t received_data) {
    uint8_t received_crc = received_data & 0x0F; // Extract received CRC
    uint8_t received_data_only = received_data >> 4; // Extract received data

    uint8_t calculated_crc = calculate_crc(received_data_only); // Calculate CRC for received data

    // Verify CRC
    if (received_crc == calculated_crc) {
        printf("Data received successfully: %X\n", received_data_only);
    } else {
        printf("Error in received data!\n");
    }
}

int main() {
    uint8_t input_data = 3; // Example input data

    Communication_Manager(input_data);

    return 0;
}
