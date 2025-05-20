package se.liu.ida.fella149.tddd78.tetris;


import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class Board
{
    private SquareType[][] squares;
    private int width;
    private int height;
    private Random random;
    private Poly falling;
    private List<BoardListener> listeners;
    private int frameHeight = height + 4;
    private int frameWidth = width + 4;
    public boolean gameOver;


    public Board(final int height, final int width) {
	this.width = width;
	this.height = height;
	this.frameWidth = width+4;
	this.frameHeight = height+4;
	this.squares = new SquareType[frameHeight][frameWidth];
	this.random = new Random();
	this.gameOver = false;


	this.listeners = new ArrayList<>();
	for (int i = 2; i < frameHeight-2; i++) {
	    for (int j = 2; j < frameWidth-2; j++) { squares[i][j] = SquareType.EMPTY; }
	}

	for (int i = 0; i < frameHeight; i++) {
	    for (int j = 0; j < frameWidth; j++) {
		if (squares[i][j] == null) { squares[i][j] = SquareType.OUTSIDE; }
	    }
	}
    }
    public void addBoardListener(BoardListener bl) { listeners.add(bl); }

    private void notifyListeners() {
        for (BoardListener bl : listeners) { bl.boardChanged(); }
    }
    public SquareType getSquareAt(int x, int y) {
        x += 2;
	y += 2;
        if (falling == null) {
            return squares[y][x];
	}
        int startX = falling.getX();
        int startY = falling.getY();

	int endX = startX + falling.getWidth();
        int endY = startY + falling.getHeight();

        if (startX <= x && endX > x && startY <= y && endY > y) {
	    int internalX = x - startX;
	    int internalY = y - startY;  // dessa används inte förrän nu
	    if (falling.getSquareAt(internalX, internalY) == SquareType.EMPTY) {
                return squares[y][x]; // y = height, x = width

            } else { return falling.getSquareAt(internalX, internalY); }

        } else { return squares[y][x]; }
    }
    public boolean hasCollision() {
        for (int i = 0; i < falling.getHeight(); i++) {
            for (int j = 0; j < falling.getWidth(); j++) {
		if (falling.getSquareAt(j, i) != SquareType.EMPTY && getType(i + falling.getY(), j + falling.getX()) != SquareType.EMPTY) {
		    // har getType istället eftersom getSquareAt tar hänsyn till polyns position också, du vil bara ha boardens position.
		    return true;
		}
	    }
	}
        return false;
    }
    public void tick() {
	if (!gameOver) {
	    if (falling == null) {
		Random rnd = new Random();
		int indx = rnd.nextInt(TetrominoMaker.getNumberOfTypes() - 1);
		falling = TetrominoMaker.getPoly(3);
		falling.setX(width / 2);
		falling.setY(2);
		if (hasCollision()) {
		    gameOver = true;
		    fixatePoly();
		    falling = null;
		}
	    } else {

		int startY = falling.getY();
		falling.setY(startY + 1);
		if (hasCollision()) {
		    falling.setY(startY);
		    fixatePoly();
		    notifyListeners();
		    falling = null;
		    moveDownFullRow();

		}
	    }
	    notifyListeners();
	}
    }

    public void moveDownFullRow() {
        List<Integer> fullRows = new ArrayList<>();

        for (int i = 2; i < frameHeight-2; i++) {
            if (!Arrays.asList(squares[i]).contains(SquareType.EMPTY) || !Arrays.asList(squares[i]).contains(SquareType.OUTSIDE)) {
		fullRows.add(i);
		}
	    }
	int numberOfFullRows = fullRows.size();
	    for (int i : fullRows) {
		squares[i] = setEmptyRow(i);
		for (int r = i; r > 2; r--) {
		    squares[r] = squares[r-1];
		}
	    }
    }

    public boolean isEmptyRow(int r) {
        boolean empty = true;
        for (int c = 2; c < frameWidth-2; c++) {
	    if (squares[r][c] != SquareType.EMPTY) {
		empty = false;
		break;
	    }
	}
        return empty;
    }
    public SquareType[] setEmptyRow(int r) {
        for (int c = 2; c < frameWidth-2; c++) {
            squares[r][c] = SquareType.EMPTY;
	}
        return squares[r];
    }


    public void fixatePoly() {
	for (int i = 0; i < falling.getHeight(); i++) {
	    for (int j = 0; j < falling.getWidth(); j++) {
		if (falling.getSquareAt(j, i) != SquareType.EMPTY) {
		    squares[falling.getY() + i][falling.getX() + j] = falling.getSquareAt(j, i);
		}
	    }
	}
    }

    public void moveBlock(int n) {
        if (falling != null) {
	    falling.setX(falling.getX() + n);
	    if (hasCollision()) {
		falling.setX(falling.getX() - n);
	    }
	    notifyListeners();
	}
    }

    public void rotateBlock(boolean right) {
        if (falling != null) {
            Poly oldPoly = falling;
            falling = oldPoly.rotate(right);
            if (hasCollision()) {
                falling = oldPoly;
	    }
            notifyListeners();
	}
    }
    public int getWidth() { return width; }

    public int getHeight() { return height; }

    public SquareType getType(int height, int width) { return squares[height][width]; }

    public SquareType[][] replaceSquares() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
		int indx = random.nextInt(SquareType.values().length);
                squares[i][j] = SquareType.values()[indx];
	    }
	}
        notifyListeners();
        return squares;
    }

    public void setFalling(Poly falling) { this.falling = falling; }
}