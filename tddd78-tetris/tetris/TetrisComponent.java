package se.liu.ida.fella149.tddd78.tetris;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

public class TetrisComponent extends JComponent implements BoardListener
{
    public static final int SQUARE_SIZE = 30;
    private Board board;

    private class MoveBlock extends AbstractAction {
        int moveX;

        private MoveBlock(final int moveX) {
            this.moveX = moveX;
        }

        @Override public void actionPerformed(final ActionEvent e) { board.moveBlock(moveX); }
    }
    private class RotateBlock extends AbstractAction {
        boolean right;

        private RotateBlock(final boolean right) {
            this.right = right;
        }

        @Override public void actionPerformed(final ActionEvent e) { board.rotateBlock(right); }
    }

    public TetrisComponent(final Board board) {
	this.board = board;
	final InputMap in = getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
	in.put(KeyStroke.getKeyStroke("RIGHT"), "moveRight");
        in.put(KeyStroke.getKeyStroke("LEFT"), "moveLeft");
        in.put(KeyStroke.getKeyStroke("UP"), "rotateRight");
        in.put(KeyStroke.getKeyStroke("DOWN"), "rotateLeft");

	final ActionMap act = getActionMap();
	act.put("moveRight", new MoveBlock(1));
        act.put("moveLeft", new MoveBlock(-1));
        act.put("rotateRight", new RotateBlock(true));
        act.put("rotateLeft", new RotateBlock(false));
    }
    public Dimension getPreferredSize() {
        Dimension dim = new Dimension(board.getWidth()*SQUARE_SIZE,
				      board.getHeight()*SQUARE_SIZE);

        return dim;
    }

    @Override public void boardChanged() {
        repaint();
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        final Graphics2D g2d = (Graphics2D) g;
        for (int i = 0; i < board.getHeight(); i++) {
            for (int j = 0; j < board.getWidth(); j++) {
                switch (board.getSquareAt(j, i)) {
                    case EMPTY:
                        g2d.setColor(Color.BLACK);
                        g2d.fillRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        break;
                    case J:
                        g2d.setColor(Color.BLUE);
                        g2d.fillRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        g2d.setColor(Color.WHITE);
                        g2d.drawRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        break;
                    case L:
                        g2d.setColor(Color.ORANGE);
                        g2d.fillRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        g2d.setColor(Color.WHITE);
                        g2d.drawRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        break;
                    case O:
                        g2d.setColor(Color.YELLOW);
                        g2d.fillRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        g2d.setColor(Color.WHITE);
                        g2d.drawRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        break;
                    case Z:
                        g2d.setColor(Color.RED);
                        g2d.fillRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        g2d.setColor(Color.WHITE);
                        g2d.drawRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        break;
                    case T:
                        g2d.setColor(Color.MAGENTA);
                        g2d.fillRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        g2d.setColor(Color.WHITE);
                        g2d.drawRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        break;
                    case S:
                        g2d.setColor(Color.GREEN);
                        g2d.fillRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        g2d.setColor(Color.WHITE);
                        g2d.drawRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        break;
                    case I:
                        g2d.setColor(Color.PINK);
                        g2d.fillRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        g2d.setColor(Color.WHITE);
                        g2d.drawRect(j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                        break;
                }
            }

}
    }
}
