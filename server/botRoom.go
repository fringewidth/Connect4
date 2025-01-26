package main

import (
	gameBoard "Connect4Server/game-board"
	"errors"
	"fmt"
	"io"
	"math"
)

func checkAlignment(gb *gameBoard.GameBoard, row int, col int, dRow int, dCol int) int {
	piece := gb.GameBoardMap[row][col]
	alignmentCount := 1

	for i := 1; i < 4; i++ {
		newRow := row + i*dRow
		newCol := col + i*dCol
		if newRow >= 0 && newRow < 6 && newCol >= 0 && newCol < 7 && gb.GameBoardMap[newRow][newCol] == piece {
			alignmentCount++
		} else {
			break
		}
	}
	for i := 1; i < 4; i++ {
		newRow := row - i*dRow
		newCol := col - i*dCol
		if newRow >= 0 && newRow < 6 && newCol >= 0 && newCol < 7 && gb.GameBoardMap[newRow][newCol] == piece {
			alignmentCount++
		} else {
			break
		}
	}

	if alignmentCount >= 2 {
		return int(math.Pow(100, float64(alignmentCount-1)) * float64(piece))
		// ^ this should be variable based on user turn
	}

	return 0
}

func evaluateBoard(gb *gameBoard.GameBoard) int {
	score := 0

	for row := 0; row < 6; row++ {
		for col := 0; col < 7; col++ {
			if gb.GameBoardMap[row][col] != 0 {
				score += checkAlignment(gb, row, col, 1, 0)  // EW
				score += checkAlignment(gb, row, col, 0, 1)  // NS
				score += checkAlignment(gb, row, col, 1, 1)  // NW
				score += checkAlignment(gb, row, col, 1, -1) // NE
			} else {

			}
		}
	}

	return score
}

func minimax(gb *gameBoard.GameBoard, alpha, beta, depth int, isMaximizing bool) int {
	if depth == 0 || gb.IsGameOver() {
		score := evaluateBoard(gb)
		return score
	}
	var best_score int
	if isMaximizing {
		best_score = math.MinInt
	} else {
		best_score = math.MaxInt
	}

	for col := 0; col < 7; col++ {
		if !gb.IsValidMove(col) {
			continue
		}

		gb.MakeMove(col)
		miniMaxScore := minimax(gb, alpha, beta, depth-1, !isMaximizing)
		gb.UndoMove()

		if isMaximizing {
			best_score = max(miniMaxScore, best_score)
			alpha = max(alpha, best_score)
		} else {
			best_score = min(miniMaxScore, best_score)
			beta = min(beta, best_score)
		}

		if alpha >= beta {
			break
		}
	}
	return best_score
}

func askBot(gb *gameBoard.GameBoard) int {
	bestMove := -1
	bestScore := math.MinInt
	for col := 0; col < 7; col++ {
		if !gb.IsValidMove(col) {
			continue
		}
		gb.MakeMove(col)
		miniMaxScore := minimax(gb, math.MinInt, math.MaxInt, 7, false)
		if miniMaxScore > bestScore {
			bestMove = col
			bestScore = miniMaxScore
		}
		gb.UndoMove()
	}
	fmt.Println("Printing game board")
	return bestMove
}

func botRoom(c *Client) {
	defer func() {
		if r := recover(); r != nil {
			fmt.Println("Panic recovery in botRoom.")
			if err := c.sendGameOver(true, false); err != nil {
				fmt.Printf("error while sending game over: %v", err)
			}
			c.closeConnection()
		}
	}()

	defer c.closeConnection()

	c.setBotOpponent()

	var gb gameBoard.GameBoard
	gb.Init()

	for !gb.IsGameOver() {
		lastMove, err := c.getMove()
		if err != nil {
			if errors.Is(err, io.EOF) {
				return
			}
			c.sendGameOver(false, true)
			return
		}
		fmt.Printf("Got %d from user %d\n", lastMove, c.uid)
		if !gb.IsValidMove(lastMove) {
			fmt.Println("Invalid move by user. Closing connection...")
			return
		}
		gb.MakeMove(lastMove)

		if gb.IsGameOver() {
			c.sendGameOver(true, false) // true : client won
			return
		}

		botMove := askBot(&gb)
		gb.MakeMove(botMove)
		if err = c.sendMove(botMove); err != nil {
			return
		}
		fmt.Printf("Attempting to send %d to user %d\n", botMove, c.uid)
	}
	c.sendGameOver(false, false) // server won
}
