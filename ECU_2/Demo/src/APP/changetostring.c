#include <stdio.h>
#include <stdlib.h>

// Function to get the length of an integer
int getLength(int number) {
    int length = 0;
    while (number != 0) {
        length++;
        number /= 10;
    }
    return length;
}

char years[5];

void DecimalToString(char *ptrTostring,u8 number_length,u16 number)
{
    for (int i = number_length-1; i >= 0; i--) 
    {
        ptrTostring[i] = (number % 10) + '0';  //2024 //4
        number /= 10;
    }

    ptrTostring[number_length]='\0';
}

 LCD_WriteStringAsync(years,5);
// Function to convert a decimal number to a string
char* decimalToString(int number) {
    int length = getLength(number);

    // Allocate memory for the string
    char* str = (char*)malloc((length + 1) * sizeof(char));

    // Handle the case of negative numbers
    if (number < 0) {
        str[0] = '-';
        number = -number;
    }
    decimaltostring(string,sizeofnumber,number)
    // Convert each digit to character and store it in the string
    for (int i = length - 1; i >= 0; i--) {
        str[i] = (number % 10) + '0';
        number /= 10;
    }

    // Null-terminate the string
    str[length] = '\0';

    return str;
}

int main() {
    int number = 12345;
    char* str = decimalToString(number);
    printf("Decimal Number: %d, String: %s\n", number, str);
    free(str); // Free the allocated memory
    return 0;
}