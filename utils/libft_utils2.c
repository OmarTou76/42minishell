#include "../minishell.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*tmp1;
	unsigned char	*tmp2;

	i = 0;
	tmp1 = (unsigned char *)src;
	tmp2 = (unsigned char *)dest;
	while (i < n)
	{
		*tmp2++ = *tmp1++;
		i++;
	}
	return (dest);
}

char	*ft_strdup(const char *s)
{
	char	*s2;
	size_t	len;

	len = ft_strlen(s) + 1;
	s2 = (char *)malloc(sizeof(*s2) * len);
	if (!s2)
		return (NULL);
	return ((char *)ft_memcpy(s2, s, len));
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t	i;

	i = 0;
	while (i < maxlen && s[i])
		i++;
	return (i);
}

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	while (i < len)
		dst[i++] = '\0';
	return (dst);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;

	if (!s)
		return (NULL);
	while (*s && start--)
		s++;
	dest = malloc((ft_strnlen(s, len) + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	ft_strncpy(dest, s, len);
	dest[len] = '\0';
	return (dest);
}

char *ft_strcat(char *str1, const char *str2)
{
	size_t	i;

	i = 0;
	while (str1[i])
		i++;
	while (*str2)
		str1[i++] = *str2++;
	str1[i] = '\0';
	return (str1);
}

void	ft_putnl(char *str)
{
	ft_putstr(str);
	ft_putstr("\n");
}
