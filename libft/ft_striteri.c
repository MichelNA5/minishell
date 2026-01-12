/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmakhlou <mmakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2024/06/14 12:02:56 by mmakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		(*f)(i, &s[i]);
		i++;
	}
	s[i] = '\0';
}

/*
This function applies a function f to each character of the string s, 
passing the index of each character and a pointer to the character itself. 
It does not return any value and modifies the string s in place.


example of usage:

void to_upper(unsigned int i, char *c)
{
	*c = ft_toupper(*c);
}

int main()
{
	char str[] = "hello";
	ft_striteri(str, to_upper);
	printf("%s\n", str); // Outputs: "HELLO"
	return 0;
}

*/
