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
    long unsigned len = 64;
    Datapoint* datapoints;
    int i = 1;

    file = fopen(file_name, "r");
    N = count_lines(fopen(file_name, "r"));
    line = (char*)calloc_and_check(len, sizeof(char));
    datapoints = (Datapoint*)calloc_and_check(N, sizeof(Datapoint));
    getline(&line, &len, file);
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
    char *goal, *file_name;

    double dp1[] = {0.0, 1.0, 1.0}, dp2[] = {1.0, 0.0, 1.0}, dp3[] = {1.0, 1.0, 0.0};
    Datapoint A[3];
    Diagonalization* diag;
    A[0] = dp1;
    A[1] = dp2;
    A[2] = dp3;
    N = 3;
    eps = 0;
    iter = 100;
    diag = jacobi(A);
    printf("%.4f, %.4f, %.4f\n\n", diag->eigenvalues[0], diag->eigenvalues[1], diag->eigenvalues[2]);
    print_matrix(diag->eigenvectors);
    return 0;

    if(argc == 1)
    {
        goal = "jacobi";
        file_name = "input_1.txt";
    }
    else
    {
        goal = argv[1];
        file_name = argv[2];
    }

    if(!strcmp(goal, "jacobi"))
    {
        print_matrix(read_datapoints(file_name));
    }
    else
    {
        Datapoint* datapoints = read_datapoints(file_name);
        if(!strcmp(goal, "wam")){
            print_matrix(wam(datapoints));
        }
        else if(!strcmp(goal, "ddg")){
            print_matrix(ddg(datapoints));
        }
        else
        {
            print_matrix(gl(datapoints));
        } 
    }
    return 0;
}