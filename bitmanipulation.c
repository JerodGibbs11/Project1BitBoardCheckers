#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "bitmanipulation.h"

#define BIT_WIDTH 64

//
bool valid_pos(int position)
{
	if (position >= 0 && position < BIT_WIDTH)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//sets bit to 1
void SetBit(uint64_t *value, int position)
{
	if(value == NULL)
	{
		return;
	}
	
	if(valid_pos(position))
	{
		*value = *value | (1ull << position);
	}
}

//Set bit to 0
void ClearBit(uint64_t *value, int position)
{
	if(value == NULL)
	{
		return;
	}
	
	if(valid_pos(position))
	{
		*value = *value & ~(1ull << position);
	}
}

//Flip bit
void ToggleBit(uint64_t *value, int position)
{
	if(value == NULL)
	{
		return;
	}
	
	if(valid_pos(position))
	{
		*value = *value ^ (1ull << position);
	}
}

//Get bit value
int GetBit(uint64_t value, int position)
{
	if(!valid_pos(position))
	{
		return 0;
	}
	
	if(((value >> position) & 1ull) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//Get bit range
uint8_t Get8BitsAtRowNumber(uint64_t value, int row)
{
	return (value >> (row * 8)) & EIGHT_BITS;
}

//Counting and finding
int CountBits(uint64_t value)
{
	int count = 0;
	
	while(value != 0)
	{
		value = value & (value - 1);
		count = count + 1;
	}
	
	return count;
}

//bit shifting function
uint64_t ShiftBit(uint64_t value, int position, bool bLeftShift)
{
	if(position < 0)
	{
		position = - position;
		if(bLeftShift)
		{
			bLeftShift = false;
		}
		else
		{
			bLeftShift = true;
		}
	}
	
	if(position >= BIT_WIDTH)
	{
		return 0u;
	}
	
	if(bLeftShift)
	{
		return value << position;
	}
	else
	{
		return value >> position;
	}
}

//prints the binary of the int value passed
void PrintBinary(uint64_t value)
{
	for (int i = BIT_WIDTH - 1; i >= 0; i--)
	{
		if(((value >> i) & 1ull) != 0)
		{
			putchar('1');
		}
		else
		{
			putchar('0');
		}
		if(i % 4 == 0 && i != 0)
		{
			putchar('_');
		}
	}
	printf("\n");
}

void PrintHex(uint64_t value)
{
	printf("0x%0*X", (int)(sizeof(uint64_t) * 2), value);
}


