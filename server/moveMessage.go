package main

import "time"

type MoveMessage struct {
	LastMove  int   `json:"lastMove"`
	Timestamp int64 `json:"timestamp"`
}

func (m *MoveMessage) newMessage(move int) *MoveMessage {
	return &MoveMessage{
		LastMove:  move,
		Timestamp: time.Now().UnixMilli(),
	}
}
