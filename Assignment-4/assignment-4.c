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


// function to simulate first-come-first-serve algorithm
void fcfs(int num_processes, int burst_time[]){
    int current_time = 0;
    int wait_time[num_processes], turnaround_time[num_processes];
    int arrival_time;
    
    // Initialize wait and turnaround times to 0
    for (int i = 0; i < num_processes; i++) {
        wait_time[i] = 0;
        turnaround_time[i] = 0;
    }
    
    // Iterate through each process
    for (int i = 0; i < num_processes; i++) {

        scanf("P%d", &arrival_time);
        printf("\nArrival time: %d", arrival_time);

        // Calculate wait time for current process
        if (current_time > arrival_time[i]) {
            wait_time[i] = current_time - arrival_time[i];
        }
        
        // Execute current process
        current_time += burst_time[i];
        
        // Calculate turnaround time for current process
        turnaround_time[i] = current_time - arrival_time[i];
    }
    
    // Calculate and print average wait and turnaround times
    float avg_wait_time = 0, avg_turnaround_time = 0;
    for (int i = 0; i < num_processes; i++) {
        avg_wait_time += wait_time[i];
        avg_turnaround_time += turnaround_time[i];
    }
    avg_wait_time /= num_processes;
    avg_turnaround_time /= num_processes;
    printf("Average wait time: %.1f\n", avg_wait_time);
    printf("Average turnaround time: %.1f\n", avg_turnaround_time);
}


// MAIN METHOD
int main(int argc, char *argv[]) {

    // string var for algorithm type
    char algo;
    // int var to store quantum value for RR 
    int quantum;
    // string var to store filename
    char file;

    char allProcesses;

    // verify command line parameter input
    if (argc > 4 || argc < 3) {
        printf("-- Invalid parameters (%d) --", argv[1]);
        printf("Proper usage is ./assignment-4 [-f|-s|-r <quantum>] <Input file name>\n");
        return 1;
    }

    // read choice of scheduling algorithm from command line input
    algo = argv[1][1];
    // quantum default value is 0
    quantum = 0;
    // if RR algorithm is chosen, set quantum value according to CL input
    if (algo == 'r'){
        quantum = atoi(argv[2]);
        file = argv[3]; // if RR selected, set filename from 3rd index of argv CLI
    }
    else{
        file = argv[2]; // if RR not selected, set filename from 2nd index of argv CLI
    }

    printf("Chosen algorithm: %s \n", algo);

    // read input file
    // FILE* input_file = fopen("assignment-4-input.csv", "r");
    FILE* input_file = fopen(file, "r");
    if (!input_file) {
        printf("Could not open %s \n", file);
        return 1;
    }

    // create array of process structs
    Process processes[MAX_PROCESSES];
    int pCount = 0;
    char line[256];
    // parse file 

    while (fgets(line, sizeof(line), file) != NULL) {
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

    int current_time = 0;
    int completed_processes = 0;
    int total_wait_time = 0;
    int total_turnaround_time = 0;

    // apply chosen scheduling algorithm to file input

    while (completed_processes < atoi(argv[1])) {
        int selected_pid = -1;

        if (strcmp(algo, "-f") == 0) {
            for (int i = 0; i < atoi(argv[1]); i++) {
                if (processes[i].arrival_time <= current_time) {
                    selected_pid = i;
                    break;
                }
            }
        }
        printf("checkpoint 1 passed\n");
    }
    
    // Calculate statistics
    int total_wait_time = 0;
    int total_turnaround_time = 0;
    for (int i = 0; i < num_processes; i++) {
        total_wait_time += processes[i].wait_time;
        total_turnaround_time += processes[i].turnaround_time;
    }
    double avg_wait_time = (double)total_wait_time / num_processes;
    double avg_turnaround_time = (double)total_turnaround_time / num_processes;

    // Print statistics
    printf("Average wait time: %.1f\n", avg_wait_time);
    printf("Average turnaround time: %.1f\n", avg_turnaround_time);


}



