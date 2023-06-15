/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 16:56:05 by msoriano          #+#    #+#             */
/*   Updated: 2023/04/26 17:03:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*newbuffer(char *buffer)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (buffer[i] != '\0' && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free (buffer);
		return (NULL);
	}
	str = (char *)malloc(sizeof(char) * (ft_strlen(buffer) - i + 1));
	if (!str)
		return (NULL);
	i++;
	j = 0;
	while (buffer[i] != '\0')
		str[j++] = buffer[i++];
	str[j] = '\0';
	free (buffer);
	return (str);
}

static char	*saveline(char *buffer)
{
	int		i;
	char	*aux;

	i = 0;
	if (!buffer || buffer[0] == '\0')
		return (NULL);
	while (buffer[i] != '\0' && buffer[i] != '\n')
		i++;
	if (buffer [i] == '\n')
		aux = malloc(sizeof(char) * (i + 2));
	else
		aux = malloc(sizeof(char) * (i + 1));
	if (!aux)
		return (NULL);
	i = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
	{
		aux[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		aux[i++] = '\n';
	aux[i] = '\0';
	return (aux);
}

static char	*read_line(int fd, char *buffer)
{
	char	*str;
	int		nread;

	str = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!str)
		return (NULL);
	nread = 1;
	while (!(ft_strchr(buffer, '\n')) && nread > 0)
	{
		nread = read(fd, str, BUFFER_SIZE);
		if (nread < 0)
		{
			free (str);
			return (NULL);
		}
		str[nread] = '\0';
		buffer = ft_strjoin(buffer, str);
	}
	free (str);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer[OPEN_MAX];
	char		*str;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free (buffer[fd]);
		buffer[fd] = NULL;
		return (NULL);
	}
	buffer[fd] = read_line(fd, buffer[fd]);
	if (!buffer[fd])
		return (NULL);
	str = saveline(buffer[fd]);
	buffer[fd] = newbuffer(buffer[fd]);
	return (str);
}
