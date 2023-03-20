/**
 * @file assignment-4.c
 * @author Shirley Wu 251082034
 * @version 0.1
 * @date 2023-03-18
 * 
 * 
 */


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_PROCESSES 1000

// structure to store process data
typedef struct{
    int pid;
    int arrival;
    int burst;
    int remaining;
    int wait;
    int turnaround;
} Process;



// MAIN METHOD
int main(int argc, char *argv[]) {
    printf("Entering main method!\n");

    // string var for algorithm type
    char* algo="";
    // int var to store quantum value for RR 
    int quantum = 0;
    // string var to store filename
    char* file="";


    char allProcesses;

    // verify command line parameter input
    if (argc > 4 || argc < 3) {
        // printf("-- Invalid parameters (%c) --", argv[1]);
        printf("Proper usage is ./assignment-4 [-f|-s|-r <quantum>] <Input file name>\n");
        return 1;
    }

    printf("\n argv1: %c", argv[1][1]);
    printf("\n argv2: %s", argv[2]);



    // read choice of scheduling algorithm from command line input
    // algo = argv[1][1];
    algo = argv[1];

    printf("\nalgorithm input: %c", algo);

    // if RR algorithm is chosen, set quantum value according to CL input
    if (algo == '-r'){
        quantum = atoi(argv[2]);
        file = argv[3]; // if RR selected, set filename from 3rd index of argv CLI
        // printf("\n Filename: %s", &file);
    }
    else{
        file = argv[2]; // if RR not selected, set filename from 2nd index of argv CLI
        // printf("\n Filename: %s", &file);

    }

    printf("\nChosen algorithm: %c \n", algo);

    // read input file
    FILE* input_file = fopen("/assignment-4-input.csv", "r");
    // FILE* input_file = fopen("assignment-4-input.csv", "r");
    printf("\n FILE OPENED");
    // FILE* input_file = fopen(&file, "r");
    if (!input_file) {
        // printf("Could not open %s \n", file);
        printf("Could not open file\n");
        return 1;
    }

    // create array of process structs
    Process processes[MAX_PROCESSES];
    int pCount = 0;
    char line[256];
    // parse file 

    while (fgets(line, sizeof(line), input_file) != NULL) {
        // Parse process number and burst time from the line
        int processNum, burstTime;
        sscanf(line, "%s,%d", &processNum, &burstTime);
        printf("%s, %d\n", processNum, burstTime);
        // Populate the process array with the parsed data
        // processes[num_processes].arrival = arrival_time;

        processes[pCount].burst = burstTime;
        processes[pCount].wait = 0;
        processes[pCount].turnaround = 0;
        processes[pCount].remaining = burstTime;

        pCount++;
        printf("\n pCount: %d", pCount);
    }

    fclose(input_file);


}



