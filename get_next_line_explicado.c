static char	*newbuffer(char *buffer)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	// Se recorre el buffer hasta encontrar el final de línea o fin de archivo
	while (buffer[i] != '\0' && buffer[i] != '\n')
		i++;
	// Si no hay más caracteres en el buffer, se libera y retorna NULL
	if (!buffer[i])
	{
		free (buffer);
		return (NULL);
	}
	// Se crea un nuevo buffer para almacenar la siguiente línea y se copia el contenido allí
	str = (char *)malloc(sizeof(char) * (ft_strlen(buffer) - i + 1));
	if (!str)
		return (NULL);
	// Se copia el contenido del buffer después de la primera ocurrencia de '\n' a la nueva cadena
	i++;
	j = 0;
	while (buffer[i] != '\0')
		str[j++] = buffer[i++];
	str[j] = '\0';
	// Se libera la memoria del buffer original
	free (buffer);
	return (str);
}
/*La función newbuffer se encarga de crear un nuevo buffer para almacenar la siguiente
línea del archivo. Se recorre el buffer de entrada hasta encontrar el final de línea o fin de archivo
Si no hay más caracteres en el buffer, se libera el buffer y se retorna NULL. En caso contrario
se crea un nuevo buffer y se copia el contenido desde el final de línea o fin de archivo hasta el final del buffer en el nuevo buffer
Luego, se libera el buffer anterior y se retorna el nuevo buffer
*/

static char	*saveline(char *buffer)
{
	int		i;
	char	*aux;

	i = 0;
	// Si el buffer está vacío o contiene una línea vacía, se retorna NULL
	if (!buffer || buffer[0] == '\0')
		return (NULL);
	// Se recorre el buffer hasta encontrar el final de línea o fin de archivo
	while (buffer[i] != '\0' && buffer[i] != '\n')
		i++;
	// Se reserva memoria para un nuevo buffer que contendrá la línea, incluyendo '\n' si existe en el buffer
	if (buffer [i] == '\n')
		aux = malloc(sizeof(char) * (i + 2));
	else
		aux = malloc(sizeof(char) * (i + 1));
	if (!aux)
		return (NULL);
	// Se copia el contenido del buffer hasta el final de línea o fin de archivo en el nuevo buffer
	i = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
	{
		aux[i] = buffer[i];
		i++;
	}
	// Si se encontró una línea, se agrega el carácter de fin de línea en el nuevo buffer
	if (buffer[i] == '\n')
		aux[i++] = '\n';
	aux[i] = '\0';
	return (aux);
}
/*La función saveline tiene como objetivo extraer la primera línea completa del buffer de entrada y
guardarla en una cadena de caracteres independiente, para su posterior uso o procesamiento.
Para lograr esto, la función recorre el buffer hasta encontrar el caracter de nueva línea '\n'
Luego, la función asigna dinámicamente la memoria necesaria para guardar la cadena de caracteres
correspondiente a la primera línea, incluyendo el caracter de nueva línea si este existe
Finalmente, devuelve la cadena de caracteres creada
Si no se encuentra ninguna línea completa en el buffer de entrada, la función devuelve NULL
*/

static char	*read_line(int fd, char *buffer)
{
	char	*str;
	int		nread;

	// Se reserva memoria para la cadena temporal
	str = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!str)
		return (NULL);
	nread = 1;
	// Se inicializa el último byte de la cadena temporal con '\0'
	str[BUFFER_SIZE + 1] = '\0';
	// Mientras no se encuentre '\n' en el buffer y el número de bytes leídos sea diferente de 0
	while (!(ft_strchr(buffer, '\n')) && nread != 0)
	{
		//Leer BUFFER_SIZE bytes del archivo fd y guardarlos en el buffer temporal str, y el número de bytes leídos en nread
		nread = read(fd, str, BUFFER_SIZE);
		//Si no se pudo leer del archivo, liberar la memoria asignada a str y retornar NULL
		if (nread < 0)
		{
			free (str);
			return (NULL);
		}
		//Colocar un carácter nulo ('\0') en la última posición del buffer temporal, para asegurarse de que sea una cadena válida
		str[nread] = '\0';
		//Concatena el buffer anterior con el contenido leído del archivo, utilizando la función ft_strjoin, y actualizar el valor de la variable buffer.
		buffer = ft_strjoin(buffer, str);
	}
	//Libera lo que ya no vamos a utilizar
	free (str);
	//Devuelve el buffer actualizado
	return (buffer);
}
/*La función readline lee el contenido de un archivo descriptor de archivo (file descriptor/fd) dado y lo almacena en un búfer (buffer).
Si encuentra una nueva línea (\n) en el contenido del archivo, detiene la lectura y devuelve el contenido del búfer hasta ese punto,
incluyendo la nueva línea.
Si no encuentra una nueva línea y llega al final del archivo, devuelve el contenido completo del búfer

La función utiliza una técnica de lectura en bloques utilizando la constante BUFFER_SIZE para leer el archivo en trozos de tamaño fijo
También utiliza la función ft_strjoin para unir los bloques leídos del archivo a medida que se leen en el búfer
*/

char	*get_next_line(int fd)
{
	//variable estática que se utiliza para almacenar los datos del archivo que aún no han sido leídos
	static char	*buffer;
	//Es una variable utilizada para almacenar la línea leída del archivo
	char		*str;

	//Se verifica que el file descriptor sea válido, que el tamaño del buffer sea mayor que 0 y que se pueda leer del archivo.
	//Si no se cumplen estas condiciones, se libera el buffer y se devuelve un puntero nulo
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free (buffer);
		buffer = NULL;
		return (NULL);
	}
	buffer = read_line(fd, buffer);
	//Se llama a la función read_line para leer una línea del archivo y se almacena en el buffer
	if (!buffer)
		return (NULL);
	//Si el buffer es nulo, significa que no se pudo leer una línea del archivo, por lo que se devuelve un puntero nulo
	str = saveline(buffer);
	//Se llama a la función saveline para guardar la línea en la variable str
	buffer = newbuffer(buffer);
	//Se llama a la función newbuffer para actualizar el contenido del buffer y eliminar la línea leída
	return (str);
	//Se devuelve un puntero a la línea leída del archivo
}
