package main

import gameBoard "Connect4Server/game-board"

func createGame(currentPlayer, nextPlayer *Client) {
	defer currentPlayer.closeConnection()
	defer nextPlayer.closeConnection()

	currentPlayer.setHumanOpponent(nextPlayer)
	nextPlayer.setHumanOpponent(currentPlayer)

	var gb gameBoard.GameBoard
	gb.Init()

	for !gb.IsGameOver() {
		currentMove := currentPlayer.getMove()
		gb.MakeMove(currentMove)
		nextPlayer.sendMove(currentMove)
		currentPlayer, nextPlayer = nextPlayer, currentPlayer
	}
}
