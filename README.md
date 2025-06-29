# IRC Server - 42 Project

A complete IRC (Internet Relay Chat) server implementation in C++ that follows the RFC 1459 protocol specification. This server handles multiple client connections, channel management, and real-time messaging.

## 🚀 Features

- **Multi-client Support**: Handles multiple simultaneous client connections using `poll()`
- **Channel Management**: Create, join, and manage IRC channels with various modes
- **User Authentication**: Password-based server authentication
- **Real-time Messaging**: Private messages and channel broadcasts
- **Channel Modes**: Invite-only, password-protected, user limits, topic restrictions
- **Admin Controls**: Channel operators with kick, invite, and mode management capabilities
- **Signal Handling**: Graceful shutdown with SIGINT, SIGQUIT, and SIGPIPE handling

## 🏗️ Architecture


### Server Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   IRC Client    │    │   IRC Client    │    │   IRC Client    │
│   (nc/HexChat)  │    │   (nc/HexChat)  │    │   (nc/HexChat)  │
└─────────┬───────┘    └─────────┬───────┘    └─────────┬───────┘
          │                      │                      │
          └──────────────────────┼──────────────────────┘
                                 │
                    ┌─────────────▼─────────────┐
                    │      IRC Server           │
                    │  ┌─────────────────────┐  │
                    │  │   Main Server Loop  │  │
                    │  │   (poll-based)      │  │
                    │  └─────────────────────┘  │
                    │  ┌─────────────────────┐  │
                    │  │   Command Parser    │  │
                    │  └─────────────────────┘  │
                    │  ┌─────────────────────┐  │
                    │  │   Channel Manager   │  │
                    │  └─────────────────────┘  │
                    │  ┌─────────────────────┐  │
                    │  │   Client Manager    │  │
                    │  └─────────────────────┘  │
                    └───────────────────────────┘
```

## 🔧 Connection Handling Details

### 1. TCP Protocol Overview

The server uses TCP (Transmission Control Protocol) for reliable, ordered communication:

```
┌─────────────┐    TCP Handshake   ┌─────────────┐
│   Client    │◄──────────────────►│   Server    │
│             │   SYN, SYN-ACK,    │             │
│             │      ACK           │             │
└─────────────┘                    └─────────────┘
       │                                   │
       │        Data Transfer              │
       │◄─────────────────────────────────►│
       │    (IRC Commands/Responses)       │
       │                                   │
       │        Connection Close           │
       │◄─────────────────────────────────►│
```

**TCP Features Used:**
- **Reliable Delivery**: All IRC messages arrive in order
- **Connection-oriented**: Persistent connections for real-time chat
- **Full-duplex**: Bidirectional communication (client ↔ server)
- **Error Detection**: Automatic retransmission of lost packets

### 2. Server Initialization

The server uses a non-blocking socket with `poll()` for efficient I/O multiplexing:

```cpp
// Socket creation and setup
server_socket = socket(AF_INET, SOCK_STREAM, 0);
fcntl(server_socket, F_SETFL, O_NONBLOCK);  // Non-blocking mode
bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
listen(server_socket, SOMAXCONN);
```

### 3. Command Processing Pipeline

```
Raw Data → Buffer Split → Command Parsing → Handler Execution → Response
    ↓           ↓              ↓                ↓              ↓
  recv()    SplitBuffer()   Parser.cpp    ACommands.cpp   sendResponse()
```

## 📁 Project Structure

```
42.irc/
├── main.cpp                 # Entry point
├── Makefile                 # Build configuration
├── includes/
│   ├── Libraries.hpp        # Common includes and macros
│   ├── Server.hpp          # Server class definition
│   ├── Client.hpp          # Client class definition
│   ├── Channel.hpp         # Channel class definition
│   ├── ACommands.hpp       # Command interface
│   └── Parser.hpp          # Command parser
├── src/
│   ├── Server.cpp          # Server implementation
│   ├── Client.cpp          # Client implementation
│   ├── Channel.cpp         # Channel implementation
│   ├── Parser.cpp          # Command parsing logic
│   └── cmds/               # Command implementations
│       ├── Join.cpp        # Channel joining
│       ├── Privmsg.cpp     # Private messaging
│       ├── Kick.cpp        # User kicking
│       ├── Mode.cpp        # Channel modes
│       ├── Nick.cpp        # Nickname changes
│       ├── Pass.cpp        # Password authentication
│       ├── User.cpp        # User registration
│       └── ...             # Other commands
```

## 🛠️ Building and Running

### Prerequisites
- C++98 compatible compiler (g++)
- Linux/Unix system
- netcat (for testing)

### Build
```bash
make
```

### Run
```bash
./ircserv <port> <password>
```

Example:
```bash
./ircserv 6667 mypassword
```

### Test with netcat
```bash
nc localhost 6667
```

## 📋 Supported Commands

| Command | Description | Example |
|---------|-------------|---------|
| `PASS` | Server password authentication | `PASS mypassword` |
| `NICK` | Set nickname | `NICK alice` |
| `USER` | Register username | `USER alice 0 * :Alice Smith` |
| `JOIN` | Join/create channel | `JOIN #general` |
| `PRIVMSG` | Send private message | `PRIVMSG #general :Hello!` |
| `KICK` | Kick user from channel | `KICK #general bob :Bad behavior` |
| `MODE` | Set channel modes | `MODE #general +i` |
| `TOPIC` | Set channel topic | `TOPIC #general :Welcome to general chat` |
| `INVITE` | Invite user to channel | `INVITE bob #general` |
| `PART` | Leave channel | `PART #general` |
| `QUIT` | Disconnect from server | `QUIT :Goodbye!` |

## 🔒 Channel Modes

- **+i**: Invite-only channel
- **+k**: Password-protected channel
- **+l**: User limit
- **+t**: Topic restriction (only ops can change)

## 🧪 Testing

### Basic Connection Test
```bash
# Terminal 1: Start server
./ircserv 6667 testpass

# Terminal 2: Connect with netcat
nc localhost 6667
PASS testpass
NICK alice
USER alice 0 * :Alice Smith
```

### Channel Operations Test
```bash
# Join a channel
JOIN #test

# Send a message
PRIVMSG #test :Hello everyone!

# Set channel mode
MODE #test +i
```

## 🚨 Error Handling

The server implements comprehensive error handling:

- **Connection errors**: Graceful client disconnection
- **Protocol errors**: Proper IRC error responses
- **Signal handling**: Clean shutdown on SIGINT/SIGQUIT
- **Memory management**: Proper cleanup of resources

## 📚 Protocol Compliance

This implementation follows RFC 1459 IRC protocol specifications:

- Proper message formatting with CRLF line endings
- Standard IRC numeric replies
- Channel naming conventions (#channel)
- User registration flow (PASS → NICK → USER)
