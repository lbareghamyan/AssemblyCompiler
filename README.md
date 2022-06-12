# AssemblyCompiler
A compiler that generates machine code for a simple assembly language.  
## Table of contents
* [Registers](#registers)
* [Instructions](#instructions)

## Registers
Assmebly language for this complier supports 7 general purpose register:  
**R0, R1, ....., R7**,  
also input and output registers:  
**IN, OUT**
	
## Instructions
Syntax of Assembly Language Statements:  
> opcode      operand1 operand2 destination 
   
> conditional operand1 operand2 jump_destination
### Opcode
 - **ADD** -  adds op1 and op2 values and writes the result in dest
 - **SUB** -  substracts op2 from op1, writes the result in dest
 - **AND** -  does AND on every bit of op1 and op2, writes the result in dest
 - **OR** -   does OR on every bit of op1 and op2, writes the result in dest
 - **NOT** -  inverts all bits of op1 and writes the result in dest (ignores op2)
 - **XOR** -  does XOR on every bit of op1 and op2, writes the result in dest  
  
  
 - **JE** - Jump if equal, does a goto somewhere if the two operand values are equal
 - **JNE** - Jump if not equal, does a goto somewhere if the two operand values are not equal
 - **JL** - Jump if less, does a goto somewhere if the op1 value is less than op2 value
 - **JLE** - Jump if less or equal
 - **JGR** - Jump if greater
 - **JGRE** - Jump if greater or equal
