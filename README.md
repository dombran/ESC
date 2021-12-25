# ESC
 extension of the standard C

struct {

	int32_t (*vector)(uint32_t size_type);// зарегистрироваеть новый вектор
  // возвращает дескриптор вектора.
  
	int32_t (*push_back)(DDESC dd, uint8_t *data);// добавить элемент в конце
  // обращение к  вектору по дескриптору

	int32_t (*get_num_el)(DDESC dd, uint32_t num, uint8_t *data);// получить элемент вектора под номером
  // num - номер элемента вектора , data - указатель на начало данных элемента
  
	int32_t (*get_first_el)(DDESC dd, uint8_t *data);// получить первый элемент вектора
	int32_t (*del_first_el)(DDESC dd);//удалить первый элемент вектора
	int32_t (*del_vec)(DDESC dd);// удалить весь вектор
	int32_t (*del_all_el)(DDESC dd);// удалить все данные вектора

} DINAMIC_PARS;
