package se.liu.ida.fella149.tddd78.tetris;


public class ViewerTest
{
    public static void main(String[] args) {
	Board board = new Board(20, 10);
	TetrisViewer view = new TetrisViewer(board);

	view.show();
    }

}
