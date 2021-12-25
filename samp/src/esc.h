
#include <stdint.h>




#define OK 						0
#define DYNAMIC_BUF_OVERFLOW 	-1
#define DYNAMIC_BUF_NOTFOUND 	-2
#define DYNAMIC_BUF_NOELEMENT 	-3

#define TSIZE_NEXT		4
#define TSIZE_SIZE		4
#define TSIZE_SIZE_T	4

#define HEADER_ELEMENT_SIZE 4

typedef enum{
	DM_VECT = 0,
	DM_MAP,
	DM_LIST
}DM_DATA;

typedef int32_t DDESC;	// dinamic descriptor
//	//							   'vect'    'map'		 'list'
//static uint32_t dnm_list[3] = { 0x76656374, 0x006D6170, 0x6C697374};

// базовая структура для размещения в динамическом буффере
// в чистой форме используется для размещения вектора
// для метки обнуленого вектора достаточно занулить c_type
typedef struct{ // header
	DDESC	c_desc;	// дескриптор т.е. идентификатор который хранит пользователь
					// обнуление дескриптора приводит к удалению вектора
	uint32_t	a_size;	// общий занимаемый размер (в байтах) заголовок вместе с данными
	uint32_t	t_size; // размер поля
	uint32_t	d_size; // колво полей данных
} VECTYPE;

typedef struct {
//	struct{
//		uint64_t (*g_next_dm)(VECT_TYPE *vec);// возвращает адрес на следующий вектор
//		uint32_t (*g_size_dm)(VECT_TYPE *vec); // возвращает колво данных всего вектора в байтах
//		uint32_t (*g_size_d_dm)(VECT_TYPE *vec); // возвращает колво данных в векторе
//		uint32_t (*g_size_t_dm)(VECT_TYPE *vec); // возвращает размер типа данных
//
//		void (*s_asize_dm)(VECT_TYPE *v_this, uint32_t size);
//		void (*s_dsize_dm)(VECT_TYPE *v_this, uint32_t size);
//		void (*s_tsize_dm)(VECT_TYPE *v_this, uint32_t size);
//	}internal;

	int32_t (*vector)(uint32_t size_type);
	int32_t (*push_back)(DDESC dd, uint8_t *data);

	int32_t (*get_num_el)(DDESC dd, uint32_t num, uint8_t *data);
	int32_t (*get_first_el)(DDESC dd, uint8_t *data);
	int32_t (*del_first_el)(DDESC dd);
	int32_t (*del_vec)(DDESC dd);
	int32_t (*del_all_el)(DDESC dd);

} DINAMIC_PARS;

extern DINAMIC_PARS dinamicPars;
extern void esc_init();
