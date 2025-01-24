package main

import (
	"fmt"
	"net/http"
	"time"

	"github.com/gorilla/websocket"
)

const (
	ModeBot    = "BOT"
	ModePerson = "PERSON"
)

type Opponent int

var upgrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool {
		return true
	},
}

var waitingQueue = make(chan *Client, 100)

func handleWaitingQueue() {
	client1 := <-waitingQueue
	select {
	case client2 := <-waitingQueue:
		go createGame(client1, client2)
		break
	case <-time.After(10 * time.Second):
		client1.sendOutcome(REJECT, -1)
	}
}

func handleFunc(w http.ResponseWriter, r *http.Request) {
	fmt.Println("Request received. Upgrading to socket...")
	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		fmt.Println("Error with upgrading.")
		return
	}

	client := &Client{}
	client.create(conn)

	var msg map[string]string

	if err := conn.ReadJSON(&msg); err != nil {
		fmt.Println("Error reading init message:", err)
		client.closeConnection()
		return
	}

	mode, ok := msg["mode"]
	if !ok || (mode != ModeBot && mode != ModePerson) {
		fmt.Println("Invalid mode in init message.")
		client.closeConnection()
		return
	}

	switch msg["mode"] {
	case ModeBot:
		go botRoom(client)
	case ModePerson:
		waitingQueue <- client
	}
}

func main() {
	go handleWaitingQueue()
	http.HandleFunc("/", handleFunc)
	fmt.Println("Server ready on localhost:8080")
	if err := http.ListenAndServe(":8080", nil); err != nil {
		fmt.Println("Scratch that. Error in starting server")
	}
}
