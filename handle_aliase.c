#include "main.h"

static int code_exiter;

/**
 * alias_handler - Processes alias commands.
 * @head: Pointer to the list of aliases.
 * @cmd: Command string related to alias.
 *
 * Return: 0 on success, -1 on error.
 */
int alias_handler(alias_t **head, char *cmd)
{
	while (*cmd == ' ')
		cmd++; /* Remove leading spaces */

	if (str_len(cmd) == 5)
		aliases_printer(*head);

	else if (!strn_cmp(cmd, "alias", 5))
	{
		if (!str_chr(cmd, '='))
			non_matching_processor(*head, cmd + 5, 1);
		else
			parsing_aliases(cmd, head);
	}

	else if (!strn_cmp(cmd, "unalias", 7))
		return (unalias(head, cmd));

	return (code_exiter);
}
/**
 * parsing_aliases - Extracts aliases from input string.
 * @data: Input string containing aliases.
 * @aliases: Pointer to alias list.
 *
 * Description: Uses regex to extract aliases from input and
 * populates the alias list.
 */
void parsing_aliases(const char *data, alias_t **aliases)
{
	regex_t regex;
	char *data_ptr = NULL;
	size_t value_of_Length, alias_counter = 0;
	regmatch_t mat[3];

	const char *patt =
		"([^\"]\\S*|\"[^\"]*\"|'[^\']*')=([^\"]\\S*|\"[^\"]*\"|'[^\']*')";

	if (regcomp(&regex, patt, REG_EXTENDED) != 0)
		return; /* Regex compilation failed */
	data_ptr = (char *)data;
	while (regexec(&regex, data_ptr, 3, mat, 0) == 0)
	{
		char izina[MAX_ALIAS_LENGTH] = {0};
		char value[MAX_VALUE_LENGTH] = {0};

		/* Extract alias name */
		strn_cpy(izina, (data_ptr + 1) + mat[1].rm_so,
				 (mat[1].rm_eo - 1) - mat[1].rm_so);
		/* Extract alias value */
		strn_cpy(value, data_ptr + mat[2].rm_so,
				 mat[2].rm_eo - mat[2].rm_so);
		izina[mat[1].rm_eo - mat[1].rm_so] = '\0';

		value_of_Length = mat[2].rm_eo - mat[2].rm_so;
		if (isquote(value[0])) /* Remove quotes */
		{
			strn_cpy(value, data_ptr + mat[2].rm_so + 1, value_of_Length - 2);
			value[value_of_Length - 2] = '\0';
		}
		else /* Copy as is */
			value[value_of_Length] = '\0';
		if (alias_add(aliases, izina, value) == NULL)
			return;
		if (alias_counter)
			non_matching_processor(*aliases, data_ptr, 0);
		alias_counter++;				   /* Increment alias couner */
		data_ptr += mat[0].rm_eo; /* Continue searching */
	}
	if (alias_counter)
		non_matching_processor(*aliases, data_ptr, 1);
	regfree(&regex);
}

/**
 * non_matching_processor - Handles strings without 'name=value' format.
 * @aliases: List of aliases.
 * @non_match: String to check for non-matching patterns.
 * @finish: Signal end; 1 for full string, 0 for word-by-word.
 */
void non_matching_processor(alias_t *aliases, const char *non_match, int finish)
{
	char *part, *copy;

	if (non_match == NULL || *non_match == '\0')
		return; /* No work, all matched */

	copy = str_dup(non_match);
	part = strtok(copy, " ");

	/* Non-matching if no equal sign */
	if (!str_chr(part, '='))
	{
		if (finish)
		{
			while (part != NULL)
			{
				code_exiter = alias_printer(aliases, part);
				part = *str_tok(NULL, " ");
			}
		}
		else
			code_exiter = alias_printer(aliases, part);
	}
	free(copy);
}

/**
 * cmd_alias_builder - Constructs the command line for a valid alias.
 * @sub_cmd: Pointer to the command array.
 * @value_of_alias: Alias value string.
 */
void cmd_alias_builder(char ***sub_cmd, char *value_of_alias)

{
	char **copy_array = NULL;

	if ((*sub_cmd)[1] != NULL)
	{
		/* Save a copy of the command array, excluding alias */
		copy_array = dup_str_array((*sub_cmd) + 1);
		if (copy_array == NULL)
		{
			fprintf(stderr, "alias: Memory allocation failed\n");
			return;
		}

		/* Build the command line based on alias value */
		str_free(sub_cmd);
		*sub_cmd = str_tok(value_of_alias, NULL);

		/* Concatenate arrays to form complete command */
		concat_arrays(sub_cmd, copy_array);

		/* Clean up */
		str_free(&copy_array);
	}
	else
	{
		/* No additional arguments */
		str_free(sub_cmd);
		*sub_cmd = str_tok(value_of_alias, NULL);
	}
}

