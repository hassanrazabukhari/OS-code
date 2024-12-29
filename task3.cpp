#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int main() {
    int pipefd[2]; // File descriptors for the pipe
    pid_t pid;
    char writeMsg[] = "Hello from parent process!";
    char readMsg[100];

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) { // Child process
        close(pipefd[1]); // Close the write end of the pipe
        read(pipefd[0], readMsg, sizeof(readMsg));
        std::cout << "Child received: " << readMsg << std::endl;
        close(pipefd[0]); // Close the read end of the pipe
    } else { // Parent process
        close(pipefd[0]); // Close the read end of the pipe
        write(pipefd[1], writeMsg, strlen(writeMsg) + 1);
        close(pipefd[1]); // Close the write end of the pipe
        wait(NULL); // Wait for the child process to finish
    }

    return 0;
    }
