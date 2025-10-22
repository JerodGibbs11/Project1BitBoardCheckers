#ifndef BITMANIPLUATION_H
#define BITMANIPULATION_H


#define BIT_WIDTH 64
#define EIGHT_BITS 0b11111111

//check if we are within 64 bit range
bool valid_pos(int position);
//sets bit to 1
void SetBit(uint64_t *value, int position);
//Set bit to 0
void ClearBit(uint64_t *value, int position);
//Flip bit
void ToggleBit(uint64_t *value, int position);
//Get bit value
int GetBit(uint64_t value, int position);
//Counting and finding
int CountBits(uint64_t value);
//bit shifting function
uint64_t ShiftBit(uint64_t value, int position, bool bLeftShift);
//prints the binary of the int value passed
void PrintBinary(uint64_t value);
void PrintHex(uint64_t value);
//gets a subset of bits
uint8_t Get8BitsAtRowNumber(uint64_t value, int row);


#endif