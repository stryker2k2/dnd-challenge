#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

#define TRUE 1

int is_server(const char *pid_str) {
    char path[1024];
    char name[256];
    FILE *fp;

    // Build the path to the status file
    sprintf(path, "/proc/%s/status", pid_str);

    fp = fopen(path, "r");
    if (fp == NULL) {
        perror("[!] fopen!");
        exit(1); // Directory might disappear between readdir and fopen
    }

    // Read the process name from the "status" file
    // Assumes "Name:" is the first line
    if (fgets(name, sizeof(name), fp) != NULL) {
        // Find the "Name:" part and remove newline character
        char *name_start = strstr(name, "Name:");
        if (name_start) {
            name_start += 5; // Move past "Name:"
            while (isspace(*name_start)) {
                name_start++;
            }
            char *newline = strchr(name_start, '\n');
            if (newline) {
                *newline = '\0';
            }

            if (strcmp(name_start, "defcon") == 0)
            {
                fclose(fp);
                return 1;
            }         
        }
    }
    fclose(fp);
}

int main() {
    DIR *proc_dir;
    struct dirent *entry;
    const char *full_path = "/home/jack/repo/dnd-challenge/server/output/defcon";


    while(TRUE)
    {
        int result;

        proc_dir = opendir("/proc");
        if (proc_dir == NULL) {
            perror("[!] opendir!");
            return 1;
        }

        // Iterate through all entries in the /proc directory
        while ((entry = readdir(proc_dir)) != NULL) {
            // Check if the directory name is a number (a PID)
            if (entry->d_type == DT_DIR) {
                int is_pid = 1;
                for (int i = 0; entry->d_name[i] != '\0'; i++) {
                    if (!isdigit(entry->d_name[i])) {
                        is_pid = 0;
                        break;
                    }
                }
                if (is_pid) {
                    result = is_server(entry->d_name);
                    if (result)
                    {
                        printf("PID: %s, Process Name: %s\n", entry->d_name, "defcon");
                        break;
                    }
                }
            }
        }

        if (!result)
        {
            pid_t pid;
            signal(SIGCHLD, SIG_IGN);
            pid = fork();

            if (pid == -1) 
            {
                // Handle error: fork failed
                perror("Error: fork failed");
                return 1;
            }

            if (pid == 0)
            {
                int result = execl(full_path, "service", NULL);
                if (result == -1)
                {
                    perror("execl failed");
                    exit(EXIT_FAILURE);
                }
                printf("[+] Success!");
            }

            else
            {
                printf("Parent process (PID %d) launched child (PID %d). Continuing execution...\n", getpid(), pid);
            }
        }

        closedir(proc_dir);
        proc_dir = NULL;
        entry = NULL;

        sleep(5);
    }

    return 0;
}
