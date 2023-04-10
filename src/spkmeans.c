#include "spkmeans.h"


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
    Datapoint dp = (Datapoint)calloc_and_check(dp_size, sizeof(double));
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

int count_lines(FILE* fp)
{
    int cnt = 0;
    char c;
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n')
            cnt++;
    return cnt;
}

Datapoint* read_datapoints(char* file_name)
{
    FILE *file;
    char* line;
    size_t len = 64;
    Datapoint* datapoints;
    int i = 1, ignore;

    file = fopen(file_name, "r");
    N = count_lines(file);
    /* reset the file caret to read the lines again */
    fseek(file, 0, SEEK_SET);
    line = (char*)calloc_and_check(len, sizeof(char));
    datapoints = (Datapoint*)calloc_and_check(N, sizeof(Datapoint));
    ignore=getline(&line, &len, file);
    ignore=ignore;
    dp_size = get_dp_len(line);
    datapoints[0] = read_line_to_vector(line);
    while(getline(&line, &len, file) != -1)
    {
        datapoints[i++] = read_line_to_vector(line);
    }

    fclose(file);
    free(line);
    return datapoints;
}


int main(int argc, char* argv[]){
    Matrix m;
    Diagonalization diagonlization;
    Datapoint* datapoints;
    char *goal, *file_name;
    argc=argc;

    goal = argv[1];
    file_name = argv[2];
    datapoints = read_datapoints(file_name);
    K=N;
    if(!strcmp(goal, "jacobi"))
    {
        diagonlization = jacobi(datapoints);
        print_datapoint(diagonlization.eigenvalues, K);
        m = diagonlization.eigenvectors;
        free(diagonlization.eigenvalues);
    }
    else if(!strcmp(goal, "wam"))
        m = wam(datapoints);
    else if(!strcmp(goal, "ddg"))
        m = ddg(datapoints);
    else
        m = gl(datapoints);
    print_matrix(m);
    free_matrix(m);
    free_matrix(datapoints);
    return 0;
}
