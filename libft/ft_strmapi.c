/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmakhlou <mmakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2024/06/14 12:04:55 by mmakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	unsigned int	length;
	char			*res;

	if (!s)
		return (NULL);
	length = ft_strlen(s);
	res = malloc(length * sizeof(char) + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < length)
	{
		res[i] = (*f)(i, s[i]);
		i++;
	}
	res[i] = '\0';
	return (res);
}

/*
This function creates a new string 
by applying a function f to each character of the string s, 
passing the index of each character and the character itself. 
It returns the new string without modifying the original string s.

example of usage:

char add_index(unsigned int i, char c)
{
	return (c + i);
}

int main()
{
	char *str = "abcd";
	char *new_str = ft_strmapi(str, add_index);
	printf("%s\n", new_str); // Outputs a new string 
	                            with each char modified by its index
	free(new_str);
	return 0;
}

*/
