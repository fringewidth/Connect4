package main

import (
	"time"
)

type MoveMessage struct {
	LastMove  int   `json:"lastMove"`
	Timestamp int64 `json:"timestamp"`
}

func newMessage(move int) MoveMessage {
	return MoveMessage{
		LastMove:  move,
		Timestamp: time.Now().UnixMilli(),
	}
}
