package main

import (
	gameBoard "Connect4Server/game-board"
	"fmt"
)

func createGame(currentPlayer, nextPlayer *Client) {
	defer currentPlayer.closeConnection()
	defer nextPlayer.closeConnection()

	currentPlayer.setHumanOpponent(nextPlayer)
	nextPlayer.setHumanOpponent(currentPlayer)

	var gb gameBoard.GameBoard

	for !gb.IsGameOver() {
		currentMove, err := currentPlayer.getMove()
		if err != nil {
			fmt.Println(err)
			if err := nextPlayer.sendGameOver(true, true); err != nil {
				fmt.Println(err)
			}
			if err := currentPlayer.sendGameOver(false, true); err != nil {
				fmt.Printf("Error sending game over to player %d: %v\n", currentPlayer.uid, err)
			}
			return
		}
		gb.MakeMove(currentMove)
		if err := nextPlayer.sendMove(currentMove); err != nil {
			fmt.Printf("Error sending move to player %d: %v\n", nextPlayer.uid, err)
			if err := currentPlayer.sendGameOver(true, true); err != nil {
				fmt.Printf("Error sending game over to player %d: %v\n", currentPlayer.uid, err)
			}
			if err := nextPlayer.sendGameOver(true, true); err != nil {
				fmt.Printf("Error sending game over to player %d: %v\n", nextPlayer.uid, err)
			}
			return
		}
		currentPlayer, nextPlayer = nextPlayer, currentPlayer
	}

	if err := currentPlayer.sendGameOver(true, false); err != nil {
		fmt.Printf("Error sending game over to player %d: %v\n", currentPlayer.uid, err)
	}
	if err := nextPlayer.sendGameOver(false, false); err != nil {
		fmt.Printf("Error sending game over to player %d: %v\n", nextPlayer.uid, err)
	}
}
