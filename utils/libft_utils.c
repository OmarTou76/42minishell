#include "../minishell.h"

int is_charset(char c, char *cs)
{
    int i;

    i = 0;
    while (cs[i])
    {
        if (cs[i] == c)
            return 1;
        i++;
    }
    return 0;
}

char *strchrs(char *s, char *cs)
{
    while (*s)
    {
        if (is_charset(*s, cs))
            return s;
        s++;
    }
    return NULL;
}

int ft_strlen(char *s)
{
    int i;

    i = 0;
    while (s && s[i])
        i++;
    return (i);
}

char *ft_strndup(char *s, int len)
{
    int i;
    char *new;

    i = 0;
    if (!s)
        return (NULL);
    if (ft_strlen(s) < len)
        len = ft_strlen(s);
    new = malloc(len + 1);
    while (s[i] && i < len)
    {
        new[i] = s[i];
        i++;
    }
    new[i] = 0;
    return (new);
}

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
    while ((*s1 || *s2) && n--)
    {
        if (*s1 != *s2)
            return ((unsigned char)*s1 - (unsigned char)*s2);
        s1++;
        s2++;
    }
    return (0);
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 || *s2)
    {
        if (*s1 != *s2)
            return ((unsigned char)*s1 - (unsigned char)*s2);
        s1++;
        s2++;
    }
    return (0);
}

static int count_words(const char *str, char c)
{
    int i;
    int trigger;

    i = 0;
    trigger = 0;
    while (*str)
    {
        if (*str != c && trigger == 0)
        {
            trigger = 1;
            i++;
        }
        else if (*str == c)
            trigger = 0;
        str++;
    }
    return (i);
}

static char *word_dup(const char *str, int start, int finish)
{
    char *word;
    int i;

    i = 0;
    word = malloc((finish - start + 1) * sizeof(char));
    while (start < finish)
        word[i++] = str[start++];
    word[i] = '\0';
    return (word);
}

char **ft_split(char *s, char c)
{
    int i;
    int j;
    int index;
    char **split;

    if (!s || !(split = malloc((count_words(s, c) + 1) * sizeof(char *))))
        return (0);
    i = 0;
    j = 0;
    index = -1;
    while (i <= ft_strlen(s))
    {
        if (s[i] != c && index < 0)
            index = i;
        else if ((s[i] == c || i == ft_strlen(s)) && index >= 0)
        {
            split[j++] = word_dup(s, index, i);
            index = -1;
        }
        i++;
    }
    split[j] = 0;
    return (split);
}

size_t ft_strlcpy(char *dst, const char *src, size_t size)
{
    unsigned int i;

    i = 0;
    if (!dst || !src)
        return (0);
    if (size > 0)
    {
        while (--size && src[i])
        {
            dst[i] = src[i];
            i++;
        }
        dst[i] = '\0';
    }
    while (src[i])
        i++;
    return (i);
}