#include <stdio.h>
#define POLY 0x07  // CRC-8 polynomial (x^8 + x^2 + x + 1)

unsigned char calc_checksum(FILE *f) {
    unsigned int sum = 0;
    int byte;
    while ((byte = fgetc(f)) != EOF)
        sum += byte;
    return ~(sum & 0xFF);
}

unsigned char calc_crc(FILE *f) {
    unsigned char crc = 0;
    int byte;
    while ((byte = fgetc(f)) != EOF) {
        crc ^= byte;
        for (int i = 0; i < 8; i++)
            crc = (crc & 0x80) ? (crc << 1) ^ POLY : (crc << 1);
    }
    return crc;
}

void send_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) { printf("File not found!\n"); return; }

    unsigned char checksum = calc_checksum(f);
    fclose(f);

    f = fopen(filename, "rb");
    unsigned char crc = calc_crc(f);
    fclose(f);

    printf("\n--- File Sent ---\n");
    printf("Checksum: %02X\n", checksum);
    printf("CRC: %02X\n", crc);

    FILE *out = fopen("transmit_info.txt", "w");
    fprintf(out, "%02X %02X", checksum, crc);
    fclose(out);
    printf("Checksum and CRC stored in transmit_info.txt\n\n");
}

void verify_file(const char *filename) {
    unsigned int stored_checksum, stored_crc;
    FILE *info = fopen("transmit_info.txt", "r");
    if (!info) { printf("No stored info found!\n"); return; }
    fscanf(info, "%x %x", &stored_checksum, &stored_crc);
    fclose(info);

    FILE *f = fopen(filename, "rb");
    if (!f) { printf("File not found!\n"); return; }

    unsigned char new_checksum = calc_checksum(f);
    fclose(f);

    f = fopen(filename, "rb");
    unsigned char new_crc = calc_crc(f);
    fclose(f);

    printf("\n--- Verification ---\n");
    printf("Stored Checksum: %02X | New Checksum: %02X\n", stored_checksum, new_checksum);
    printf("Stored CRC: %02X | New CRC: %02X\n", stored_crc, new_crc);

    if (stored_checksum == new_checksum && stored_crc == new_crc)
        printf("Verification Result: No Error Detected\n\n");
    else
        printf("Verification Result: Error Detected!\n\n");
}

int main() {
    int choice;
    char filename[100] = "./input.txt";

    do {
        printf("====== FILE ERROR DETECTION ======\n");
        printf("1. Send File (Calculate Checksum & CRC)\n");
        printf("2. Verify File\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: send_file(filename); break;
            case 2: verify_file(filename); break;
            case 3: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 3);

    return 0;
}
