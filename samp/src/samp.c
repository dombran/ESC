/*
 ============================================================================
 Name        : samp.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include "esc.h"

typedef struct{
	uint8_t a;
	uint32_t b;
	uint16_t c;
}D_SVT;


int main(void) {

	esc_init();	// инициализация динамического буфера

	DDESC vect_desc_1 = 0;	// дескриптор вектора
	DDESC vect_desc_2 = 0;
	DDESC vect_desc_3 = 0;
	DDESC vect_desc_4 = 0;

	vect_desc_1 = dinamicPars.vector(sizeof(D_SVT));
	vect_desc_2 = dinamicPars.vector(sizeof(D_SVT));
	vect_desc_3 = dinamicPars.vector(sizeof(D_SVT));
	vect_desc_4 = dinamicPars.vector(sizeof(D_SVT));

	dinamicPars.del_vec(vect_desc_2);
	vect_desc_4 = dinamicPars.vector(sizeof(D_SVT));


	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
