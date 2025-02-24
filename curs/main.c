#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include "main.h"
#include "root.h"
#include "integral.h"
#include "func.h"



int main(int argc, char* argv[])
{
    
    int rez = 0;
    int print_roots = 0, print_iterations = 0, print_test_r = 0, print_test_i = 0;
    float eps1 = 0.0001;
    float eps2 = 1000;

    if(argc <= 1)
    {
        printf(OPTIONS_USAGE);
        printf(OPTIONS_HELP);
    }   
                
    while((rez = getopt(argc,argv,"hHritTe:E:")) != -1)
    {
        switch(rez)
        {
            case 'H':
            case 'h':
                printf(OPTIONS_DESCRIPTION
                    OPTIONS_r
                    OPTIONS_i
                    OPTIONS_e
                    OPTIONS_E
                    OPTIONS_t
                    OPTIONS_T
                    OPTIONS_USAGE);
                break;
            case 't':
                print_test_r = 1;
                break;
            case 'T':
                print_test_i = 1;
                break;
            case 'r':
                print_roots = 1;
                break;
            case 'i':
                print_iterations = 1;
                break;
            case 'e':
                eps1 = atof(optarg);
                break;
            case 'E':
                eps2 = atof(optarg);
                break;
            case '?':
                printf(OPTIONS_ERROR);
                return 1;
                break;
        }
    }

    printf("hello!\n");
    return 0;
}