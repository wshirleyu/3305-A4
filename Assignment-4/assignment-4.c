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
#include <limits.h>
#define MAX_PROCESSES 300
#define MAX_NAME_LENGTH 3

// structure to store process data
typedef struct {
    char name[MAX_NAME_LENGTH];
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
void simulateFCFS(Process processes[], int num_processes);
void simulateSJF(Process processes[], int num_processes);
void simulateRR(Process processes[], int num_processes, int quantum);

// main method
int main(int argc, char* argv[]) {

    char* algorithm="";
    char* filename="";
    int quantum = 0;

    // Check for correct number of arguments
    if (argc < 3 || argc > 4) {
        printf("Proper usage is ./assignment-4 [-f|-s|-r <quantum>] <Input file name>");
        return 1;
    }

    // Set algorithm parameter
    algorithm = argv[1];   

    // based on command line input, set filename parameter and quantum value if applicable
    if (strcmp(algorithm, "-r") == 0){
        quantum = atoi(argv[2]);
        filename = argv[3]; // if RR selected, set filename from 3rd index of argv CLI
    }
    else{
        filename = argv[2]; // if RR not selected, set filename from 2nd index of argv CLI

    }


    // initialize processes array 
    Process array[MAX_PROCESSES];

    // Read processes from file
    int num_processes = read_processes(array, filename);


    // Simulate selected algorithm
    if (strcmp(algorithm, "-f") == 0) {
        simulateFCFS(array, num_processes);
        // printf("this would simulate SJF");

    } else if (strcmp(algorithm, "-s") == 0) {
        simulateSJF(array, num_processes);
        // printf("this would simulate SJF");
    } else if (strcmp(algorithm, "-r") == 0) {
        simulateRR(array, num_processes, quantum);
        // printf("this would simulate RR");

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

    char line[256];

    // open file indicated via command line input
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open %s\n", filename);
        exit(1);
    }

    // Parse input file and store processes in array
    while (fgets(line, sizeof(line), file)) {
        // Process cur_process = processes[num_processes];
        Process cur_process;

        // parse through
        char* processName = strtok(line, ",");
        cur_process.pid = atoi(processName);

        char* burstTime = strtok(NULL, ",");
        int burst_time = atoi(burstTime);

   
        strncpy(cur_process.name, processName, MAX_NAME_LENGTH);

        // arrival time is same as line read
        // cur_process.pid = num_processes;
        cur_process.arrival_time = num_processes;
        cur_process.burst_time=burst_time;
        cur_process.remaining_time = burst_time;
    

        // initialize wait and turnaround times to 0
        cur_process.wait_time = 0;
        cur_process.turnaround_time = 0;

        // initialize completed state to 0
        cur_process.completed = 0;
    

        //increment number of processes
        processes[num_processes] = cur_process;
        num_processes++;

    }
    fclose(file); 

    return num_processes;

}



// Function to simulate First Come First Served algorithm
void simulateFCFS(Process* processes, int num_processes) {

    printf("First Come First Served\n");
    int current_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    // Loop through all processes
    for (int i = 0; i < num_processes; i++) {
            Process current = processes[i];
            current.wait_time = current_time - current.arrival_time;

            // Go through each process in the "queue"
            while(current.remaining_time > 0){
                current.turnaround_time = current_time - current.arrival_time;
                // Print output per tick
                printf("T%d : %s \t - Burst left \t%d, Wait time \t%d, Turnaround time \t%d\n", current_time,
                    current.name, current.remaining_time, current.wait_time, current.turnaround_time);
                current_time++;
                current.remaining_time--;
            }

            // Accumulate total wait and turnaround times
            processes[i].wait_time = current.wait_time;
            processes[i].turnaround_time = current.turnaround_time+1;
            total_waiting_time += processes[i].wait_time;
            total_turnaround_time += processes[i].turnaround_time;

    }

    // 
    double avg_waiting_time = ((double) total_waiting_time / num_processes);
    double avg_turnaround_time = (double) total_turnaround_time / num_processes;
    printf("\n\nTotal average waiting time: %.1f\n", avg_waiting_time);
    printf("Total average turnaround time: %.1f\n", avg_turnaround_time);
}


// Function to simulate Shortest Job First algorithm
void simulateSJF(Process* processes, int num_processes) {
    printf("Shortest Job First\n");
    int current_time = 0;
    int completed_processes = 0;
    int shortest_job_index = -1;
    int shortest_job_burst_time = 99999; // A very large number to start with
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    
    // Loop until all processes are completed
    while (completed_processes < num_processes) {


        // Find the process with shortest burst time
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].burst_time > 0 && processes[i].arrival_time <= current_time) {
                if (processes[i].burst_time <= shortest_job_burst_time) {
                    shortest_job_burst_time = processes[i].burst_time;
                    shortest_job_index = i;
                }
            }
        }
        
        // If a process was found, simulate its execution for one unit of time
        if (shortest_job_index != -1) {
            Process current = processes[shortest_job_index];

            processes[shortest_job_index].burst_time--;
      
            // Update wait and turnaround times for other processes
            for (int i = 0; i < num_processes; i++) {
                if (processes[i].burst_time > 0 && processes[i].arrival_time <= current_time && i != shortest_job_index) {
                    processes[i].wait_time++;
                    total_waiting_time++;
                }else if (processes[i].burst_time > 0 && processes[i].arrival_time <= current_time){
                    processes[i].turnaround_time++;
                }
            }

            // If the process has completed its burst time, update its completion time and move to the next process
            if (processes[shortest_job_index].burst_time == 0) {
                processes[shortest_job_index].turnaround_time = current_time - processes[shortest_job_index].arrival_time + 1;
                completed_processes++;
                shortest_job_burst_time = 99999;
                shortest_job_index = -1;

            }
            // Print information for each tick
            printf("T%d : %s \t - Burst left \t%d, Wait time \t%d, Turnaround time \t%d\n", current_time,
            current.name, current.burst_time, current.wait_time, current.turnaround_time);    
  
       
        }
        // Increment time
        current_time++;

    }

    // Calculate total turnaround
    for (int i=0; i<num_processes; i++){
        total_turnaround_time = total_turnaround_time + processes[i].turnaround_time;
    }

    double avg_waiting_time = (double) total_waiting_time / num_processes;
    double avg_turnaround_time = (double) total_turnaround_time / num_processes;
    printf("\n\nTotal average waiting time: %.1f", avg_waiting_time);
    printf("\nTotal average turnaround time: %.1f\n", avg_turnaround_time);

}


// Function to simulate Round Robin algorithm
void simulateRR(Process* processes, int num_processes, int quantum){
    printf("Round Robin with Quantum %d\n", quantum);
    int time = 0;
    int remaining_processes = num_processes;
    int active = -1;
    int time_quantum_left = 0;


    // Simulate until all processes have completed
    while (remaining_processes > 0) {

        // Loop through processes
        for (int i = 0; i < num_processes; i++) {

            // If just arrived, set burst time
            if (processes[i].arrival_time == time) {
                processes[i].remaining_time = processes[i].burst_time;
            }
            // Choose a new process if no process is active
            if (active == -1) {
                for (int i = 0; i < num_processes; i++) {

                    if (processes[i].burst_time > 0) {
                        active = i;
                        time_quantum_left = quantum;
                    }

                    // check for preemption
                    if(active!=1 && time_quantum_left == 0){
                        processes[active].wait_time += time - processes[active].turnaround_time;
                        active = -1;
                    }


                    // When a process is active
                    if (active !=-1){

                        // compare time remaining with quantum value
                        if (processes[active].remaining_time < quantum){
                            // execute till finished
                            for (int k=0; k<processes[active].remaining_time; k++){

                                // calculate wait
                                processes[active].turnaround_time = time - processes[active].arrival_time;
                                processes[active].wait_time = processes[active].turnaround_time - (processes[active].burst_time -processes[active].remaining_time);
                                
                                // processes[active].turnaround_time = time - processes[active].arrival_time+1;



                                // print info for each tick
                                printf("T%d : %s \t - Burst left \t%d, Wait time \t%d, Turnaround time \t%d\n", time,
                                processes[active].name, processes[active].remaining_time, processes[active].wait_time, 
                                processes[active].turnaround_time);

                                // processes[active].turnaround_time = time - processes[active].arrival_time+1;


                                processes[active].remaining_time--; // Decrement remaining burst time at each tick
                                time_quantum_left--; // Decrement time quantum remaining
                                time++; // Increment time 

                                // processes[active].turnaround_time = time - processes[active].arrival_time +1; // increment turnaround time


                            }
                            // Once executed till finished, remove from 'ready queue'
                                if (remaining_processes>0){
                                    remaining_processes--;
                                }
                                else{
                                    break;
                                }
                                // printf("\n\t remaining processes: %d", remaining_processes);

                            

                        }
                        else if (processes[active].remaining_time>=quantum){
                            // If process won't finish within this burst, execute till quantum is up
                            for (int k=0; k<quantum; k++){
                                
                                processes[active].turnaround_time = time - processes[active].arrival_time;
                                processes[active].wait_time = processes[active].turnaround_time- (processes[active].burst_time -processes[active].remaining_time);

                                
                                // Print for each tick
                                printf("T%d : %s \t - Burst left \t%d, Wait time \t%d, Turnaround time \t%d\n", time,
                                processes[active].name, processes[active].remaining_time, processes[active].wait_time, 
                                processes[active].turnaround_time);
                                


                                processes[active].remaining_time--; // Decrement remaining burst time at each tick
                                time_quantum_left--; // Decrement time quantum remaining
                                time++; // Increment time



                            }

                        }

                        // Reset active state
                        active=-1;
                        
                    }


                }
            }


        }

    }

    // Print results
    double total_wait_time = 0;
    double total_turnaround_time = 0;

    // parse through all processes to add
    for (int i = 0; i < num_processes; i++) {
        total_wait_time += processes[i].wait_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    // Calculate average wait and turnaround times
    double avg_wait_time = total_wait_time / num_processes;
    double avg_turnaround_time = total_turnaround_time / num_processes;

    printf("\nAverage wait time: %.1lf\n", avg_wait_time);
    printf("Average turnaround time: %.1lf\n", avg_turnaround_time);
}

