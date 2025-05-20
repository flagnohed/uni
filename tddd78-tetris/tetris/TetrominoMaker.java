package se.liu.ida.fella149.tddd78.tetris;

public class TetrominoMaker
{
    public static int getNumberOfTypes() {
	int amount = SquareType.values().length-1;
	return amount;
    }

    public static Poly getPoly(int n) {
	SquareType[][] seq;
	Poly poly;

	switch (n) {
	    case 0:
		seq = new SquareType[4][4];
		seq[0][0] = SquareType.EMPTY;
		seq[0][1] = SquareType.EMPTY;
		seq[0][2] = SquareType.EMPTY;
		seq[0][3] = SquareType.EMPTY;

		seq[1][0] = SquareType.I;
		seq[1][1] = SquareType.I;
		seq[1][2] = SquareType.I;
		seq[1][3] = SquareType.I;

		seq[2][0] = SquareType.EMPTY;
		seq[2][1] = SquareType.EMPTY;
		seq[2][2] = SquareType.EMPTY;
		seq[2][3] = SquareType.EMPTY;

		seq[3][0] = SquareType.EMPTY;
		seq[3][1] = SquareType.EMPTY;
		seq[3][2] = SquareType.EMPTY;
		seq[3][3] = SquareType.EMPTY;

		poly = new Poly(seq);
		return poly;

	    // LÄGG TILL J
	    case 1:
	        seq = new SquareType[3][3];
		seq[0][0] = SquareType.J;
		seq[0][1] = SquareType.EMPTY;
		seq[0][2] = SquareType.EMPTY;

		seq[1][0] = SquareType.J;
		seq[1][1] = SquareType.J;
		seq[1][2] = SquareType.J;

		seq[2][0] = SquareType.EMPTY;
		seq[2][1] = SquareType.EMPTY;
		seq[2][2] = SquareType.EMPTY;
		poly = new Poly(seq);
		return poly;
	    case 2:
		seq = new SquareType[3][3];
		seq[0][0] = SquareType.EMPTY;
		seq[0][1] = SquareType.EMPTY;
		seq[0][2] = SquareType.L;

		seq[1][0] = SquareType.L;
		seq[1][1] = SquareType.L;
		seq[1][2] = SquareType.L;

		seq[2][0] = SquareType.EMPTY;
		seq[2][1] = SquareType.EMPTY;
		seq[2][2] = SquareType.EMPTY;

		poly = new Poly(seq);
		return poly;
	    case 3:
		seq = new SquareType[2][2];
		seq[0][0] = SquareType.O;
		seq[0][1] = SquareType.O;

		seq[1][0] = SquareType.O;
		seq[1][1] = SquareType.O;

		poly = new Poly(seq);
		return poly;
	    case 4:
		seq = new SquareType[3][3];
		seq[0][0] = SquareType.EMPTY;
		seq[0][1] = SquareType.S;
		seq[0][2] = SquareType.S;

		seq[1][0] = SquareType.S;
		seq[1][1] = SquareType.S;
		seq[1][2] = SquareType.EMPTY;

		seq[2][0] = SquareType.EMPTY;
		seq[2][1] = SquareType.EMPTY;
		seq[2][2] = SquareType.EMPTY;

		poly = new Poly(seq);
		return poly;
	    case 5:
		seq = new SquareType[3][3];
		seq[0][0] = SquareType.EMPTY;
		seq[0][1] = SquareType.T;
		seq[0][2] = SquareType.EMPTY;

		seq[1][0] = SquareType.T;
		seq[1][1] = SquareType.T;
		seq[1][2] = SquareType.T;

		seq[2][0] = SquareType.EMPTY;
		seq[2][1] = SquareType.EMPTY;
		seq[2][2] = SquareType.EMPTY;

		poly = new Poly(seq);
		return poly;
	    case 6:
		seq = new SquareType[3][3];
		seq[0][0] = SquareType.Z;
		seq[0][1] = SquareType.Z;
		seq[0][2] = SquareType.EMPTY;

		seq[1][0] = SquareType.EMPTY;
		seq[1][1] = SquareType.Z;
		seq[1][2] = SquareType.Z;

		seq[2][0] = SquareType.EMPTY;
		seq[2][1] = SquareType.EMPTY;
		seq[2][2] = SquareType.EMPTY;

		poly = new Poly(seq);
		return poly;
	    default:
		throw new IllegalArgumentException("Invalid index: " + n);
	}
    }
}
