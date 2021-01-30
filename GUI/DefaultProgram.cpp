#include "DefaultProgram.hpp"

std::string CodeString = {
	"; This is an example program that this assembly language emulator can execute.\n"
	"; It will simply count to 200, then exit.\n"
	"\n"
	"MOV R0, #0                    ; Clear R0\n"
	"\n"
	"loop:\n"
		"\tADD R0, R0, #1          ; Increment R0\n"
		"\tCMP R0, #200            ; Test R0 against the value 200\n"
		"\tOUT R0                  ; Print R0\n"
		"\tBNE loop                ; Loop if not equal\n"
	"\n"
	"HALT"
};