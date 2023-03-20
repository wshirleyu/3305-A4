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
#define MAX_PROCESSES 2000

// structure to store process data
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int wait_time;
    int turnaround_time;
    int completed;
} Process;




// Function prototypes
int read_processes(Process processes[], char* filename);
void simulateFCFS();
void simulateSJF();
void simulateRR();


int main(int argc, char* argv[]) {
    
    Process processes[MAX_PROCESSES];

    printf('entered main at least...\n');
    char* algorithm="";
    char* filename="";
    int quantum = 0;

    // Check for correct number of arguments
    if (argc < 3 || argc > 4) {
        printf("Usage: %s <algorithm> <filename> [-r <quantum>]\n", argv[0]);
        printf("  algorithm: -f for FCFS, -s for SJF, -r for RR\n");
        printf("  filename: name of file containing processes\n");
        printf("  quantum (optional): time quantum for RR\n");
        return 1;
    }

    // Get algorithm parameter
    strcpy(algorithm, argv[1]);

    // Get filename parameter
    strcpy(filename, argv[2]);

    // Read processes from file
    read_processes(processes, filename);

    printf("\nread_processes ran successfully...\n");

    // Get quantum parameter if -r is specified
    if (strcmp(algorithm, "-r") == 0) {
        if (argc != 4) {
            printf("Usage: %s <algorithm> <filename> [-r <quantum>]\n", argv[0]);
            printf("  algorithm: -f for FCFS, -s for SJF, -r for RR\n");
            printf("  filename: name of file containing processes\n");
            printf("  quantum (optional): time quantum for RR\n");
            return 1;
        }
        quantum = atoi(argv[3]);
    }

    // Simulate selected algorithm
    if (strcmp(algorithm, "-f") == 0) {
        simulateFCFS();
    } else if (strcmp(algorithm, "-s") == 0) {
        // simulateSJF();
        printf("this would simulate SJF");
    } else if (strcmp(algorithm, "-r") == 0) {
        // simulateRR();
        printf("this would simulate RR");

    } else {
        printf("Invalid algorithm: %s\n", algorithm);
        return 1;
    }

    return 0;
}




// Function to read input from file and store processes in an array
int read_processes(Process processes[], char* filename){


    // Initialize variables
    int num_processes = 0;
    // Process processes[MAX_PROCESSES];
    // Process process;
    char line[256];

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }


    // Parse input file and store processes in array
    while (fgets(line, sizeof(line), filename)) {
        char* processName = strtok(line, ",");
        processes[num_processes].pid = atoi(processName);

        char* burstTime = strtok(NULL, ",");
        processes[num_processes].burst_time = atoi(burstTime);

        // arrival time is same as line read
        processes[num_processes].arrival_time = num_processes;
        processes[num_processes].remaining_time = processes[num_processes].burst_time;

        // initialize wait and turnaround times to 0
        processes[num_processes].wait_time = 0;
        processes[num_processes].turnaround_time = 0;

        // initialize completed state to 0
        processes[num_processes].turnaround_time = 0;

        //incremment number of processes
        // processes[num_processes] = process;
        num_processes++;
    }
    fclose(filename); 
    return num_processes;
}






// Function to simulate First Come First Served algorithm
void simulateFCFS(Process* processes, int num_processes) {
    int current_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    while (1) {
        int all_finished = 1;
        for (int i = 0; i < num_processes; i++) {
            if (!processes[i].completed) {
                all_finished = 0;
                if (processes[i].arrival_time <= current_time) {
                    processes[i].wait_time = current_time - processes[i].arrival_time;
                    total_waiting_time += processes[i].wait_time;
                    processes[i].turnaround_time = processes[i].wait_time + processes[i].burst_time;
                    total_turnaround_time += processes[i].turnaround_time;
                    processes[i].completed = 1;
                    printf("Process %d finished at time %d. Wait time: %d, Turnaround time: %d\n",
                            processes[i].pid, current_time, processes[i].wait_time, processes[i].turnaround_time);
                }
            }
        }
        if (all_finished) {
            break;
        }
        current_time++;
    }

    double avg_waiting_time = (double) total_waiting_time / num_processes;
    double avg_turnaround_time = (double) total_turnaround_time / num_processes;
    printf("Average waiting time: %.1f\n", avg_waiting_time);
    printf("Average turnaround time: %.1f\n", avg_turnaround_time);
}






