/*
 * esc.c
 *
 *  Created on: Nov 20, 2021
 *      Author: dombran
 */

// extension of the standard C
// расширение стандарта  си

#include "esc.h"

#define MALLOC_BUF_SIZE		272//4096

uint8_t	mall_bf[MALLOC_BUF_SIZE] = {0};
uint32_t dinamic_cntr;

DINAMIC_PARS dinamicPars;

int32_t new_vector(uint32_t size_type);
int32_t push_back_vec(DDESC dd, uint8_t *data);

uint32_t g_next_dm(uint32_t num_start);
uint32_t g_size_dm(DDESC dd);
uint32_t g_size_d_dm(DDESC dd);
uint32_t g_size_t_dm(DDESC dd);


void s_asize_dm(DDESC dd, uint32_t size);
void s_dsize_dm(DDESC dd, uint32_t size);
void s_tsize_dm(DDESC dd, uint32_t size);

int32_t get_num_el(DDESC dd, uint32_t num, uint8_t *data);
int32_t get_first_el(DDESC dd, uint8_t *data);
int32_t del_first_el(DDESC dd);
int32_t del_vec(DDESC dd);
int32_t del_all_el(DDESC dd);

void esc_init(){

	dinamicPars.vector = &new_vector; // зарегистрироваеть новый вектор
	dinamicPars.push_back = &push_back_vec; // добавить элемент в конце

	dinamicPars.get_num_el = &get_num_el;// получить элемент вектора под номером
	dinamicPars.get_first_el = &get_first_el;// получить первый элемент вектора
	dinamicPars.del_first_el = &del_first_el;//удалить первый элемент вектора
	dinamicPars.del_vec = &del_vec;// удалить весь вектор
	dinamicPars.del_all_el = &del_all_el;// удалить все данные вектора

	dinamic_cntr = 0;

//	dinamicPars.internal.g_next_dm = &g_next_dm;
//	dinamicPars.internal.g_size_dm = &g_size_dm;
//	dinamicPars.internal.g_size_d_dm = &g_size_d_dm;
//	dinamicPars.internal.g_size_t_dm = &g_size_t_dm;
//
//	dinamicPars.internal.s_asize_dm = &s_asize_dm;
//	dinamicPars.internal.s_dsize_dm = &s_dsize_dm;
//	dinamicPars.internal.s_tsize_dm = &s_tsize_dm;

}

int32_t find_desc(DDESC dd) {	// поиск дескриптора в буфере
	VECTYPE *vt;

	for( uint32_t i=0; i < MALLOC_BUF_SIZE; i = g_next_dm(i) ){
		vt = (VECTYPE*)&mall_bf[i];
		if(vt->a_size == 0)
			return DYNAMIC_BUF_NOTFOUND;
		if(vt->c_desc == dd)
			return i;
	}

	return DYNAMIC_BUF_NOTFOUND;
}

// num_start - адрес в динамическом массиве на начало вектора
uint32_t g_next_dm(uint32_t num_start){ // возвращает адрес на следующий вектор
	VECTYPE *vt = 0;
	vt = (VECTYPE*)&mall_bf[num_start];
	return num_start + vt->a_size;
}
uint32_t g_size_dm(DDESC dd){ // возвращает колво данных всего вектора в байтах
	return ((VECTYPE*)&mall_bf[find_desc(dd)])->a_size;
}
uint32_t g_size_d_dm(DDESC dd){ // возвращает колво данных в векторе
	return ((VECTYPE*)&mall_bf[find_desc(dd)])->d_size;
}
uint32_t g_size_t_dm(DDESC dd){ // возвращает размер типа данных
	return ((VECTYPE*)&mall_bf[find_desc(dd)])->t_size;
}
uint32_t g_size_f_link(DDESC dd){	// получить адрес первого элемента
	return find_desc(dd) + sizeof(VECTYPE);
}


void s_asize_dm(DDESC dd, uint32_t size){
	((VECTYPE*)&mall_bf[find_desc(dd)])->a_size = size;
}
void s_dsize_dm(DDESC dd, uint32_t size){
	((VECTYPE*)&mall_bf[find_desc(dd)])->d_size = size;
}
void s_tsize_dm(DDESC dd, uint32_t size){
	((VECTYPE*)&mall_bf[find_desc(dd)])->t_size = size;
}

uint32_t get_all_num_elements(DDESC dd){ // возвращает колво полей вектора
	return ( g_size_dm(dd) - sizeof(VECTYPE) ) / g_size_t_dm(dd);
}


uint32_t free_size_vec(DDESC dd) { // возвращает колво свободных элементов в векторе
	return (get_all_num_elements(dd) - g_size_d_dm(dd));
}
uint32_t free_addr_vec(DDESC dd){ // возвращает адрес резервного элемента вектора
	return find_desc(dd) + sizeof(VECTYPE)+g_size_d_dm(dd)*g_size_t_dm(dd);
}
uint32_t byte_data_vec(DDESC dd){ // колво данных вектора в байтах
	return g_size_dm(dd)*g_size_t_dm(dd);
}
int32_t get_new_zero_addr(){ // возвращает адрес на пустое место в динамической памяти
	VECTYPE *az = 0;

	for(uint32_t i=0; i<(uint32_t)MALLOC_BUF_SIZE; i = g_next_dm(i)){
		az = (VECTYPE*)&mall_bf[i];
		if(az->c_desc == 0){// фрагмент найден
			if(az->a_size == 0){// все поля не могут быть равну нулю, дошли до конца
				return i;
			}
		}
	}

	return DYNAMIC_BUF_OVERFLOW;
}

// сложно и не доконца
void difragment(){ // дифрагментация динамического вектора
	uint32_t n = 0;
	VECTYPE *acc_zero = 0;


	for(uint32_t i = 0; i < (uint32_t)MALLOC_BUF_SIZE; i = g_next_dm(i)){
		acc_zero = (VECTYPE*)&mall_bf[i];
		if( acc_zero->c_desc == 0 ){ // фрагмент найден
			if(acc_zero->a_size == 0) // все поля не могут быть равну нулю, дошли до конца
				break;
			// не фрагментированное поле
			n = g_next_dm(i);
			for(uint32_t d = i; d < MALLOC_BUF_SIZE; d++){
				mall_bf[d] = mall_bf[n++];
			}
		}
	}
}

int32_t transf_forvard(DDESC dd){ // перенос вектора вперед
	uint32_t ds;
	VECTYPE *acc_new;
	uint32_t new_size = (g_size_dm(dd) + (5 * g_size_t_dm(dd)));
	ds = find_desc(dd);

	for(uint32_t i = g_next_dm(dd); i < MALLOC_BUF_SIZE; i = g_next_dm(i)){
		acc_new = (VECTYPE*)&mall_bf[i];
		if( ( acc_new->c_desc == 0 ) && ( acc_new->a_size == 0) ){
			if( ( MALLOC_BUF_SIZE - i) > new_size ){ // место для переноса найдено

				acc_new->a_size = new_size;
				acc_new->c_desc = dd;
				acc_new->d_size = g_size_d_dm(dd);
				acc_new->t_size = g_size_t_dm(dd);
				for(uint32_t j=0; j<g_size_dm(dd); j++){
					  mall_bf[i+sizeof(VECTYPE)+j] = mall_bf[ds+sizeof(VECTYPE)];
				}

				((VECTYPE*)&mall_bf[ds])->c_desc = 0;

				return OK;
			}else
				return DYNAMIC_BUF_OVERFLOW;

		}
	}

	return DYNAMIC_BUF_OVERFLOW;
}

// возвращает дескриптор динамического вектора
// если отрицательный, то ошибка
int32_t new_vector(uint32_t size_type){ //иництализация нового вектора
	int32_t gn_zero = 0;
	uint32_t num = 0;
	VECTYPE *vc = 0;

	gn_zero = get_new_zero_addr();
	if(gn_zero < 0)
		return gn_zero;

	num =  MALLOC_BUF_SIZE - gn_zero;

	if( num > (sizeof(VECTYPE)+ (size_type * 5) ) ){ // 5 запасных пустых полей
		dinamic_cntr++; // инкрементируем дескриптор

		vc = (VECTYPE*)&mall_bf[gn_zero];
		vc->c_desc = dinamic_cntr;
		vc->d_size = 0;
		vc->t_size = size_type;
		vc->a_size = sizeof(VECTYPE) + (size_type * 5);

		return dinamic_cntr;
	}else{
		difragment(); // места нет, запускам дифрагментацию

		gn_zero = get_new_zero_addr();
		num =  MALLOC_BUF_SIZE - gn_zero;
		if( num > (sizeof(VECTYPE)+ (size_type * 5) ) ){
			dinamic_cntr++; // инкрементируем дескриптор

				vc = (VECTYPE*)&mall_bf[gn_zero];
				vc->c_desc = dinamic_cntr;
				vc->d_size = 0;
				vc->t_size = size_type;
				vc->a_size = sizeof(VECTYPE) + (size_type * 5);

				return OK;
		}else
			return DYNAMIC_BUF_OVERFLOW;
	}
}



int32_t push_back_vec(DDESC dd, uint8_t *data){

	int32_t fr_num = find_desc(dd); // точка начала вектора

	if(fr_num < 0)
		return DYNAMIC_BUF_NOTFOUND;


	uint32_t free_size = free_size_vec(fr_num);
	uint32_t dacc;

	dacc = free_addr_vec(dd);
	if(free_size > 1){ // проверяем пустое место
		for(uint32_t i = 0; i < g_size_t_dm(dd); i++){
			mall_bf[dacc+i] = data[i];
		}
		return OK;
	}

	difragment(); // места нет, запускам дифрагментацию

	if(transf_forvard(dd) < 0)
		return DYNAMIC_BUF_OVERFLOW;

	dacc = free_addr_vec(dd);
	if(free_size > 1){ // проверяем пустое место
		for(uint32_t i = 0; i < g_size_t_dm(dd); i++){
			mall_bf[dacc+i] = data[i];
		}
		return OK;
	}else
		return DYNAMIC_BUF_OVERFLOW;
}

int32_t get_num_el(DDESC dd, uint32_t num, uint8_t *data){ // получить элемент вектора под номером
	uint32_t num_f = g_size_f_link(dd); // номер первого элемента
	uint32_t num_d = g_size_d_dm(dd);
	uint32_t num_t = g_size_t_dm(dd);

	if(num_d < num)	// нет такого элемента
		return DYNAMIC_BUF_NOELEMENT;

	data = &mall_bf[ num_f + (num_t*num) ];
	return OK;
}
int32_t get_first_el(DDESC dd, uint8_t *data){ // получить первый элемент вектора
	uint32_t num_f = g_size_f_link(dd); // номер первого элемента
	data = &mall_bf[ num_f ];
	return OK;
}
int32_t del_first_el(DDESC dd){ //удалить первый элемент вектора
	uint32_t num_d = (g_size_d_dm(dd)* g_size_t_dm(dd));
	uint32_t type_s = g_size_t_dm(dd);
	uint8_t *pd;

	pd = &mall_bf[g_size_f_link(dd)];
	for(uint32_t i=0; i<(num_d - type_s); i++)
		pd[i] = pd[i+type_s];

	( (VECTYPE*)&mall_bf[ find_desc(dd) ] )->d_size -= 1;

	return OK;
}
int32_t del_vec(DDESC dd){ // удалить весь вектор
	( (VECTYPE*)&mall_bf[ find_desc(dd) ] )->c_desc = 0;
	return OK;
}
int32_t del_all_el(DDESC dd){ // удалить все данные вектора
	( (VECTYPE*)&mall_bf[ find_desc(dd) ] )->d_size = 0;
	return 0;
}
