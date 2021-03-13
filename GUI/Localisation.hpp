#pragma once

#ifndef LOCALISATION_HPP
#define LOCALISATION_HPP

#include <string>

#define c (char*)
#define _c (const char*)

struct Localisation_t
{
	char* HaltProcessor = c"Processor halted.";
	char* CurrentExecLine = c"Currently executing line.";
	char* CodeEntry = c"Code Entry";
	char* Stop = c"Stop";
	char* Run = c"Run";
	char* Play = c"Play";
	char* Pause = c"Pause";
	char* Reset = c"Reset";
	char* Assemble = c"Assemble";
	char* LastError = c"Last Error: %s";
	char* Delay = c"Delay";
	char* Eval = c"Eval: %s";
	char* Copyright = c"Copyright (c) Luke Goule    Released under MIT License";

	struct Error_t
	{
		char* WindowInit = c"FATAL! Failed to initialise window!";
		char* DXInit = c"FATAL! Failed to initialise DirectX!";
		char* Fatal = c"FATAL ERROR";
		char* Fonts = c"FATAL! Failed to load fonts!";
	} Error;

};
extern Localisation_t Localisation;

#endif