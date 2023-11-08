#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "include/list.h"
#include "include/graph.h"
#include <stdlib.h>

#define CREATE_DOT_LINE(command_string) fprintf(dot_file, "" command_string "\n");
#define CREATE_HTML_LINE(command_string) fprintf(html_file, "" command_string "\n");

#define RUN_DOT_FILE(graph_filename, dot_filename, graph_file_extension)  								\
{																										\
	char system_command[MAX_COMMANDNAME_SIZE] = {};														\
	sprintf(system_command, "dot -T%s %s -o %s", graph_file_extension, dot_filename, graph_filename);	\
	system(system_command);																				\
}																										\


#define RUN_HTML_FILE(graph_folder, html_filename)												\
{																								\
	char system_command[MAX_COMMANDNAME_SIZE] = {};												\
	sprintf(system_command, "cd %s && %s", graph_folder, html_filename);						\
	printf("%s", system_command);																\
	system(system_command);																		\
}																								\


FILE* open_file(const char* const filename, const char* const modificator)
{
	FILE* inputfile = fopen(filename, modificator);
	assert(inputfile);
	return inputfile;
}


err_t create_one_graph(struct List* list_ptr, ssize_t* graph_number_ptr, char *  graph_filepath, const char * const dot_filepath, const char* const graph_file_extension, FILE* html_file)	
{	
	err_t return_code = list_verificator(list_ptr);

	sprintf(graph_filepath, "graph/graph%zd.png", *graph_number_ptr);						
																						
	write_to_dot_file(list_ptr, dot_filepath);											
																						
																						
																						
	RUN_DOT_FILE(graph_filepath, dot_filepath, graph_file_extension)					
																		
	CREATE_HTML_LINE("<section>")				
	fprintf(html_file, "<img src=graph%zd.png alt=\"struct List list [%p]\"/>\n", *graph_number_ptr, list_ptr);	
	CREATE_HTML_LINE("</section>")					
																						
	(*graph_number_ptr)++;	

	return return_code;
}		


err_t write_to_dot_file(struct List* list_ptr, const char* const dot_filepath)
{
	CHECK_LIST();

	FILE* dot_file = open_file(dot_filepath, "w"); 
	assert(dot_file);

	CREATE_DOT_LINE("digraph G {")
	fprintf(dot_file, "rankdir = \"LR\";\n");
	fprintf(dot_file, "graph [splines=ortho];\n");
	CREATE_DOT_LINE("subgraph cluster0 {")

	fprintf(dot_file, "label = \"struct List list [%p]\";", list_ptr);

	fprintf(dot_file, "fontname=\"Helvetica,Arial,sans-serif\";\n");
	fprintf(dot_file, "node [fontname=\"Helvetica,Arial,sans-serif\" fontsize = \"16\" ];\n");
	fprintf(dot_file, "edge [fontname=\"Helvetica,Arial,sans-serif\"];\n");




	CREATE_DOT_LINE("subgraph cluster1 {")
	fprintf(dot_file, "rankdir = \"TB\";\n");
	fprintf(dot_file, "label = \"list variables\"");
	fprintf(dot_file, "list_size [shape=box3d, label=\"list size = %zd\"];\n", list_ptr->list_size);


	CREATE_DOT_LINE("}")	
	CREATE_DOT_LINE("subgraph cluster2 {")

	fprintf(dot_file, "label = \"list elements\"\n");

	ssize_t elem_number = 0;

	struct List_elem* list_elem_ptr = list_ptr->zero_elem_ptr;

	do 
	{
		fprintf(dot_file, "\"node%zd\" [ label = \"struct List_elem list_elem_ptr [%p] <f1> | { <f0> elem = " LIST_ELEM_PRINTF_SPEC " | <f1> elem_number = %zd | <f2> prev_elem_ptr = [%p] | <f3> elem_ptr = [%p] | <f4> next_elem_ptr = [%p] } \" shape = \"Mrecord\", color=\"Yellow\"];\n",elem_number,  list_elem_ptr,  list_elem_ptr->elem, elem_number, list_elem_ptr->prev_elem_ptr, list_elem_ptr, list_elem_ptr->next_elem_ptr);




		printf("%zd\n", elem_number);


		elem_number++;

		list_elem_ptr = list_elem_ptr->next_elem_ptr;
	} while (list_elem_ptr != list_ptr->zero_elem_ptr);


	/*for (ssize_t n_data_elem = 0; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if ((list_ptr->prev)[n_data_elem] >= 0)
		{
			fprintf(dot_file, "\"node%zd\":f0 -> \"node%zd\":f2 [id = %zd, color=\"#%06X\", constraint = false, splines = none];\n", n_data_elem, (list_ptr->prev)[n_data_elem], 3 * list_ptr->data_size + n_data_elem, rand() % MAX_COLOR_VALUE);
		}		
	}	*/


	//fprintf(dot_file, " free -> \"node%zd\": f4 [color = \"#%06X\"]", (list_ptr->free), rand() % MAX_COLOR_VALUE);

	CREATE_DOT_LINE("} } }")

	fclose(dot_file);


	return return_code;
}