#include "minishell.h"
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*p;
	const unsigned char	*m;
	size_t				i;

	if (dest == NULL && src == NULL)
		return (NULL);
	p = (unsigned char *)dest;
	m = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		p[i] = m[i];
		i++;
	}
	return (dest);
}

char	*ft_strdup(const char *s)
{
	char	*v;
	size_t	len;

	len = ft_strlen(s);
	v = malloc(len + 1);
	if (v == NULL)
		return (NULL);
	ft_memcpy(v, s, len);
	v[len] = '\0';
	return (v);
}

char *ft_substr(char const *s, unsigned int start, size_t len)
{
    char *sub;
    size_t i;

    if (!s)
        return NULL;
    if (start >= strlen(s))
        return strdup("");
    if ((sub = malloc(len + 1)) == NULL)
        return NULL;
    i = 0;
    while (i < len && s[start + i])
    {
        sub[i] = s[start + i];
        i++;
    }
    sub[i] = '\0';
    return sub;
}

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n'
         || c == '\v' || c == '\f' || c == '\r');
}

char *ft_strndup(const char *s, size_t n) {
    char *new = malloc(n + 1);
    if (!new) return NULL;
    ft_memcpy(new, s, n);
    new[n] = '\0';
    return new;
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}
