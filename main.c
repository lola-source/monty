#include "monty.h"
stack_t *head = NULL;

int main(int argc, char **argv)
{
	if (argc < 2 || argc > 2)
		err(1);
	open_file(argv[1]);
	free_nodes();
	return (0);
}

void free_nodes(void)
{
	stack_t *tmp;

	if (head == NULL)
		return;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}


stack_t *create_node(int n)
{
	stack_t *node;

	node = malloc(sizeof(stack_t));
	if (node == NULL)
		err(4);
	node->next = NULL;
	node->prev = NULL;
	node->n = n;
	return (node);
}
