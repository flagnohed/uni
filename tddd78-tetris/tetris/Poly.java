package se.liu.ida.fella149.tddd78.tetris;

public class Poly
{
    public SquareType[][] squares;

    private int x;
    private int y;

    public Poly(final SquareType[][] squares) {
	this.squares = squares;
	this.x = 0;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getX() { return x; }
    public int getY() { return y; }

    public int getWidth() { return squares[0].length; }
    public int getHeight() {
        return squares.length;
    }

    public SquareType getSquareAt(int x, int y) { return squares[y][x]; }

    public void setY(int y) {
        this.y = y;
    }

    public Poly rotate(boolean right) {
        int size = getHeight();
        Poly newPoly = new Poly(new SquareType[size][size]);

        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                if (right) {
                    newPoly.squares[c][size - 1 - r] = squares[r][c];
                } else {
                    newPoly.squares[size-1-c][r] = squares[r][c];
                }
            }
        }
        newPoly.x = x;
        newPoly.y = y;
        return newPoly;
    }
}

