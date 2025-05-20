package se.liu.ida.fella149.tddd78.tetris;


import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

public class TetrisViewer
{

    private JFrame frame;
    private Board board;

    public TetrisViewer(final Board board) {
	this.board = board;
    }

    private int rows;
    private int columns;

    public void show() {
	TetrisComponent tc = new TetrisComponent(board);
	final Action doOneStep = new AbstractAction() {
	    public void actionPerformed(ActionEvent e) {
	        board.addBoardListener(tc);
	        board.tick();
	    }
	};
	final Timer clockTimer = new Timer(400, doOneStep);
	clockTimer.setCoalesce(true);
	clockTimer.start();

	frame = new JFrame("Felas Tetris");
	frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
	frame.getPreferredSize();
	frame.setLayout(new BorderLayout());
	frame.add(tc, BorderLayout.CENTER);
	frame.pack();
	frame.setVisible(true);
    }

}
