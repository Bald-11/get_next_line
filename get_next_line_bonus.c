/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 22:14:20 by yabarhda          #+#    #+#             */
/*   Updated: 2024/11/17 10:14:51 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int	check_newline(char *string)
{
	if (!string)
		return (0);
	while (*string)
	{
		if (*string == '\n')
			return (1);
		string++;
	}
	return (0);
}

static char	*extract_line(char **stash)
{
	int		line_len;
	int		is_newline;
	char	*line;
	char	*temp_stash;

	line_len = 0;
	is_newline = 0;
	temp_stash = NULL;
	while ((*stash)[line_len] != '\n' && (*stash)[line_len] != '\0')
		line_len++;
	if ((*stash)[line_len] == '\n')
		is_newline = 1;
	line = (char *)malloc(line_len + is_newline + 1);
	if (!line)
		return (NULL);
	ft_strlcpy(line, *stash, line_len + is_newline + 1);
	if ((*stash)[line_len] == '\n')
		temp_stash = ft_strdup(*stash + line_len + 1);
	free(*stash);
	*stash = NULL;
	if (temp_stash)
		*stash = ft_strdup(temp_stash);
	free(temp_stash);
	return (line);
}

static int	extract_and_stash(int fd, char **stash, char *buffer)
{
	int		size;
	char	*temp;

	size = read(fd, buffer, BUFFER_SIZE);
	if (size < 0)
		return (-1);
	buffer[size] = '\0';
	if (!*stash)
		*stash = ft_strdup(buffer);
	else
	{
		temp = *stash;
		*stash = ft_strjoin(*stash, buffer);
		free(temp);
	}
	return (size);
}

char	*get_next_line(int fd)
{
	static char	*stash[1024];
	char		*buffer;
	int			size;

	buffer = NULL;
	if (BUFFER_SIZE <= 0 || fd <= -1)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	size = 1;
	while (check_newline(stash[fd]) == 0 && size > 0)
	{
		size = extract_and_stash(fd, &stash[fd], buffer);
		if (size < 0 || *(stash[fd]) == '\0')
		{
			free(stash[fd]);
			free(buffer);
			stash[fd] = NULL;
			return (NULL);
		}			
	}
	free(buffer);
	return (extract_line(&stash[fd]));
}
