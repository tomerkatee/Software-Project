#include "spkmeans.h"
#include <string.h>

/* globals */
int N, dp_len;


void *calloc_and_check(size_t nitems, size_t size)
{
    void *p = calloc(nitems, size);
    if(p == NULL)
    {
        printf("An Error Has Occurred!\n");
        exit(1);
    }
    return p;
}

int get_dp_len(char *line)
{
    char* c = line;
    int len = 0;
    while(1)
    {
        if (*c == ',' || *c == '\n') len++;
        if(*c=='\n') break;
        c++;
    } 
    return len;
}


Datapoint read_line_to_vector(char* line)
{
    Datapoint dp = (Datapoint)calloc_and_check(dp_len, sizeof(double));
    char* c = line;
    char buffer[64];
    int dp_index = 0;
    int char_index = 0;
    do
    {
        if(*c == ',' || *c == '\n')
        {
            buffer[char_index] = 0;
            dp[dp_index++] = atof(buffer);
            char_index = 0;
        }
        else
        {
            buffer[char_index++] = *c;
        }  
    } while (*c++ != '\n');
    return dp;
}

Datapoint* read_datapoints(char* file_name)
{
    FILE *file;
    file = fopen(file_name, "r");
    long unsigned len = 10;
    char *line = (char*)calloc_and_check(len, sizeof(char));
    DPNode *head, *p, *tmp, *new;
    Datapoint dp;
    Datapoint* datapoints;
    int first = 1, i, dp_len = 0;
    N = 0;
    while(getline(&line, &len, stdin) != -1)
    {
        if(first) 
        {
            dp_len = get_dp_len(line);
        }
        dp = read_line_to_vector(line);
        new = (DPNode*)calloc_and_check(1, sizeof(DPNode));
        if(first)
        {
            head = new;
            p = head;
        }
        else
        {
            p->next = new;
            p = p->next;
        }
        p->next = NULL;
        p->value = dp;
        N++;
        first = 0;
    }
    fclose(file);
    
    free(line);
    datapoints = (Datapoint*)calloc_and_check(N, sizeof(Datapoint));
    for (i = 0; i < N; i++)
    {
        datapoints[i] = head;
        tmp = head;
        head = head->next;
        tmp->next = NULL;
        tmp->prev = NULL;
    }

    return datapoints;

}


int main(int argc, char* argv[]){
    char* goal = argv[1], file_name = argv[2];
    Matrix m;
    if(!strcmp(goal, "jacobi"))
    {
        m = read_datapoints(file_name);
    }
    else
    {
        Datapoint* datapoints = read_datapoints(file_name);
        m = wam(datapoints);
        if(!strcmp(goal, "wam")){
            print_matrix(m);
            return 0;
        }
        m = ddg(datapoints); 
        if(!strcmp(goal, "ddg")){
            print_matrix(m);
            return 0;
        }
        m = gl(datapoints);
        print_matrix(gl);
    }
    return 0;
}

