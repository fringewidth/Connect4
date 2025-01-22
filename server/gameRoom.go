package main

import gameBoard "Connect4Server/game-board"

func createGame(c1, c2 *Client) {
	defer c1.closeConnection()
	defer c2.closeConnection()

	c1.setHumanOpponent(c2)
	c2.setHumanOpponent(c1)

	c1.sendOutcome(ACCEPT, 0)
	c2.sendOutcome(ACCEPT, 1)

	var gb gameBoard.GameBoard
	gb.Init()

	for !gb.IsGameOver() {
		currentMove := c1.getMove()
		gb.MakeMove(currentMove)
		c2.sendMove(currentMove)
		c1, c2 = c2, c1
	}
}
