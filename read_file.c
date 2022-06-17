#define _GNU_SOURCE
#include "monty.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>


void open_file(char *file_name)
{
	int file_check;
	FILE *fd;

	if (file_name == NULL)
		err(2, file_name);
/*Checks if the file exists*/
	file_check = access(file_name, R_OK);
	if (file_check == -1)
		err(2, file_name);
	fd = fopen(file_name, "r");
	if (fd == NULL)
		err(2, file_name);
/*errors should be handled inside this function*/
	read_file(fd);
/*might need to check for errors on closing a file.*/
	fclose(fd);
}

void read_file(FILE *fd)
{
	int line_n;
	int format;
	char *lineprt;
	size_t n;

	lineprt = NULL;
	n = 0;
	format = 0;

	if (fd == NULL)
		err(2, "file_name");

	for (line_n = 1; getline(&lineprt, &n, fd) != EOF; line_n++)
	{
		format = interpret_line(lineprt, line_n, format);
	}
	free(lineprt);
}

int interpret_line(char *lineptr, int line_number, int format)
{
	const char *delim;
	char *opcode;
	char *value;

	if (lineptr == NULL)
		err(4);
	delim = "\n ";
	opcode = strtok(lineptr, delim);
/*hanlding blank lines*/
	if (opcode == NULL)
		return (format);
	value = strtok(NULL, delim);
	if (strcmp(opcode, "queue") == 0)
		return (1);
	else if (strcmp(opcode, "stack") == 0)
		return (0);
	find_func(opcode, value, line_number, format);
		return (format);
}

void find_func(char *opcode, char *value, int ln, int format)
{
	int i;
	int flag;

	instruction_t func_list[] = {
		{"push", add_to_stack},
		{"pall", print_stack},
		{"pint", print_top},
		{"pop", pop_top},
		{"nop", nop},
		{"swap", swap_nodes},
		{"add", add_nodes},
		{"sub", sub_nodes},
		{"div", div_nodes},
		{"mul", mul_nodes},
		{"mod", mod_nodes},
		{"pchar", print_char},
		{"pstr", print_str},
		{"rotl", rotl},
		{"rotr", rotr},
		{NULL, NULL}
	};
	if (opcode[0] == '#')
		return;
/*Iterates through list to find the right function*/
	for (flag = 1, i = 0; func_list[i].opcode != NULL; i++)
	{
/*When 0 found the right opcode*/
		if (strcmp(opcode, func_list[i].opcode) == 0)
		{
			call_fun(func_list[i].f, opcode, value, ln, format);
			flag = 0;
		}
	}
	if (flag == 1)
		err(3, ln, opcode);
}


void call_fun(op_func f, char *op, char *val, int ln, int format)
{
	stack_t *node;
	int flag;
	int i;

	flag = 1;
	if (strcmp(op, "push") == 0)
	{
/*Checks if the number is negative and moves the val ptr*/
		if (val != NULL && val[0] == '-')
		{
			val = val + 1;
			flag = -1;
	}
/*val is not a digit is the return value is 0*/
		if (val == NULL)
			err(5, ln);
		for (i = 0; val[i] != '\0'; i++)
		{
			if (isdigit(val[i]) == 0)
				err(5, ln);
		}
		node = create_node(atoi(val) * flag);
		if (format == 0)
			f(&node, ln);
		if (format == 1)
			add_to_queue(&node, ln);
	}
	else
		f(&head, ln);
}
