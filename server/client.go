package main

import (
	"fmt"
	"time"

	"github.com/gorilla/websocket"
)

const (
	ACCEPT = iota
	REJECT
)

const (
	BOT     = -1
	PENDING = 0
)

type Outcome int

type Client struct {
	uid      int
	conn     *websocket.Conn
	opponent int
}

func (c *Client) sendGameOver(winner bool) {
	err := c.conn.WriteJSON(map[string]string{"type": "gameOver", "winner": fmt.Sprintf("%d", winner)})
	if err != nil {
		fmt.Println("Error sending gameover message.")
	}
}

func (c *Client) sendOutcome(o Outcome, order int) {
	var err error
	switch o {
	case ACCEPT:
		err = c.conn.WriteJSON(map[string]string{"type": "accept", "order": fmt.Sprintf("%d", order)})
	case REJECT:
		err = c.conn.WriteJSON(map[string]string{"type": "timeout"})
		c.closeConnection()
	}
	if err != nil {
		fmt.Println("Error sending outcome:", err)
	}
}

func (c *Client) setHumanOpponent(c2 *Client) {
	c.opponent = c2.uid
}

func (c *Client) setBotOpponent() {
	c.opponent = BOT
}

func (c *Client) create(conn *websocket.Conn) {
	c.uid = int(time.Now().UnixMilli())
	c.conn = conn
	c.opponent = PENDING
}

func (c *Client) getMove() int {
	var msg MoveMessage
	if err := c.conn.ReadJSON(&msg); err != nil || msg.LastMove < 0 || msg.LastMove > 6 {
		fmt.Println("Invalid move received by user with id:", c.uid)
		return -1
	}
	return msg.LastMove
}

func (c *Client) sendMove(move int) {
	returnMessage := newMessage(move)
	fmt.Printf("Message to send: %+v\n", returnMessage)
	if err := c.conn.WriteJSON(returnMessage); err != nil {
		if websocket.IsCloseError(err, websocket.CloseNormalClosure) {
			fmt.Println("Connection closed by user.")
			return
		}
		fmt.Println("Error while sending move:", err)
		return
	}
}

func (c *Client) closeConnection() {
	if err := c.conn.Close(); err != nil {
		fmt.Println("Error closing connection:", err)
	}
}
