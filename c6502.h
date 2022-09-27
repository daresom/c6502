#ifndef C6502_H
#define C6502_H
#include "common.h"

typedef struct cpu{
	uint16_t PC; //Program counter
	uint8_t SP;// Stack pointer

	uint8_t A, X, Y; //Registers

	bool C, Z, I, D, B, V, N; //Flags
							  /* C = Carry 
							  Z = Zero 
							  I = Interrupt 
							  D = Decmal 
							  B = Break  
							  V = Overflow 
							  N = negative */	
							  
	uint8_t memory[65536]; // Amount of memory that processor has (256x256)
	int curInst; //Current instruction being pointed to
}CPU;

void resetCPU();
void writeByte(uint8_t data);
uint8_t readByte();
void executeInstruction(int cycles);
void printCPU();

#endif
