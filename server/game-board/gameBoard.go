package gameBoard

type TURN int

const (
	RED    TURN = 1
	YELLOW TURN = -1
	NONE   TURN = 0
)

type move struct {
	row  int
	turn TURN
}

type GameBoard struct {
	movesDone      []move
	rowOccupancies [7]int
	currentTurn    TURN
	gameOver       bool

	GameBoardMap [6][7]TURN
}

// private: checkrowfull, validateboard, changeturn

func (g *GameBoard) Init() {
	g.rowOccupancies = [7]int{-1, -1, -1, -1, -1, -1, -1}
	g.currentTurn = RED
}

func (g *GameBoard) IsRedTurn() bool {
	return g.currentTurn == RED
}

func SwapTurn(t TURN) TURN {
	return t ^ 1
}

func (g *GameBoard) ChangeTurn() {
	g.currentTurn = SwapTurn(g.currentTurn)
}

func (g *GameBoard) checkRowFull(row int) bool {
	return g.rowOccupancies[row] >= 5
}

func (g *GameBoard) IsValidMove(row int) bool {
	return !(g.checkRowFull(row) || g.gameOver)

}

func (g *GameBoard) validateBoard() {
	for row := 0; row < 6; row++ {
		for col := 0; col < 4; col++ {
			player := g.GameBoardMap[col][row]
			if player != 0 &&
				player == g.GameBoardMap[col+1][row] &&
				player == g.GameBoardMap[col+2][row] &&
				player == g.GameBoardMap[col+3][row] {
				g.gameOver = true
				return
			}
		}
	}

	for col := 0; col < 7; col++ {
		for row := 0; row < 3; row++ {
			player := g.GameBoardMap[col][row]
			if player != 0 &&
				player == g.GameBoardMap[col][row+1] &&
				player == g.GameBoardMap[col][row+2] &&
				player == g.GameBoardMap[col][row+3] {
				g.gameOver = true
				return
			}
		}
	}

	for col := 0; col < 4; col++ {
		for row := 0; row < 3; row++ {
			player := g.GameBoardMap[col][row]
			if player != 0 &&
				player == g.GameBoardMap[col+1][row+1] &&
				player == g.GameBoardMap[col+2][row+2] &&
				player == g.GameBoardMap[col+3][row+3] {
				g.gameOver = true
				return
			}
		}
	}

	for col := 3; col < 7; col++ {
		for row := 0; row < 3; row++ {
			player := g.GameBoardMap[col][row]
			if player != 0 &&
				player == g.GameBoardMap[col-1][row+1] &&
				player == g.GameBoardMap[col-2][row+2] &&
				player == g.GameBoardMap[col-3][row+3] {
				g.gameOver = true
				return
			}
		}
	}
}

func (g *GameBoard) MakeMove(row int) {
	if g.IsValidMove(row) {
		return
	}

	g.movesDone = append(g.movesDone, move{row, g.currentTurn})
	g.rowOccupancies[row]++
	g.GameBoardMap[row][g.rowOccupancies[row]] = g.currentTurn
	g.ChangeTurn()
	g.validateBoard()
}

func (g *GameBoard) UndoMove() {
	lastMove := g.movesDone[len(g.movesDone)-1]
	g.movesDone = g.movesDone[:len(g.movesDone)-1]
	g.ChangeTurn()

	g.GameBoardMap[lastMove.row][g.rowOccupancies[lastMove.row]] = NONE
	g.gameOver = false
	g.rowOccupancies[lastMove.row]--
}

func (g *GameBoard) GetCurrentturn() TURN {
	return g.currentTurn
}

func (g *GameBoard) IsGameOver() bool {
	return g.gameOver
}
