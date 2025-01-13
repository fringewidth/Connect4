package main

import (
	gameBoard "Connect4Server/game-board"
	"fmt"
	"math"
)

func Max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

func Min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func checkAlignment(gb *gameBoard.GameBoard, row int, col int, dRow int, dCol int) int {
	piece := gb.GameBoardMap[col][row]
	alignmentCount := 1

	for i := 1; i < 4; i++ {
		newRow := row + i*dRow
		newCol := col + i*dCol
		if newRow >= 0 && newRow < 6 && newCol > 0 && newCol < 7 && gb.GameBoardMap[newCol][newRow] == piece {
			alignmentCount++
		} else {
			break
		}
	}
	for i := 1; i < 4; i++ {
		newRow := row - i*dRow
		newCol := col - i*dCol
		if newRow >= 0 && newRow < 6 && newCol >= 0 && newCol < 7 && gb.GameBoardMap[newCol][newRow] == piece {
			alignmentCount++
		} else {
			break
		}
	}

	if alignmentCount >= 2 {
		return int(math.Pow(100, float64(alignmentCount-1)) * -1 * float64(piece))
		// ^ this should be variable based on user turn
	}

	return 0
}

func evaluateBoard(gb *gameBoard.GameBoard) int {
	score := 0

	for row := 0; row < 6; row++ {
		for col := 0; col < 7; col++ {
			if gb.GameBoardMap[col][row] != 0 {
				score += checkAlignment(gb, row, col, 1, 0)  // EW
				score += checkAlignment(gb, row, col, 0, 1)  // NS
				score += checkAlignment(gb, row, col, 1, 1)  // NW
				score += checkAlignment(gb, row, col, 1, -1) // NE
			}
		}
	}

	return score
}

func minimax(gb *gameBoard.GameBoard, alpha, beta, depth int, isMaximizing bool) int {
	if depth == 0 || gb.IsGameOver() {
		return evaluateBoard(gb)
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
			best_score = Max(miniMaxScore, best_score)
			alpha = Max(alpha, best_score)
		} else {
			best_score = Min(miniMaxScore, best_score)
			beta = Min(beta, best_score)
		}

		if alpha >= beta {
			break
		}
	}

	return best_score
}

func askBot(gb *gameBoard.GameBoard) int {
	fmt.Println("Asking bot")
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
	return bestMove
}
