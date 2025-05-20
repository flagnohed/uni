package se.liu.ida.fella149.tddd78.tetris;


public class BoardToTextConverter
{

    public static String convertToText(Board board) {
	StringBuilder builder = new StringBuilder();
        for (int i = 0; i < board.getHeight(); i++) {
            for (int j = 0; j < board.getWidth(); j++) {
                //switch(board.getSquareAt(j, i)) {
		switch(board.getSquareAt(j, i)) {
		    case I:
		        builder.append("#");
		        break;
		    case EMPTY:
		        builder.append("-");
		        break;
		    case J:
		        builder.append("%");
		        break;
		    case L:
		        builder.append("@");
		        break;
		    case O:
		        builder.append("=");
		        break;
		    case S:
		        builder.append("&");
		        break;
		    case T:
		        builder.append("€");
		        break;
		    case Z:
		        builder.append("+");
		        break;
		}
	    }
            builder.append("\n");
	}
        return builder.toString();

    }
}
