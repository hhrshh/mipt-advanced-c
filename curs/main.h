#ifndef MAIN_H
#define MAIN_H

#define OPTIONS_USAGE "Usage: areafinder [OPTION]...\n"
#define OPTIONS_DESCRIPTION "A program to calculate the area between three functions, find their intersection points, \
and display the number of iterations for root-finding. Supports command-line options for precision and help.\n"
#define OPTIONS_HELP "options: -h, display help message.\n"
#define OPTIONS_Hh "  -h display this help text.\n"
#define OPTIONS_i "  -i the number of iterations required for t\n"
#define OPTIONS_r "  -r the abscissas of the points of intersection of the curves.\n"
#define OPTIONS_e "  -e ebs, the precision for finding the root of the equation.\n"
#define OPTIONS_E "  -E ebs, the precision for computing the integral.\n"
#define OPTIONS_t "  -t test the function root\n"
#define OPTIONS_T "  -T test the faunction integral\n"

#define OPTIONS_ERROR "Error argument: %s, try -h for help.\n", argv[optind-1]
#endif