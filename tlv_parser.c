#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "functions.c"

bool clean_string(char* string) {
    if (string == NULL) return false;
    for (int i = 0; string[i] != '\0'; i++) {
        // If current character is not a digit or letter move each next index of string 1 place forward
        char c = string[i];
        if (isalnum(c) == 0) {
            int j = i;
            while (string[j] != '\0') {
                string[j] = string[j+1];
                j++;
            }
            i--;
        }
        // end the program if the current number is not a hexadecimal
        else if ((c < '0' || c > '9') && (c < 'A' || c > 'F')) {
            printf("Error: TLV must be an even number of hexadecimal digits\n");
            return false;
        }
    }
    if (strlen(string) % 2 != 0) {
        printf("Error: TLV must be an even number of hexadecimal digits\n");
        return false;
    }
    return true;
}

bool tag_encode(char* tag, bool first_byte, bool *constructed) {
    char* binaryTag = hexadecimal_to_binary(tag);
    //printf("Binary tag: %s\n",binaryTag);
    if (first_byte) {
        if (binaryTag[2] == '1') {
            *constructed = true;
        }
        // Iterate through the first 5 bits of the tag
        for (int i = 3; i <= 7; i++) {
            // If 1 bit is equal to 0 zero return false, i.e. short tag
            if (binaryTag[i] != '1') {
                return false;
            }
        }
        return true;
    }
    // Check if it's not the first byte and then check the 8th bit if it's the last byte
    if (binaryTag[8] == 1) {
        return true;
    }
    return false;
}

int length_encode(char* length, bool *length_of_length) {
    // if length is smaller than 128
    int decimalLength = hexadecimal_to_decimal(length);
    if (decimalLength < 128) {
        return decimalLength;    
    }
    *length_of_length = true;
    // to get how many bytes is the length, convert the length to binary and make bit8 equal 0
    char* binaryLength = hexadecimal_to_binary(length);
    binaryLength[0] = '0';
    // then convert it to decimal
    int bytes_of_length = binary_to_decimal(binaryLength);
    return bytes_of_length;
}

int length_of_length_encode(char* length) {
    int decimalLength = hexadecimal_to_decimal(length);
    return decimalLength;
}

// tlv will be moved 2 places left, after every print of tag, length or value,
void parse_tlv(char* tlv) {
    // base case
    if (tlv[0] == '\0') {
        return;
    }

    // rec case
    char current[3]; // current for the tags, lengths and values, stores a byte
    memcpy(current, tlv, 2);
    //current[2] = '\0';
    bool first = true;
    bool constructed_object = false;
    printf("Tag: %s", current);
    // Loot till tag_encode return false, i.e the tag's last byte
    while (tag_encode(current, first, &constructed_object)) {
        tlv += 2;
        memcpy(current, tlv, 2);
        printf("%s", current);
        first = false;
    }
    if (constructed_object) {
        printf(" Template");
    }
    printf("\n");
    tlv += 2;
    memcpy(current, tlv, 2);
    first = true;
    // Get the length and move tvl two places left, if length is not length of length
    bool length_of_length = false;
    int length = length_encode(current, &length_of_length);
    if (length_of_length) {
        // add to length the values of the next bytes in the tlv, counter times or previous length
        printf("Length of lenght field: %i\n", length);
        // length is the bytes of the length field, so multiply length by 2 and get the bytes
        int bytes = length * 2;
        char length_field[bytes+1];
        tlv += 2;
        memcpy(length_field, tlv, bytes);
        tlv += bytes - 2;  // so I can continue like normal
        length = length_of_length_encode(length_field);
    }
    printf("Length: %i\n", length);
    // Go inside the constructed object if it is
    if (constructed_object) {
        parse_tlv(tlv+2);
        return;
    }
    tlv += 2;
    memcpy(current, tlv, 2);
    // Print the values of the current package
    printf("Value: ");
    for (int i = 0; i < length; i++) {
        if (strlen(current) < 2) {
            printf("Error: Short Value, expected length %i, got %i", length, i);
            return;
        }
        printf("%s ", current);
        tlv += 2;
        memcpy(current, tlv, 2);
    }
    printf("\n\n");
    parse_tlv(tlv);
}

// TLVs
//  9F0206000000001000
// 61 1F 4F 08 A0 00 00 00 25 01 05 01 50 10 50 65 72 73 6F 6E 61 6C 20 41 63 63 6F 75 6E 74 87 01 01
// 61 1E 4F 07 A0 00 00 00 29 10 10 50 10 50 65 72 73 6F 6E 61 6C 20 41 63 63 6F 75 6E 74 87 01 02
// 77 1E 9F 27 01 80 9F 36 02 02 13 9F 26 08 2D F3 83 3C 61 85 5B EA 9F 10 07 06 84 23 00 31 02 08 
// 77 22 82 02 78 00 94 1C 10 01 04 00 10 05 05 00 08 06 06 01 08 07 07 01 08 08 09 01 08 0A 0A 00 08 01 04 00
// 9F 02 82 02 03 01 02 03 04 05
int main(void) {
    char tlv[200] = {'\0'};
    printf("Enter TLV: ");
    // Get the tlv
    fgets(tlv, 200, stdin);
    // check if tlv is empty and if not, remove the \n
    if (tlv[0] == '\n') {
        printf("Empty TLV\n");
        return 1;
    }
    int tlv_length = strlen(tlv);
    if (tlv_length > 0 && (tlv[tlv_length-1] == '\n')) {
        tlv[tlv_length-1] = '\0';
    }
    if (clean_string(tlv) == false) {
        printf("Error: Invalid TLV\n");
        return 1;
    }
    parse_tlv(tlv);
    return 0;
}