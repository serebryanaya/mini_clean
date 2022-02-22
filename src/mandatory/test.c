#include <stdio.h>
# include <readline/readline.h>
#include <stdlib.h>
#include <string.h>

int spaces(char *str, int *i)
{
    while (str[*i] && str[*i] == ' ')
        (*i)++;
    return (0);
}

// void find_and(char *str, int *i) // ><&
// {
//     if (str[*i + 1] && str[*i + 1] == '&')
        
//     return (0);
// }

// void find_out2(char *str, int *i) // ><>
// {
//     if (str[*i + 1] && str[*i + 1] == '&')
        
//     return (0);
// }

int find_in(char *str, int *i) // ><<
{
    if (str[*i + 1] && str[*i + 1] == '<')
    {
        // input->token = modif_strdup("<<<", input);
        printf("syntax error near unexpected token `<<<'\n");
        return (1) ; // or return?
    }
    else
        // input->token = modif_strdup("<<", input);
        printf("syntax error near unexpected token `<<'\n"); 
    return (0);
}

int find_out(char *str, int *i, char *res) // >>
{
    char res[1];
    char *tmp;

    (*i)++;
    res[0] = '\0';

    if (!str[*i] || spaces(str, i) == 0)
    {
        if (!str[*i])
        // input->token = modif_strdup("newline", input);
        {
            printf("syntax error near unexpected token `newline'\n", newline);
            return (1); 
        }    
    }
    else if (str[*i] == ';' || str[*i] == '&' || \
    str[*i] == '|' || str[*i] == '>' || str[*i] == '<')
    {
        tmp = strjoin(res, str[*i]); 
        res = tmp;
        free(tmp);
        if (str[*i + 1] && str[*i] != ' ' && str[*i] == str[*i + 1])
        {
            tmp = strjoin(res, str[*i]); 
            res = tmp;
            free(tmp);
            // input->token = modif_strdup(res, input);
            printf("syntax error near unexpected token `%s\n", res);
            return (1);   
        }
    else if (str[*i] != ' ')
    {
        spaces(str, i);
        free(res);
        res[0] = '\0';

     } 



        else if (str[*i + 1] &&)
        {

        }
        // input->token = modif_strdup(";", input);
        printf("syntax error near unexpected token `;\n");
        return (1);     
    }
    else if (str[*i] == '*')
    {
        // input->token = modif_strdup("!!!! ambiguous redirect", input);
        printf("minishell: *: ambiguous redirect\n");
        return (-1);     // !!!!!!!
    }
    
    


    else if (str[*i] == '<') // >><
    {
        if (str[*i + 1] && str[*i + 1] == '&') // >><&
            // input->token = modif_strdup("<&", input);
            printf("syntax error near unexpected token `<&\n");
        else
            // input->token = modif_strdup("<", input);
            printf("syntax error near unexpected token `<\n");

        return (1);   
    }


    else if (str[*i] == '>') // >>>
    {
        if (str[*i] && (str[*i] == '>' || str[*i] == '&' || str[*i] == '|')) // >>>> >>>& >>>|
        {
            token[1] = str[*i];
            // input->token = modif_strdup(token, input);
            printf("syntax error near unexpected token `%s'\n", token);
            return (1);         
        }
        else
        {
            // input->token = modif_strdup(token, input);
            printf("syntax error near unexpected token `%s'\n", token);
            return (1);         
        }  
    }

    return (0);
}

int main (int argc, char **argv)
{
    char *str;
    int i = 0;

    str = readline("my test: ");
    // printf("%s\n", str);
    while (str[i] && spaces(str, &i) == 0)
    {
        if (str[i] && str[i] == '>')
        {
            printf("find > \n");
            i++;
            if (str[i] && str[i] == '>') // >>
            {
                printf("find >> \n");
                if (find_out(str, &i, res) == 1)
                    break ;
            }
            else if (str[i] == '<')
            {
                i++;
                if (str[i] && str[i] == '<') //  ><<
                {
                    if (find_in(str, &i) == 1)
                        break ;
                }
                else if (str[i] == '>') // ><>
                {
                    // input->token = modif_strdup("<>", input);
                    printf("syntax error near unexpected token `<>'\n");
                    break ; // or return?
                }
                else if (str[i] == '&') // // ><&
                {
                    // input->token = modif_strdup("<&", input);
                    printf("syntax error near unexpected token `<&>'\n");
                    break ; // or return?
                }
                else
                {
                    // input->token = modif_strdup("<", input);
                    printf("syntax error near unexpected token `<'\n");
                    break ; // or return?
                }
            }
        }
        else
            printf("Sleeping...\n");
    }
}