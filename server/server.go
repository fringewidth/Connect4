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
	for {
		client1 := <-waitingQueue
		fmt.Println("added client1 to waiting queue")
		select {
		case client2 := <-waitingQueue:
			fmt.Println("found a match!")
			client1.sendOutcome(ACCEPT, 0)
			client2.sendOutcome(ACCEPT, 1)
			go createGame(client1, client2)
		case <-time.After(10 * time.Second):
			fmt.Println("timeout!")
			client1.sendOutcome(REJECT, -1)
		}
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
		client.sendOutcome(ACCEPT, 0)
		go botRoom(client)
	case ModePerson:
		fmt.Println("Client wants a person. Adding to waiting queue.")
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
