#include <stdio.h>
#include <malloc.h>
#include "include/list.h"

#define CHECK_ITERATOR()


err_t list_init(struct List* list_ptr)
{
	list_ptr->zero_elem_ptr = (struct List_elem*) calloc(1 ,sizeof(struct List_elem));

	(list_ptr->zero_elem_ptr)->next_elem_ptr = list_ptr->zero_elem_ptr;

	(list_ptr->zero_elem_ptr)->prev_elem_ptr = list_ptr->zero_elem_ptr;

	list_ptr->list_size = 1;

	return NO_ERROR;
}


err_t list_dtor(struct List* list_ptr)
{
	CHECK_LIST()

	struct List_elem* elem_ptr = list_ptr->zero_elem_ptr;
	struct List_elem* next_elem_ptr = elem_ptr->next_elem_ptr;

	do 
	{
		if (elem_ptr != NULL)
		{
			free(elem_ptr);
			printf("free list elem [%p]\n", elem_ptr);
		}
		else 
		{
			return_code |= NULL_REFERENCE_TO_NEIGHBOR;
		}
		
		elem_ptr = next_elem_ptr;
		next_elem_ptr = next_elem_ptr->next_elem_ptr;

	} while (elem_ptr != list_ptr->zero_elem_ptr);

	list_ptr->list_size = -1;

	return return_code;
}


err_t list_verificator(const struct List* const list_ptr)
{
	err_t return_code = NO_ERROR;

	if (list_ptr->list_size < 0) return_code |= INVALID_LIST_SIZE;

	struct List_elem* elem_ptr = list_ptr->zero_elem_ptr;

	ssize_t elem_number = 0;

	do 
	{
		if (elem_ptr == NULL) 
		{
			return_code |= NULL_REFERENCE_TO_NEIGHBOR;
			return return_code;
		}
		if ((elem_ptr->next_elem_ptr)->prev_elem_ptr != elem_ptr)
		{
			return_code |= INVALID_PREV_OF_NEXT_OF_ELEM;
		}

		if (elem_number > list_ptr->list_size)
		{
			return_code |= VERY_BIG_LIST;
		}

		elem_number++;

	} while (elem_ptr != list_ptr->zero_elem_ptr);

	return return_code;
}


err_t print_list_error(const err_t return_code)
{
	switch (return_code)
	{
		case INVALID_LIST_SIZE:
			printf("invalid list size\n");
			break;
		case NULL_REFERENCE_TO_NEIGHBOR:
			printf("null reference to next or prev element in list\n");
			break;
		case INVALID_PREV_OF_NEXT_OF_ELEM:
			printf("prev from next does not match the original element\n");
			break;
		case VERY_BIG_LIST:
			printf("a loop that iterates through all elements has produced more iterations than the size of the array\n");
			break;
		case INVALID_ITERATOR:
			printf("invalid iterator in list usage\n");
			break;
		case UNABLE_TO_FREE_ZERO_ELEMENT:
			printf("unable to free zero element (elem_ptr_to_free == list_ptr->zero_elem_ptr)\n");
			break;
		default:
			printf("no error or wrong error code\n");
			break;
	}

	return return_code;
}


static struct List_elem* get_element_by_iterator(const struct List* const list_ptr, ssize_t iterator)
{
	ssize_t elem_number = 0;

	struct List_elem* elem_ptr = list_ptr->zero_elem_ptr;

	if (iterator >= list_ptr->list_size)
	{
		return NULL;
	}

	while (elem_number < iterator)
	{
		elem_ptr = elem_ptr->next_elem_ptr;
		elem_number++;
	}

	return 	elem_ptr;
}


err_t list_add_after_elem_by_iterator(struct List* const list_ptr, const ssize_t iterator, const elem_t elem)
{
	CHECK_LIST();

	struct List_elem* before_elem = get_element_by_iterator(list_ptr, iterator);

	if (before_elem == NULL)
	{
		return_code |= INVALID_ITERATOR;
		print_list_error(return_code);
		return return_code;
	}

	return_code |= list_add_after_elem_by_his_ptr(list_ptr, before_elem, elem);

	return return_code;
}


err_t list_add_before_elem_by_iterator(struct List* const list_ptr, const ssize_t iterator, const elem_t elem)
{
	CHECK_LIST();

	struct List_elem* after_elem = get_element_by_iterator(list_ptr, iterator);

	return_code |= list_add_before_elem_by_his_ptr(list_ptr, after_elem, elem);

	return return_code;
}


err_t list_add_after_elem_by_his_ptr(struct List* const list_ptr, struct List_elem* const before_elem, const elem_t elem)
{
	CHECK_LIST();

	struct List_elem* list_elem_ptr = (struct List_elem*) calloc(1 ,sizeof(struct List_elem));

	list_elem_ptr->elem = elem;

	struct List_elem* after_elem = before_elem->next_elem_ptr;

	before_elem->next_elem_ptr = list_elem_ptr;
	list_elem_ptr->prev_elem_ptr = before_elem;

	after_elem->prev_elem_ptr = list_elem_ptr;
	list_elem_ptr->next_elem_ptr = after_elem;

	(list_ptr->list_size)++;

	return return_code;
}


err_t list_add_before_elem_by_his_ptr(struct List* const list_ptr, struct List_elem* const after_elem, const elem_t elem)
{
	CHECK_LIST();

	struct List_elem* list_elem_ptr = (struct List_elem*) calloc(1 ,sizeof(struct List_elem));

	list_elem_ptr->elem = elem;

	struct List_elem* before_elem = after_elem->prev_elem_ptr;

	before_elem->next_elem_ptr = list_elem_ptr;
	list_elem_ptr->prev_elem_ptr = before_elem;

	after_elem->prev_elem_ptr = list_elem_ptr;
	list_elem_ptr->next_elem_ptr = after_elem;

	(list_ptr->list_size)++;

	return return_code;
}


err_t list_add_in_tail(struct List* const list_ptr, const elem_t elem)
{
	return list_add_before_elem_by_his_ptr(list_ptr, list_ptr->zero_elem_ptr, elem);
}


err_t list_add_in_head(struct List* const list_ptr, const elem_t elem)
{
	return list_add_after_elem_by_his_ptr(list_ptr, list_ptr->zero_elem_ptr, elem);
}


err_t print_list_elem(const struct List_elem* const list_elem_ptr, const ssize_t elem_number = UNDEFINED_LIST_NUMBER)
{
	err_t return_code = NO_ERROR;

	printf("\tstruct List_elem list_elem_ptr [%p]\n", list_elem_ptr);
	printf("\t{\n");

	printf("\t\telem          = " LIST_ELEM_PRINTF_SPEC "\n", list_elem_ptr->elem);

	printf("\t\telem number   = %zd\n", elem_number);

	printf("\t\tprev_elem_ptr = [%p]\n", list_elem_ptr->prev_elem_ptr);

	printf("\t\tnext_elem_ptr = [%p]\n", list_elem_ptr->next_elem_ptr);

	printf("\t}\n");

	return return_code;
}


err_t free_cell_by_his_ptr(struct List* const list_ptr, struct List_elem*  elem_to_free)
{
	CHECK_LIST();

	if (elem_to_free == list_ptr->zero_elem_ptr)
	{
		return_code |= UNABLE_TO_FREE_ZERO_ELEMENT;
		print_list_error(return_code);
		return return_code;
	}

	struct List_elem* before_elem = elem_to_free->prev_elem_ptr;

	struct List_elem* after_elem  = elem_to_free->next_elem_ptr;

	before_elem->next_elem_ptr = after_elem;
	after_elem->prev_elem_ptr = before_elem;

	free(elem_to_free);

	(list_ptr->list_size)--;

	return return_code;	
}


err_t free_cell_by_iterator(struct List* const list_ptr, const ssize_t iterator)
{
	CHECK_LIST();

	struct List_elem* elem_to_free = get_element_by_iterator(list_ptr, iterator);

	return_code |= free_cell_by_his_ptr(list_ptr, elem_to_free);	

	return return_code;
}


err_t list_dump(const struct List* const list_ptr)
{
	CHECK_LIST();

	struct List_elem* elem_ptr = list_ptr->zero_elem_ptr;

	printf("struct List list [%p]\n", list_ptr);
	printf("{\n");

	printf("\tlist size = %zd\n", list_ptr->list_size);

	printf("\tlist_head_ptr = [%p]\n", LIST_HEAD);

	printf("\tlist_tail_ptr = [%p]\n", LIST_TAIL);

	ssize_t elem_number = 0;

	do 
	{
		return_code |= print_list_elem(elem_ptr, elem_number);

		elem_number++;

		elem_ptr = elem_ptr->next_elem_ptr;
		
	} while (elem_ptr != list_ptr->zero_elem_ptr);

	printf("}\n");

	return return_code;
}