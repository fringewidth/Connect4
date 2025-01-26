package main

import (
	"errors"
	"fmt"
	"io"
	"net"
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

func (c *Client) writeJSONSafe(data interface{}) error {
	err := c.conn.WriteJSON(data)
	if err == nil {
		return nil
	}
	if err == io.EOF {
		return fmt.Errorf("client closed connection. %w", err)
	}
	return fmt.Errorf("error while writing JSON: %w", err)
}

func boolToString(x bool) string {
	if x {
		return "true"
	}
	return "false"
}

func (c *Client) sendGameOver(winner bool, forfeited bool) error {
	return c.writeJSONSafe(map[string]string{"type": "gameOver", "winner": boolToString(winner), "forfeit": boolToString(forfeited)})
}

func (c *Client) sendOutcome(o Outcome, order int) error {
	switch o {
	case ACCEPT:
		return c.writeJSONSafe(map[string]string{"type": "accept", "order": fmt.Sprintf("%d", order)})
	case REJECT:
		err := c.writeJSONSafe(map[string]string{"type": "timeout"})
		c.closeConnection()
		return err
	}
	return nil
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

func (c *Client) getMove() (int, error) {
	var msg MoveMessage

	err := c.conn.SetReadDeadline(time.Now().Add(30 * time.Second))
	if err != nil {
		return -1, fmt.Errorf("failed to set read deadline: %w", err)
	}

	if err := c.conn.ReadJSON(&msg); err != nil {
		if err == io.EOF {
			return -1, fmt.Errorf("user disconnected: %w", err)
		}
		if netErr, ok := err.(net.Error); ok && netErr.Timeout() {
			return -1, fmt.Errorf("read timeout exceeded (waiting for 30 seconds): %w", err)
		}
		return -1, fmt.Errorf("error reading move: %w", err)
	}

	fmt.Println(c.uid, " got move ", msg.LastMove)
	return msg.LastMove, nil
}

func (c *Client) sendMove(move int) error {
	returnMessage := newMessage(move)
	fmt.Println(c.uid, " is sending ", move)
	if err := c.writeJSONSafe(returnMessage); err != nil {
		if websocket.IsCloseError(err, websocket.CloseNormalClosure) {
			return fmt.Errorf("connection closed by user: %w", err)
		}
		return fmt.Errorf("error while sending move: %w", err)
	}
	return nil
}

func (c *Client) closeConnection() {
	if err := c.conn.Close(); err != nil {
		if !errors.Is(err, net.ErrClosed) {
			fmt.Printf("Error closing connection for user %d: %v\n", c.uid, err)
		}
	} else {
		fmt.Printf("Connection closed successfully for user %d.\n", c.uid)
	}
}
