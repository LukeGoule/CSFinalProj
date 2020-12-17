# Simple_AQA_ASM_Executor
AQA Assembly language program executor, written in C++ for my final A Level Project.

## How to execute
Write your program with any kind of text editor, then drag and drop the program onto the output program.
Alternatively you can directly execute the program and run "Quick Execution Mode" which allows you to enter instructions manually or enter a file name to load. (file loading like this is currently none functional, for whatever reason).

## A simple program
```
; This is an example program that this assembly language emulator can execute.
; It will simply count to 200, then exit.

MOV R0, #0	; Clear R0
	
loop:
ADD R0, R0, #1	; Increment R0
CMP R0, #200	; Test R0 against the value 200
OUT R0			; Print R0
BNE loop		; Loop if not equal
	
HALT
```
