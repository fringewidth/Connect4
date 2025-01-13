package main

import (
	gameBoard "Connect4Server/game-board"
	"fmt"
	"net/http"
	"time"

	"github.com/gorilla/websocket"
)

type Message struct {
	LastMove  int   `json:"lastMove"`
	Timestamp int64 `json:"timestamp"`
}

var upgrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool { return true },
}

func main() {
	http.HandleFunc("/", handleFunc)
	fmt.Println("Server starting on port 8080...")
	if err := http.ListenAndServe(":8080", nil); err != nil {
		fmt.Println("Error starting server:", err)
	}
}

func closeConnection(conn *websocket.Conn) {
	fmt.Println("Closed connection.")
	conn.Close()
}

func handleFunc(w http.ResponseWriter, r *http.Request) {
	fmt.Println("Waiting for client connection")
	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		fmt.Println("Error upgrading connection:", err)
		return
	}
	defer closeConnection(conn)

	fmt.Println("Client connected")
	var gb gameBoard.GameBoard
	gb.Init()

	for {
		fmt.Println("Awaiting user message...")
		var userMove Message
		if err := conn.ReadJSON(&userMove); err != nil {
			fmt.Println("Error while reading user JSON.")
		}
		fmt.Printf("User move received: %d\n", userMove.LastMove)
		gb.MakeMove(userMove.LastMove)
		fmt.Println("Thinking...")
		move := askBot(&gb)
		timestamp := time.Now().UnixMilli()

		returnMessage := Message{LastMove: move, Timestamp: timestamp}

		fmt.Printf("Move: %d, Timestamp: %d\n", move, timestamp)
		if err := conn.WriteJSON(returnMessage); err != nil {
			if websocket.IsCloseError(err, websocket.CloseNormalClosure) {
				fmt.Println("Connection closed by user.")
				return
			}
			fmt.Println("error while closing connection.")
			return
		}

	}
}
