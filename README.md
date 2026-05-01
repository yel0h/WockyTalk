# WockyTalk
WockyTalk is a multithreaded TCP chat system written in modern C++.

It consists of:
- a concurrent TCP server (Linux)
- a cross-platform command-line chat client (Linux + Windows)
  
<img width="768" height="432" alt="output" src="https://github.com/user-attachments/assets/29ad2708-ad61-4264-ad44-497859b178bc" />

## Features
- TCP networking using POSIX sockets (also Winsock for the client)
- Multithreaded architecture (thread per client)
- Producer-consumer message queue
- Custom binary protocol (length-prefixed, typed messages)
- Client identification with a handshake (nickname + fallback IDs)
- Structured logging system

## Architecture
Communication through WockyTalk follows this sequence:

Client -> ClientHandler -> MessageQueue -> Server Loop -> Broadcast
- Client
  - sends messages with an added header through the network
  - receives messages broadcasted by the server
- ClientHandler
  - handles receiving of messages per client
  - pushes parsed messages into queue
- MessageQueue
  - thread-safe queue using `std::condition_variable`
  - implements producer-consumer pattern
- Server Loop
  - processes messages from the queue
  - broadcasts messages to clients when needed
 
## Protocol
Every message is made up of its body and an added [header](common/MessageHeader.hpp) which consists of:
- size (4 bytes), the body's size in bytes
- type (4 bytes), describing the message's function - see below
- senderSocket (4 bytes), the FD associated with the client's socket (filled in by ClientHandler during message parsing)

### Message types
1. `HELLO` - sent by the client upon successfully connecting to the server. The message's body contains a nickname the user wishes to be assigned. Not broadcasted.
2. `CHAT` - sent as a regular chat message meant to be broadcasted to other clients connected to the server.

The addition of this header handles TCP stream fragmentation as well as allows for more features to be added later on without relying on simple text parsing.

## Client identification
Each client is assigned a unique ID by the server on connect. Clients optionally provide a nickname in the handshake (`HELLO` message). If none is provided, the client will instead be known as `User_<ID>`.

## Running
Building the project with CMake will result in two executables:
- **WockyTalkServer**, which runs on port 6767. Start it using:
```bash
./WockyTalkServer
```
- **WockyTalkClient**, which, by default, tries to connect to localhost on port 6767 without providing a custom username. Start it using:
```bash
./WockyTalkClient [IP] [PORT] [NICKNAME]
```
