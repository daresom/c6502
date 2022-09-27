#include "common.h"
#include "c6502.h"

CPU cpu;

#define setZ(register) (cpu.Z = register == 0)
#define setN(register) (cpu.N = (register & 0b1000000) > 0)


void resetCPU(){
	cpu.PC = 0x00;
	cpu.SP = 0xff;
	
	for(int i = 0; i < 65536; i++){
		cpu.memory[i] = 0;
	}

	cpu.curInst = 0;

	cpu.C = cpu.Z = cpu.I = cpu.D = cpu.B = cpu.V = cpu.N = 0;

	cpu.A = cpu.X = cpu.Y = 0;
}

void printCPU(){
	printf("A:  %u\nX:  %u\nY:  %u\n", cpu.A, cpu.X, cpu.Y);
	printf("C:%u  Z:%u  I:%u  D:%u  B:%u  V:%u  N:%u\n\n", cpu.C, cpu.Z, cpu.I, cpu.D, cpu.B, cpu.V, cpu.N);
}

void writeByte(uint8_t data){ //Intended for testing instructions by hardcoding them into the program
	cpu.memory[cpu.curInst] = data;
	cpu.curInst++;
}

uint8_t fetchByte(int* cycles){ //Read byte and increment PC
	uint8_t byte = cpu.memory[cpu.PC];
	cpu.PC++;
	*cycles -= 1;
	
	return byte;
}

uint8_t requestByte(int* cycles, uint16_t address){ //Get value from byte of memory
	uint8_t value = cpu.memory[address];
	*cycles -= 1;

	return value;
}

void storeRegister(int* cycles, uint16_t address, uint8_t reg){ //Store register in address
	cpu.memory[address] = reg;
	*cycles -= 1;
}

void executeInstruction(int cycles){
	while(cycles > 0){
		uint8_t instruction = fetchByte(&cycles);

		switch(instruction){
			case 0xa9:{ //LDA immediate
				uint8_t value = fetchByte(&cycles);
				cpu.A = value;
				setZ(cpu.A);
				setN(cpu.A); //Check if 7th bit set
			}break;
			case 0xa5:{ //LDA Zero Page
				uint16_t address = fetchByte(&cycles);
				cpu.A = requestByte(&cycles, address);
				setZ(cpu.A);
				setN(cpu.A); 
			 }break;
			case 0xb5:{ //LDA Zero Page,X
				uint8_t address = fetchByte(&cycles);
				address += cpu.X;
				cpu.A = requestByte(&cycles, address);
				cycles--;
				setZ(cpu.A);
				setN(cpu.A);
			 }break;
			case 0xad:{ //LDA Absolute
				uint8_t lsb = fetchByte(&cycles);
				uint8_t msb = fetchByte(&cycles);
				uint16_t address = lsb + (msb << 8);
				cpu.A = requestByte(&cycles, address);
				setZ(cpu.A);
				setN(cpu.A);
			}break;
			case 0xbd:{ //LDA Absolute,X
				uint8_t lsb = fetchByte(&cycles);
				uint8_t msb = fetchByte(&cycles);	
				uint16_t address = lsb + (msb << 8);

				lsb = (lsb + cpu.X) & 0xff;

				if(lsb < cpu.X){
					requestByte(&cycles, address);
					msb = (msb + 1) & 0xff;
				}

				address = lsb + (msb << 8);

				cpu.A = requestByte(&cycles, address);
				setZ(cpu.A);
				setN(cpu.A);
			}break;
			case 0xb9:{ //LDA Absolute,Y
				uint8_t lsb = fetchByte(&cycles);
				uint8_t msb = fetchByte(&cycles);	
				uint16_t address = lsb + (msb << 8);

				lsb = (lsb + cpu.Y) & 0xff;

				if(lsb < cpu.Y){
					requestByte(&cycles, address);
					msb = (msb + 1) & 0xff;
				}

				address = lsb + (msb << 8);

				cpu.A = requestByte(&cycles, address);
				setZ(cpu.A);
				setN(cpu.A);
			}break;
			case 0xa1:{ //LDA Indexed Indirect
				uint16_t address = fetchByte(&cycles);
				address = (address + cpu.X) & 0xff;
				requestByte(&cycles, address);

				uint8_t lsb = requestByte(&cycles, address);
				uint8_t msb = requestByte(&cycles, address+1);
				uint16_t targetAddress = lsb + (msb << 8);
				
				cpu.A = requestByte(&cycles, targetAddress);

				setZ(cpu.A);
				setN(cpu.A);			
			}break;
			case 0xb1:{ //LDA Indirect Indexed
				uint16_t address = fetchByte(&cycles);
				uint8_t lsb = requestByte(&cycles, address);
				uint8_t msb = requestByte(&cycles, address+1);
				uint16_t targetAddress = lsb + (msb << 8);
				
				lsb = (lsb + cpu.Y) & 0xff;
				
				if(lsb < cpu.Y){
					requestByte(&cycles, targetAddress);
					msb = (msb + 1) & 0xff;
				}
				targetAddress = lsb + (msb << 8);

				cpu.A = requestByte(&cycles, targetAddress);
				
				setZ(cpu.A);
				setN(cpu.A);
			 }break;

			case 0xa0:{ //LDY immediate
				uint8_t value = fetchByte(&cycles);
				cpu.Y = value;
				setZ(cpu.Y);
				setN(cpu.Y); //Check if 7th bit set
			}break;
			case 0xa4:{ //LDY Zero Page
				uint16_t address = fetchByte(&cycles);
				cpu.Y = requestByte(&cycles, address);
				setZ(cpu.Y);
				setN(cpu.Y); 
			 }break;
			case 0xb4:{ //LDY Zero Page,X
				uint8_t address = fetchByte(&cycles);
				address += cpu.X;
				cpu.Y = requestByte(&cycles, address);
				cycles--;
				setZ(cpu.Y);
				setN(cpu.Y);
			 }break;
			case 0xac:{ //LDY Absolute
				uint8_t lsb = fetchByte(&cycles);
				uint8_t msb = fetchByte(&cycles);
				uint16_t address = lsb + (msb << 8);
				cpu.Y = requestByte(&cycles, address);
				setZ(cpu.Y);
				setN(cpu.Y);
			}break;
			case 0xbc:{ //LDY Absolute,X
				uint8_t lsb = fetchByte(&cycles);
				uint8_t msb = fetchByte(&cycles);	
				uint16_t address = lsb + (msb << 8);

				lsb = (lsb + cpu.X) & 0xff;

				if(lsb < cpu.X){
					requestByte(&cycles, address);
					msb = (msb + 1) & 0xff;
				}

				address = lsb + (msb << 8);

				cpu.Y = requestByte(&cycles, address);
				setZ(cpu.Y);
				setN(cpu.Y);
			}break;

			case 0xa2:{ //LDX immediate
				uint8_t value = fetchByte(&cycles);
				cpu.X = value;
				setZ(cpu.X);
				setN(cpu.X); //Check if 7th bit set
			}break;
			case 0xa6:{ //LDX Zero Page
				uint16_t address = fetchByte(&cycles);
				cpu.X = requestByte(&cycles, address);
				setZ(cpu.X);
				setN(cpu.X); 
			 }break;
			case 0xb6:{ //LDX Zero Page,Y
				uint8_t address = fetchByte(&cycles);
				address += cpu.Y;
				cpu.X = requestByte(&cycles, address);
				cycles--;
				setZ(cpu.X);
				setN(cpu.X);
			 }break;
			case 0xae:{ //LDX Absolute
				uint8_t lsb = fetchByte(&cycles);
				uint8_t msb = fetchByte(&cycles);
				uint16_t address = lsb + (msb << 8);
				cpu.X = requestByte(&cycles, address);
				setZ(cpu.X);
				setN(cpu.X);
			}break;
			case 0xbe:{ //LDX Absolute,Y
				uint8_t lsb = fetchByte(&cycles);
				uint8_t msb = fetchByte(&cycles);	
				uint16_t address = lsb + (msb << 8);

				lsb = (lsb + cpu.Y) & 0xff;

				if(lsb < cpu.Y){
					requestByte(&cycles, address);
					msb = (msb + 1) & 0xff;
				}

				address = lsb + (msb << 8);

				cpu.X = requestByte(&cycles, address);
				setZ(cpu.X);
				setN(cpu.X);
			}break;

			case 0xe8: //INX
				cycles--;
				cpu.X++;
				setZ(cpu.X);
				setN(cpu.X);
			 break;
			case 0xc8: //INY
			 	cycles--;
			 	cpu.Y++;
				setZ(cpu.Y);
				setN(cpu.Y);
			 break;

			case 0xca: //DEX
			 	cycles--;
			 	cpu.X--;
				setZ(cpu.X);
				setN(cpu.X);
			break;
			case 0x88: //DEY
				cycles--;
			 	cpu.Y--;
				setZ(cpu.Y);
				setN(cpu.Y);
			break;

			case 0x85:{ //STA Zero Page
				uint16_t address = fetchByte(&cycles);
				storeRegister(&cycles, address, cpu.A);
			}break;
			case 0x95:{ //STA Zero Page,X
				uint8_t address = fetchByte(&cycles);
				address += cpu.X;
				storeRegister(&cycles, address, cpu.A);
				cycles--;
			}break;
			case 0x8d:{ //STA Absolute
				uint8_t lsb = fetchByte(&cycles);
				uint8_t msb = fetchByte(&cycles);
				uint16_t address = lsb + (msb << 8);
				storeRegister(&cycles, address, cpu.A);
			}break;
			case 0x9d:{ //STA Absolute,X
				uint8_t lsb = fetchByte(&cycles);
				uint8_t msb = fetchByte(&cycles);
				uint16_t address = lsb + (msb << 8);

				
			}break;
				  
		}
	}
}
