#include "shell.h"

/**
 * main - the main function
 *
 * Return: (Success) 0 always
 * ------- (Fail) we drop out the looser :)
 */
int main(void)
{
	sh_t data;
	int pl;

	_memset((void *)&data, 0, sizeof(data));
	signal(SIGINT, signal_handler);
	while (1)
	{
		index_cmd(&data);
		if (read_line(&data) < 0)
		{
			if (isatty(STDIN_FILENO))
				PRINT("\n");
			break;
		}
		if (split_line(&data) < 0)
		{
			free_data(&data);
			continue;
		}
		pl = parse_line(&data);
		if (pl == 0)
		{
			free_data(&data);
			continue;
		}
		if (pl < 0)
		{
			print_error(&data);
			continue;
		}
		if (process_cmd(&data) < 0)
		{
			print_error(&data);
			break;
		}
		free_data(&data);
	}
	free_data(&data);
	exit(EXIT_SUCCESS);
}

/**
 * read_line - read a line from the standard input
 * @data: a pointer to the struct of data
 *
 * Return: (Success) a positive number
 * ------- (Fail) a negative number
 */
int read_line(sh_t *data)
{
	char *csr_ptr, *end_ptr, c;
	size_t size = BUFSIZE, read_st, length, new_size;

	data->line = malloc(size * sizeof(char));
	if (data->line == NULL)
		return (FAIL);
	if (isatty(STDIN_FILENO))
		PRINT(PROMPT);
	for (csr_ptr = data->line, end_ptr = data->line + size;;)
	{
		read_st = read(STDIN_FILENO, &c, 1);
		if (read_st == 0)
			return (FAIL);
		*csr_ptr++ = c;
		if (c == '\n')
		{
			*csr_ptr = '\0';
			return (SUCCESS);
		}
		if (csr_ptr + 2 >= end_ptr)
		{
			new_size = size * 2;
			length = csr_ptr - data->line;
			data->line = _realloc(data->line, size * sizeof(char),
						new_size * sizeof(char));
			if (data->line == NULL)
				return (FAIL);
			size = new_size;
			end_ptr = data->line + size;
			csr_ptr = data->line + length;
		}
	}
}
