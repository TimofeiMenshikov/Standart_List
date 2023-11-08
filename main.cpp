#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "include/list.h"
#include "include/graph.h"


#define START_TEST()      																\
	err_t return_code = NO_ERROR;														\
																						\
	printf("___________________________________________________________________\n");	\
																						\
	printf("start %s\n", __func__);														\
																						\
	struct List list;																	\
																						\
	return_code |= list_init(&list);													\
																						\
	return_code |= list_dump(&list);													\
																						\
	FILE* html_file = fopen(html_filepath, "w");										\
	assert(html_file);																	\
																						\
	char graph_filepath[MAX_FILENAME_SIZE] = {};										\
																						\
	ssize_t number = 0;																	\


#define END_TEST()																			\
	list_dtor(&list);																		\
    fclose(html_file);																		\
	RUN_HTML_FILE(graph_folder, html_filename);												\

int main()
{
	START_TEST()

	CREATE_ONE_GRAPH()

	list_add_after_elem_by_his_ptr(&list, list.zero_elem_ptr, 3);

	CREATE_ONE_GRAPH()

	list_add_after_elem_by_iterator(&list, 0, 2);

	CREATE_ONE_GRAPH()


	list_add_before_elem_by_iterator(&list, 1, 4);

	CREATE_ONE_GRAPH()

	free_cell_by_iterator(&list, 3);

	CREATE_ONE_GRAPH()

	END_TEST()



	return 0;
}