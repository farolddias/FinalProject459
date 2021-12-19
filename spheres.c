#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//reading input files
int main(int argc, char* argv[])
{
//Reading sphere file 
    char *sph = argv[1];
    FILE *fptr2 = fopen(sph, "r");
    double *B;

    if (fptr2 == NULL){
        printf("No file");
        exit(1);
    }

    char *line_sph = NULL;  //string_which_stores_line
    size_t read_sph = 0;    //variable_that_stores_max_size_of_line
    int c=0;
    int sph_num;
    int j=0;

    while ((read_sph = getline(&line_sph, &read_sph, fptr2))!= -1) {
        c++;
        if (c == 3){
            sph_num = atoi(line_sph);
            printf("Number of spheres: %d\n", sph_num);
        }

        B = (double*) malloc(sph_num * 3 * sizeof(double)); //creating array with size 

        if(c >= 4){
            char* token_s = strtok(line_sph, ",");
            while (token_s != NULL) {
                char* end;
                double d2 = strtod(token_s, &end);
                B[j] = d2;
                printf("%f\n", B[j]);
                token_s = strtok(NULL, ",");
            }
        }
    }

    free(B);
    return 0;
}