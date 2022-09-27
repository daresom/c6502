#include "common.h"
#include "c6502.h"

int main(){
	//Test cases from demonstration
	
	resetCPU();
	printCPU(); //Print cpu's properties before any changes
	
	//Increment and decrement X
	printf("inc and dec x\n");
	writeByte(0xe8);
	executeInstruction(2);
	printCPU();
	writeByte(0xca);
	executeInstruction(2);
	printCPU();
	
	//Increment and decrement Y
	printf("inc and dec y\n");
	writeByte(0xc8);
	executeInstruction(2);
	printCPU();
	writeByte(0x88);
	executeInstruction(2);
	printCPU();
	
	//Load A register, Immediate mode with the number 5
	printf("LDA immediate\n");
	writeByte(0xa9);
	writeByte(0x05);
	executeInstruction(2);
	printCPU();
	
	//Load A register, Zero page mode from address 0001, the value of A will be ca (202)
	printf("LDA zero page\n");
	writeByte(0xa5);
	writeByte(0x01);
	executeInstruction(3);
	printCPU();
	
	//Load A register, Absolute mode from address 0501, the value of A will be 0
	printf("LDA absolute\n");
	writeByte(0xad);
	writeByte(0x01);
	writeByte(0x05);
	executeInstruction(4);
	printCPU();
	

	return 0;
}
