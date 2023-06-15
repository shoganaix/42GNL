/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 11:40:48 by msoriano          #+#    #+#             */
/*   Updated: 2023/05/10 13:35:36 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	static char	*buffer;
	char		*str;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free (buffer);
		buffer = NULL;
		return (NULL);
	}
	buffer = read_line(fd, buffer);
	if (!buffer)
		return (NULL);
	str = saveline(buffer);
	buffer = newbuffer(buffer);
	return (str);
}

/*#include <fcntl.h>
#include <stdio.h>
int	main(void)
{
	int		fd;
	char	*path = "example.txt";

fd = open(path, O_RDONLY);
if (fd < 0)
{
	printf("Could not open file: %s\n", path);
	return (0);
}
char *line;
while (1)
{
	line = get_next_line(fd);
	if (!line)
	{
		printf("This line is null \n");
		break;
	}
	printf("This is the line: %s\n", line);
	free(line);
}
	close(fd);
	return (0);
}*/
