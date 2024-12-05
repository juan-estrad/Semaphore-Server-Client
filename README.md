# Semaphore and Message Passing Server-Client Program

## Overview
This project enhances an OS-like platform by implementing both a **semaphore mechanism** for critical section management and a **message passing facility** for inter-process communication. The server program processes system call-like requests from multiple clients, enabling synchronization and message exchange.

---

## Objectives
1. **Simulate OS Functions**:
   - Implement a semaphore for mutual exclusion.
   - Add a message passing facility with send and receive operations.

2. **Client-Server Interaction**:
   - Clients can lock/unlock the semaphore to manage critical sections.
   - Clients can send and retrieve messages via a shared queue.

3. **Inter-Process Communication**:
   - Use FIFOs (named pipes) for communication between clients and the server.
   - Pass structured messages including message type, length, and content.

---

## Program Details

### Server Program
The server:
1. Implements:
   - A **semaphore** with wait (P) and signal (V) operations.
   - A **message queue** to handle sending and receiving messages.

2. Processes the following system calls:
   - **Semaphore Operations**:
     - **Lock Semaphore (P/Wait)**: Allows clients to enter critical sections.
     - **Unlock Semaphore (V/Signal)**: Releases the semaphore and notifies waiting clients.
   - **Message Operations**:
     - **Message Send**:
       - Clients send messages with a specified type, length, and data.
       - Messages are added to a queue in the order they are received.
     - **Message Receive**:
       - Clients request a message of a specific type.
       - The server checks the queue:
         - If a message of the requested type is found, the oldest one is removed and sent to the client.
         - If no matching message is found, the client receives a "No message available" reply.

3. Logs all activities:
   - Client ID and type of request.
   - Message details (type, length, content) for message operations.
   - Replies sent to clients.

---

### Client Program
The client:
1. Interacts with the server to:
   - Lock/unlock the semaphore for critical section execution.
   - Send and retrieve messages.

2. Offers the following functionality:
   - **Semaphore Operations**:
     - **Non-Critical Section**: Simulates processing before requesting semaphore lock.
     - **Critical Section**: Simulates processing after acquiring semaphore lock.
   - **Message Passing**:
     - **Message Send**:
       - Gather message type, length, and data from the user.
       - Send the message to the server.
     - **Message Receive**:
       - Request a message of a specific type.
       - Display the received message or notify the user if no message is available.

---

## Features

### Semaphore
- Ensures mutual exclusion for critical sections.
- Supports a queue for clients waiting to access the semaphore.

### Message Passing
- Structured messages with:
  - **Message Type**: Integer to categorize the message.
  - **Message Length**: Integer to specify data length.
  - **Data**: The actual message content.
- Handles multiple messages in a queue:
  - FIFO (First In, First Out) order within each message type.

### Logging
- Server logs each request, including client details and system call parameters.
- Logs replies sent to clients.

---

## Setup

### Prerequisites
- GCC compiler
- UNIX/Linux-based system (to support FIFOs and semaphores)

### Compilation
```bash
gcc -o server Server.c -pthread
gcc -o client Client.c
```
---

## Execution

### Start the Server:
```bash
./server
```

### Start the Client(s):
```bash
./client
```
---

## Usage

### Client Menu Options
### Semaphore Operations:
- **1**: Lock semaphore (P/Wait).
- **2**: Unlock semaphore (V/Signal).

### Message Passing:
- **6**: Send a message.
- **7**: Receive a message (specify message type).

### Exit Options:
- **0**: Exit the client.
- **-1**: Terminate the server.

---

## Message Passing Example

### Message Send:
1. **User inputs**:
   - **Message Type**: E.g., `1` for notifications.
   - **Message Length**: Number of characters.
   - **Data**: The actual message content.
2. The server adds the message to its queue.

### Message Receive:
1. **User specifies the Message Type** to retrieve.
2. The server:
   - Sends the oldest message of that type, if available.
   - Notifies the client if no message of the requested type exists.

---

## Example Workflow

### Semaphore Management

#### Non-Critical Section:
- Client processes for 20-30 iterations, printing messages to indicate it is outside the critical section.
- Sends a wait request to the server and blocks until the semaphore is available.

#### Critical Section:
- Client processes for 20-30 iterations, printing messages to indicate it is inside the critical section.
- Sends a signal request to the server and releases the semaphore.

### Message Passing
1. **Client 1 sends a message**:
   - Type: `1`, Length: `12`, Data: `"Hello World!"`.
2. **Client 2 retrieves a message of type `1`**:
   - Receives `"Hello World!"`.
3. **Client 3 requests a message of type `2`**:
   - Receives "No message available."

---

# Limitations
- Assumes unique client IDs.
- Processes requests serially, which may cause delays under heavy load.

---

## License

[MIT](https://choosealicense.com/licenses/mit/)

