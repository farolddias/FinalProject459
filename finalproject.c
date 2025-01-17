#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define NSEC_PER_SEC 1000000000

//reading input files
int main()
{
    FILE *fptr = fopen("mesh.input", "r"); //Reading mesh input file
    int firstNum;   //number of triangles 

    if (fptr == NULL){  //if there is no mesh.input file
        printf("No file");
        exit(1);
    }

    if(fptr!=NULL){
        fscanf(fptr, "%d", &firstNum);  //scans first line of mesh file and reads the number of triangles
    }

    double *triangle = (double*) malloc(firstNum * 9 * sizeof(double)); //creates array to hold triangle point coordinates

    char *line = NULL;  //string_which_stores_line
    size_t read = 0;    //variable_that_stores_max_size_of_line

    int i=0;    
    int x=0;    //counter variable
    while ((read = getline(&line, &read, fptr)) != -1) {    //while loop to read through each line of mesh input file
        x++;    //increment counter till desired line to be read
        if(x>=2){   //begins to read coordinates on second line in file 
            char* token = strtok(line, ",");    //breaks the first line of strings into a series of tokens separated by comma     
            while (token != NULL) {
                char* end;
                double d1 = strtod(token, &end);    //each token converted into double
                triangle[i] = d1;   //each triangle point coordinate in double format placed into array
                token = strtok(NULL, ",");
                i++;
            }
        }
    } 

    FILE *fptr2 = fopen("spheres.input.csv", "r");  //Reading spheres input file

    if (fptr2 == NULL){
        printf("No file");
        exit(1);
    }

    char *line_sph = NULL;  //string_which_stores_line
    size_t read_sph = 0;    //variable_that_stores_max_size_of_line
    int c=0;    //counter variable
    int sph_num=0;  //number of spheres
    double sph_r=0; //sphere radius
    int j=0;

    while ((read_sph = getline(&line_sph, &read_sph, fptr2))!= -1) {    //while loop to read through each line of sphere input file
        c++;    //increment counter till desired line to be read
        if (c == 2){
            sph_r = atof(line_sph);     //converts the sphere radius which is a string in sphere file to float      
        }
        
        if (c == 3){
            sph_num = atoi(line_sph);   //converts number of spheres string format to float
        }
    }

    double *sphere = (double*) malloc(sph_num * 3 * sizeof(double)); //create array to hold sphere center coordinates
    
    fclose(fptr2);  //close spheres input file
    FILE *fptr3 = fopen("spheres.input.csv", "r");  //open spheres input file again to read from beginning of file

    int w=0;    //counter variable
    while ((read_sph = getline(&line_sph, &read_sph, fptr3))!= -1) {    
        w++;    //increment counter till desired line to be read
        if(w >= 4){
            char* token_s = strtok(line_sph, ",");  //breaks the first line of strings into a series of tokens separated by comma
            while (token_s != NULL) {
                char* end1;
                double d2 = strtod(token_s, &end1); //each token converted into double
                sphere[j] = d2; //each sphere center coordinate in double format placed into array 
                token_s = strtok(NULL, ",");
                j++;
            }
        }
    }
    
    FILE *fp;
    fp = fopen("collision_detection.out", "w"); //open output file for writing 
    if (fp == NULL)
    {
        printf("File does not exist.\n");
    }
    fprintf(fp, "s,t\n");   //print the header file

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start); // start timing
    
    int sphere_counter=0;
    for (j = 0; j < (sph_num * 3); j+=3) {  //first for loop to run through all spheres
        int triangle_counter=0; 
        for (i = 0; i < (firstNum * 9); i+=9) { //second for loop to run through all triangles for each sphere
            double Ux=triangle[i+3]-triangle[i];    //triangle vector in x direction
            double Vx=triangle[i+6]-triangle[i];    //triangle vector in x direction
            double Uy=triangle[i+4]-triangle[i+1];  //triangle vector in y direction
            double Vy=triangle[i+7]-triangle[i+1];  //triangle vector in y direction 
            double Uz=triangle[i+5]-triangle[i+2];  //triangle vector in z direction
            double Vz=triangle[i+8]-triangle[i+2];  //triangle vector in z direction 
            double nx=(Uy*Vz)-(Uz*Vy);      // cross multiplication to obtain plane normal for x, y, and z 
            double ny=(Uz*Vx)-(Ux*Vz);
            double nz=(Ux*Vy)-(Uy*Vx);
            double ndist = sqrt((nx*nx) + (ny*ny) + (nz*nz)); //square root of sum of squares
            double Nx = nx/ndist;   //normalize vectors to normal unit vectors for x, y, and z 
            double Ny = ny/ndist;
            double Nz = nz/ndist;
            double dist = abs((sphere[j]-triangle[i])*Nx + (sphere[j+1]-triangle[i+1])*Ny + (sphere[j+2]-triangle[i+2])*Nz);    //distance vector from point on plane to center of sphere and dot product with unit normal 
            if (dist < sph_r){ // if statement to determine if collision happens
                fprintf(fp, "%d, ", sphere_counter);    //print sphere ID to output file
                fprintf(fp, "%d\n", triangle_counter);  //print triangle ID to output file
            }
            triangle_counter++; 
        }
        sphere_counter++;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);   // stop timing
    time_t elapsed_sec = end.tv_sec - start.tv_sec;
	long elapsed_nsec = end.tv_nsec - start.tv_nsec;
	double elapsed_total = elapsed_sec + (double)elapsed_nsec / (double)NSEC_PER_SEC;   //calculate the time taken for collision detection 
	printf("%g\n", elapsed_total*1000);

    fclose(fp); //close output file
    free(triangle); //deallocate the memory stored in triangle array 
    free(sphere);   //deallocate the mermoy stored in sphere array
    return 0;
}
