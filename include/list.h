#ifndef LIST_H
	#define LIST_H

	const ssize_t LIST_INCREASE_CONSTANT = 2; 
	const ssize_t START_SIZE = 5;

	const ssize_t UNDEFINED_LIST_NUMBER = -1;

	typedef unsigned int err_t;
	typedef ssize_t elem_t;

	#define NUMBER_PRINTF_SPEC     "%zd"
	#define LIST_ELEM_PRINTF_SPEC  "%zd"

	struct List_elem
	{
		struct List_elem* prev_elem_ptr;
		struct List_elem* next_elem_ptr;

		elem_t elem;
	};

	struct List
	{
		ssize_t list_size;

		struct List_elem* zero_elem_ptr;
	};

	#define LIST_TAIL (list_ptr->zero_elem_ptr)->next_elem_ptr
	#define LIST_HEAD (list_ptr->zero_elem_ptr)->prev_elem_ptr

	const ssize_t NEXT_PREV_POISON_VALUE = -1;

	enum ListErrors
	{
		NO_ERROR = 0,
		INVALID_LIST_SIZE          		= 1 << 0,
		NULL_REFERENCE_TO_NEIGHBOR 		= 1 << 2,
		INVALID_PREV_OF_NEXT_OF_ELEM 	= 1 << 3,
		VERY_BIG_LIST 					= 1 << 4,
		INVALID_ITERATOR 				= 1 << 5
	};

	enum ListExeptions
	{
		UNABLE_TO_FREE_ZERO_ELEMENT = 1 << 19
	};

	#define CHECK_LIST()															\
	err_t return_code = list_verificator(list_ptr); 								\
	if (return_code != NO_ERROR)													\
	{																				\
		printf("invalid list\n");													\
		printf("file: <%s> func: <%s> line <%d>\n", __FILE__, __func__, __LINE__);  \
		print_list_error(return_code);												\
	}																				

	err_t list_init(struct List* list_ptr);
	err_t list_dtor(struct List* list_ptr);
	err_t list_verificator(const struct List* const list_ptr);
	err_t print_list_error(const err_t return_code);
	err_t list_add_in_tail(struct List* const list_ptr, const elem_t elem);
	err_t list_add_in_head(struct List* const list_ptr, const elem_t elem);
	err_t print_list_elem(const struct List_elem* const list_elem_ptr);
	err_t list_dump(const struct List* const list_ptr);

	err_t list_add_after_elem_by_his_ptr(struct List* const list_ptr, struct List_elem* const before_elem, const elem_t elem);
	err_t list_add_before_elem_by_his_ptr(struct List* const list_ptr, struct List_elem* const after_elem, const elem_t elem);

	err_t list_add_after_elem_by_iterator(struct List* const list_ptr, const ssize_t iterator, const elem_t elem);
	err_t list_add_before_elem_by_iterator(struct List* const list_ptr, const ssize_t iterator, const elem_t elem);

	err_t free_cell_by_iterator(struct List* const list_ptr, const ssize_t iterator);
	err_t free_cell_by_his_ptr(struct List* const list_ptr, struct List_elem*  elem_to_free);

#endif /* LIST_H */
