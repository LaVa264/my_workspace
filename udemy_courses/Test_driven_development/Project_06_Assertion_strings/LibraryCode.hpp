#pragma once 
#include <cctype>
#include <string.h>

void toUpper( char * inputString){
	for(unsigned i = 0; i < strlen(inputString); i++){
		inputString[i] = toupper(inputString[i]);
	}
}



