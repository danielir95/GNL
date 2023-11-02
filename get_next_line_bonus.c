/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daibanez <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 13:39:10 by daibanez          #+#    #+#             */
/*   Updated: 2023/10/18 13:39:31 by daibanez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*intr_l(char *buff, int *new_line)
{
	size_t	len;
	char	*line;

	len = 0;
	while (buff[len] && buff[len] != '\n')
		len++;
	len++;
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	ft_memcpy(line, buff, len);
	line[len] = '\0';
	if (len > 0 && line[len - 1] == '\n')
		*new_line = len - 1;
	return (line);
}

size_t	loc_new_line(char *line)
{
	size_t	i;

	i = 0;
	if (!line)
		return (-1);
	while (i < BUFFER_SIZE)
	{
		if (line[i] == '\n' || line[i] == '\0')
			return (i + 1);
		i++;
	}
	return (i);
}

char	*find_line(char *line, char *buff, int *new_line, int fd)
{
	char	buffer[BUFFER_SIZE + 1];
	ssize_t	read_ok;
	size_t	line_size;

	while (*new_line == -1)
	{
		ft_bzero(buffer, (BUFFER_SIZE + 1));
		read_ok = read(fd, buffer, BUFFER_SIZE);
		if (read_ok == -1)
		{
			free(line);
			ft_bzero(buff, (BUFFER_SIZE + 1));
			return (NULL);
		}
		line_size = loc_new_line(buffer);
		ft_strlcpy(buff, &buffer[line_size], (BUFFER_SIZE + 1));
		buffer[line_size] = '\0';
		line = ft_strjoin(line, buffer, new_line);
		if (read_ok == 0)
		{
			ft_bzero(buff, BUFFER_SIZE + 1);
			break ;
		}
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	buff[OPEN_MAX][BUFFER_SIZE + 1];
	char		*line;
	int			new_line;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	new_line = -1;
	line = intr_l(buff[fd], &new_line);
	if (!line)
		return (NULL);
	ft_strlcpy(buff[fd], &buff[fd][new_line + 1], BUFFER_SIZE + 1);
	line = find_line(line, buff[fd], &new_line, fd);
	if (!line || line[0] == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}
