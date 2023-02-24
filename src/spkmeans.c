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
    /*
    int i, j;
    Cluster** cls;
    Datapoint* dps;
    DPNode* nodes[5000];
    long init[] = {2732,2975,4236,4294,4251,3087,1904,1476,304,1373,2389,4066,2406,1977,4221};
    DPNode* node;
    dps = read_datapoints("input_3.txt");
    for(i = 0; i < N; i++)
    {
        node = (DPNode*)malloc(sizeof(DPNode));
        node->value = dps[i];
        node->next = NULL;
        node->prev = NULL;
        node->cl_i = 0;
        nodes[i] = node;
    }
    K = 15;
    eps = 0;
    iter = 750;
    cls = kmeans_clustering(nodes, init);
    for(i = 0; i < K; i++)
    {
        for(j = 0; j < dp_size-1; j++)
        {
            printf("%.4f,", cls[i]->centroid[j]);
        }
        printf("%.4f\n", cls[i]->centroid[dp_size-1]);
    }

    return 0;
    */

    if(argc == 1)
    {
        goal = "wam";
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