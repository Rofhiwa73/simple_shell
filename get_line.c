#include "shell.h"

/**
 * free_info - Free fields of the info_t struct
 * @info: The address of the info_t struct
 * @all: True if freeing all fields
 */
void free_info(info_t *info, int all)
{
	sfree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			free_list(&(info->env));
		if (info->history)
			free_list(&(info->history));
		if (info->alias)
			free_list(&(info->alias));
		sfree(info->environ);
		info->environ = NULL;
		_free((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}

/**
 * remove_env - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: 1 on successful deletion, 0 otherwise
 * @var: The environment variable property to remove
 */
int remove_env(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = check_starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = del_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * _setenv - Initialize a new environment variable or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * @var: The environment variable property
 * @value: The environment variable value
 * Return: Always 0
 */
int _setenv(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = info->env;
	while (node)
	{
		p = check_starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	_add_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
