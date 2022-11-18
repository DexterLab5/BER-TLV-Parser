#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"

char* hexadecimal_to_binary(char* hexadecimal) {
    char* binaryNum = malloc(sizeof(char)*32);
    strcpy(binaryNum, "");
    // For every hexadecimal digit or letter, concatenate the corresponding bits
    for (unsigned i = 0; hexadecimal[i] != '\0'; i++) {
        switch (hexadecimal[i]) {
            case '0':
                strcat(binaryNum, "0000");
                break;
            case '1':
                strcat(binaryNum, "0001");
                break;
            case '2':
                strcat(binaryNum, "0010");
                break;
            case '3':
                strcat(binaryNum, "0011");
                break;
            case '4':
                strcat(binaryNum, "0100");
                break;
            case '5':
                strcat(binaryNum, "0101");
                break;
            case '6':
                strcat(binaryNum, "0110");
                break;
            case '7':
                strcat(binaryNum, "0111");
                break;
            case '8':
                strcat(binaryNum, "1000");
                break;
            case '9':
                strcat(binaryNum, "1001");
                break;
            case 'A':
                strcat(binaryNum, "1010");
                break;
            case 'B':
                strcat(binaryNum, "1011");
                break;
            case 'C':
                strcat(binaryNum, "1100");
                break;
            case 'D':
                strcat(binaryNum, "1101");
                break;
            case 'E':
                strcat(binaryNum, "1110");
                break;
            case 'F':
                strcat(binaryNum, "1111");
                break;
        }
    }
    int len = strlen(binaryNum);
    binaryNum[len] = '\0';
    return binaryNum;
}

int hexadecimal_to_decimal(char* hexadecimal) {
    int decimal = 0;
    int base = 1, length = strlen(hexadecimal);
    // Get the first digit and mulitply it by base, base gets multiplied by 16 on every loop
    for (int i = --length; i >= 0; i--) {
        if (hexadecimal[i] >= '0' && hexadecimal[i] <= '9') {
            decimal += (hexadecimal[i] - 48) * base;
            base *= 16;
        }
        else if (hexadecimal[i] >= 'A' && hexadecimal[i] <= 'F') {
            decimal += (hexadecimal[i] - 55) * base;
            base *= 16;
        }
    }
    return decimal;
}

int binary_to_decimal(char* binaryNumber) {
    int res = 0, power = 0;
    // Iterate through the binary number, convert it to int with ascii and multiply it by a power of 2. 
    // Power(variable) is incermented on every iteration.
    for (int i = 7; i >= 0; i--) {
        res += (binaryNumber[i] - 48) * pow(2,power);
        power++;
    }
    return res;
}