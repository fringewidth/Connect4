package gameBoard

type TURN int

const (
	RED    TURN = -1
	YELLOW TURN = 1
	NONE   TURN = 0
)

type move struct {
	col  int
	turn TURN
}

type GameBoard struct {
	movesDone      []move
	colOccupancies [7]int
	currentTurn    TURN
	gameOver       bool
	GameBoardMap   [6][7]TURN
}

func (g *GameBoard) Init() {

	g.colOccupancies = [7]int{-1, -1, -1, -1, -1, -1, -1}
	g.currentTurn = RED
}

func (g *GameBoard) Reset() {
	g.Init()
	g.movesDone = nil
	g.gameOver = false
	g.GameBoardMap = [6][7]TURN{}
}

func (g *GameBoard) IsRedTurn() bool {
	return g.currentTurn == RED
}

func SwapTurn(t TURN) TURN {
	return t * -1
}

func (g *GameBoard) ChangeTurn() {
	g.currentTurn = SwapTurn(g.currentTurn)
}

func (g *GameBoard) checkColumnFull(col int) bool {
	return g.colOccupancies[col] == 5
}

func (g *GameBoard) IsValidMove(col int) bool {
	return col >= 0 && !(g.checkColumnFull(col) || g.gameOver)
}

func (g *GameBoard) validateBoard() {

	for col := 0; col < 7; col++ {
		for row := 0; row < 3; row++ {
			player := g.GameBoardMap[row][col]
			if player != NONE && player == g.GameBoardMap[row+1][col] && player == g.GameBoardMap[row+2][col] && player == g.GameBoardMap[row+3][col] {
				g.gameOver = true
				return
			}
		}
	}

	for row := 0; row < 6; row++ {
		for col := 0; col < 4; col++ {
			player := g.GameBoardMap[row][col]
			if player != NONE && player == g.GameBoardMap[row][col+1] && player == g.GameBoardMap[row][col+2] && player == g.GameBoardMap[row][col+3] {
				g.gameOver = true
				return
			}
		}
	}

	for row := 0; row < 3; row++ {
		for col := 0; col < 4; col++ {
			player := g.GameBoardMap[row][col]
			if player != NONE && player == g.GameBoardMap[row+1][col+1] && player == g.GameBoardMap[row+2][col+2] && player == g.GameBoardMap[row+3][col+3] {
				g.gameOver = true
				return
			}
		}
	}

	for row := 0; row < 3; row++ {
		for col := 3; col < 7; col++ {
			player := g.GameBoardMap[row][col]
			if player != NONE && player == g.GameBoardMap[row+1][col-1] && player == g.GameBoardMap[row+2][col-2] && player == g.GameBoardMap[row+3][col-3] {
				g.gameOver = true
				return
			}
		}
	}

	g.gameOver = g.isDraw()
}

func (g *GameBoard) isDraw() bool {
	return len(g.movesDone) == 6*7
}

func (g *GameBoard) MakeMove(col int) {
	if !g.IsValidMove(col) {
		return
	}

	g.colOccupancies[col]++
	row := g.colOccupancies[col]

	g.movesDone = append(g.movesDone, move{col, g.currentTurn})
	g.GameBoardMap[row][col] = g.currentTurn

	g.ChangeTurn()

	g.validateBoard()

	// for i := 0; i < len(g.GameBoardMap); i++ {
	// 	fmt.Printf("%v\n", g.GameBoardMap[i])
	// }
}

func (g *GameBoard) UndoMove() {
	if len(g.movesDone) == 0 {
		return
	}

	lastMove := g.movesDone[len(g.movesDone)-1]
	g.movesDone = g.movesDone[:len(g.movesDone)-1]

	g.GameBoardMap[g.colOccupancies[lastMove.col]][lastMove.col] = NONE
	g.colOccupancies[lastMove.col]--
	g.ChangeTurn()

	g.gameOver = false
}

func (g *GameBoard) GetCurrentTurn() TURN {
	return g.currentTurn
}

func (g *GameBoard) IsGameOver() bool {
	return g.gameOver
}
