// Copyright CSCE 311 2025
//

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sys/wait.h>

constexpr int BUFFER_SIZE = 256;

int main() {
    // Create two pipes: one for parent-to-child, one for child-to-parent
    int parent_to_child[2], child_to_parent[2];  // file descriptors for pipes' "handles"
    if (::pipe(parent_to_child) == -1 || ::pipe(child_to_parent) == -1) {
        ::perror("One or both pipes failed");
        return 1;
    }

    // pipe successful; fork a new process
    pid_t pid = ::fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Child process
        // Close unused pipe ends
        close(parent_to_child[1]); // Close write end of parent-to-child
        close(child_to_parent[0]); // Close read end of child-to-parent

        // Read a message from the parent
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = read(parent_to_child[0], buffer, BUFFER_SIZE - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the string
            std::cout << "[Child] Received from parent: " << buffer << std::endl;

            // Respond to the parent
            std::string response = "Hello, Parent! I got your message.";
            write(child_to_parent[1], response.c_str(), response.size());
        }

        // Close the remaining pipe ends
        close(parent_to_child[0]);
        close(child_to_parent[1]);

    } else { // Parent process
        // Close unused pipe ends
        close(parent_to_child[0]); // Close read end of parent-to-child
        close(child_to_parent[1]); // Close write end of child-to-parent

        // Send a message to the child
        std::string message = "Hello, Child!";
        write(parent_to_child[1], message.c_str(), message.size());

        // Wait for a response from the child
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = read(child_to_parent[0], buffer, BUFFER_SIZE - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the string
            std::cout << "[Parent] Received from child: " << buffer << std::endl;
        }

        // Close the remaining pipe ends
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        // Wait for the child to finish
        wait(nullptr);
    }

    return 0;
}
