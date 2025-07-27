# Connect4 Multiplayer Game

A Connect4 implementation with single-player (vs AI) and online multiplayer modes. Built with a Go WebSocket server and C++ Axmol Engine client.

## Features

**Game Modes**
- Single Player: Play against an AI bot
- Online Multiplayer: Real-time matches with automatic matchmaking
- Cross-Platform: Built with Axmol Engine

**Gameplay**
- Classic Connect4 rules (7×6 grid, connect 4 to win)
- Real-time game state synchronization
- Automatic game over detection (wins, draws, forfeits)
- Visual game board with animations
- Touch and mouse input support

**Technical**
- WebSocket-based communication
- AI opponent with minimax algorithm
- Automatic matchmaking queue
- Connection timeout handling
- Clean game state management

## Architecture

The project has two main parts:

**Server (Go)**
- WebSocket server on port 8080
- Complete Connect4 game logic with win/draw detection
- Queue-based matchmaking (3-second timeout)
- AI bot with minimax algorithm
- Connection handling and timeouts

**Client (C++ with Axmol Engine)**
- Cross-platform game engine
- Multiple screens: LoadScreen, ConnectingScreen, BotPlayerServer, ServerScreen
- WebSocket client using Boost.Beast
- Touch/mouse input with visual feedback

**Communication Protocol**
```
Client → Server: {"mode": "BOT"} or {"mode": "PERSON"}
Server → Client: {"type": "accept", "order": "0"}
Client → Server: {"lastMove": 3, "timestamp": 1234567890}
Server → Client: {"lastMove": 5, "timestamp": 1234567891}
Server → Client: {"type": "gameOver", "winner": "true", "forfeit": "false"}
```

## Getting Started

**Prerequisites**

Server:
- Go 1.23.4 or higher
- gorilla/websocket package

Client:
- C++ compiler with C++17 support
- Axmol Engine
- Boost libraries (for WebSocket client)
- Android Studio if you want to compile for Android

**Note**: Update the server IP address in `Source/WebSocketClient.h`:
```cpp
auto const SERVER_HOST = "YOUR_SERVER_IP";  // Currently set to "10.187.96.122"
auto const SERVER_PORT = "8080";
```

## How to Play

1. Connect 4 of your pieces in a row (horizontally, vertically, or diagonally)
2. Click/tap on a column to drop your piece
3. Players alternate turns (red goes first in multiplayer)
4. First player to connect 4 pieces wins
5. Game ends in a draw when the board is full with no winner

## AI Bot Implementation

The game includes two AI implementations:

**Client-side Bot (Local Play)**:
- Uses minimax algorithm with 5-move lookahead depth
- Evaluates board positions by counting consecutive piece alignments
- Scores positions using exponential weighting (100^(alignment_length-1))
- Includes alpha-beta pruning for performance optimization

**Server-side Bot (Online Play)**:
- Uses minimax algorithm with 7-move lookahead depth  
- Same evaluation function as client-side bot
- Processes moves on the server and returns responses via WebSocket

**Evaluation Function**:
- Scans board in 4 directions: horizontal, vertical, and both diagonals
- Counts consecutive pieces of the same color
- Assigns scores based on alignment length (2+ pieces)
- Differentiates between player and bot pieces for scoring

## Network Features

**Matchmaking**
- Automatic player pairing for online matches
- 3-second timeout for finding opponents
- Graceful handling of connection failures

## Development

**Server Code Structure**
```
server/
├── server.go          # Main server and WebSocket handling
├── client.go          # Client connection management
├── gameRoom.go        # Multiplayer game logic
├── botRoom.go         # Single-player bot logic
├── moveMessage.go     # Message structures
└── game-board/
    └── gameBoard.go   # Core Connect4 game logic
```

**Client Code Structure**
```
Source/
├── AppDelegate.*      # Application lifecycle
├── MainScene.*        # Base game scene class
├── GameBoard.*        # Visual game board
├── WebSocketClient.*  # Network communication
├── LoadScreen.*       # Main menu
├── ConnectingScreen.* # Connection UI
├── ServerScreen.*     # Multiplayer UI
├── BotPlayerServer.*  # Bot game UI
└── constants.h        # Game constants
```

## Known Issues

- Server IP is currently hardcoded in client
- Limited error handling for network failures

