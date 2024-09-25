## To-Do List Application with Blockchain-Like Features
##### This application is a C-based to-do list that simulates storing and managing tasks as if they were on a blockchain, ensuring data integrity using cryptographic hashing (SHA-256).

### Features
Add, update, delete, and list tasks.
Use SHA-256 hashing to secure tasks, simulating blockchain immutability.
Link tasks using their hashes to create a blockchain-like chain of tasks.
Menu-driven interface for ease of use.

### Compilation
Ensure you have OpenSSL installed, then compile the program with:
gcc -o todo_list todo_list.c -lssl -lcrypto


### Running the Program
Execute the program:
./todo_list