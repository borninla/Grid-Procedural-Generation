import java.util.HashSet;
import java.util.Set;

/**
 * Grid of Tile objects which can be accessed, changed, and printed.
 * @author Ryan Villena
 *
 */
public class Grid2D {
    
    /**
     * Constructs new Grid2D and initializes all tiles to Tile.EMPTY
     * @param dimensions X is horizontal, Y is vertical
     */
    public Grid2D(Coord2D dimensions) {
        
        assert dimensions.getX() >= 1 : " Invalid dimension " + dimensions.toString();
        assert dimensions.getY() >= 1 : " Invalid dimension " + dimensions.toString();
        
        ROWS = dimensions.getY();
        COLS = dimensions.getX();
        
        grid = new Tile[ROWS][COLS];
        
        for (int thisRow = 0; thisRow < ROWS; thisRow++) {
            for (int thisCol = 0; thisCol < COLS; thisCol++) {
                
                setTile(Tile.TileType.EMPTY, new Coord2D(thisCol, thisRow));
            }
        }
    }
    
    public Grid2D(Grid2D other) {
        
        ROWS = other.ROWS;
        COLS = other.COLS;
        
        grid = new Tile[ROWS][COLS];
        
        for (int thisRow = 0; thisRow < ROWS; thisRow++) {
            for (int thisCol = 0; thisCol < COLS; thisCol++) {
                
                Coord2D thisCoord2D = new Coord2D(thisCol, thisRow);
                Tile this_otherTile = other.getTile(thisCoord2D);
                setTile(this_otherTile.getType(), thisCoord2D);
            }
        }
    }
    
    @Override
    public String toString() {
        
        StringBuilder sb = new StringBuilder((ROWS + 2) * (COLS + 3) + 1);
        
        // Top row border
        sb.append('*');
        for (int i = 0; i < COLS; i++)
            sb.append('-');
        sb.append('*');
        sb.append('\n');
        
        // Actual grid
//        for (int thisRow = 0; thisRow < ROWS; thisRow++) {
        for (int thisRow = ROWS - 1; thisRow >= 0; thisRow-- ) {
            
            for (int thisCol = 0; thisCol < COLS; thisCol++) {
                
                // Left border
                if (thisCol == 0)
                    sb.append('|');
                
                Tile thisTile = grid[thisRow][thisCol];
                sb.append(thisTile.getChar());
                
                // Right border
                if (thisCol == COLS - 1)
                    sb.append('|');
            }
            
            sb.append('\n');
        }
        
        
        // Bottom row border
        sb.append('*');
        for (int i = 0; i < COLS; i++)
            sb.append('-');
        sb.append('*');
        sb.append('\n');
        
        
        
        return sb.toString();
    }
    
    /**
     * 
     * @param t Type of tile to be set
     * @param location Desired location
     */
    public void setTile(Tile.TileType t, Coord2D location) {
        
        assertBounds(location);
        
        grid[location.getY()][location.getX()] = new Tile(t);
    }
        
    public Tile getTile(Coord2D location) {
        
        assertBounds(location);
        
        return grid[location.getY()][location.getX()];
    }
        
    public void assertBounds(Coord2D location) {
        
        assert checkBounds(location) : " Invalid coordinate " + location.toString();
    }
    
    public boolean checkBounds(Coord2D location) {
        
        int x = location.getX();
        int y = location.getY();
        
        // Make sure they aren't negative
        if (x < 0 || y < 0) return false;
        
        return x < COLS && y < ROWS;
    }
    
    public Coord2D getGridDimensions() {
        
        return new Coord2D(COLS, ROWS);
    }
    
    public char getChar(Coord2D location) {
        
        return getTile(location).getChar();
    }

    public boolean canGoUp(Coord2D location) {
        
        return location.getY() < ROWS - 1;
    }
    
    public boolean canGoDown(Coord2D location) {
        
        return location.getY() > 0;
    }
    
    public boolean canGoLeft(Coord2D location) {
        
        return location.getX() > 0;
    }
    
    public boolean canGoRight(Coord2D location) {
        
        return location.getX() < COLS - 1;
    }
    
    public Tile getUp(Coord2D fromHere) {
        
        if (!canGoUp(fromHere)) return null;
        
        return getTile(new Coord2D(fromHere.getX(), fromHere.getY() + 1));
    }
    
    public Tile getDown(Coord2D fromHere) {
        
        if (!canGoDown(fromHere)) return null;
        
        return getTile(new Coord2D(fromHere.getX(), fromHere.getY() - 1));
    }
    
    public Tile getLeft(Coord2D fromHere) {
        
        if (!canGoLeft(fromHere)) return null;
        
        return getTile(new Coord2D(fromHere.getX() - 1, fromHere.getY()));
    }
    
    public Tile getRight(Coord2D fromHere) {
        
        if (!canGoRight(fromHere)) return null;
        
        return getTile(new Coord2D(fromHere.getX() + 1, fromHere.getY()));
    }
    
    /**
     * Mark a STRAIGHT line on this grid between the two points.
     * Points must be within bounds of this grid.
     * @param point1 an endpoint
     * @param point2 another endpoint
     */
    public void markLine(Coord2D point1, Coord2D point2, boolean mark) {
        
        assertBounds(point1);
        assertBounds(point2);
        
        assert point1.getX() == point2.getX() || point1.getY() == point2.getY()
                : " point1 and point2 must lie on a straight line";
        
        if (point1.equals(point2)) {
            Tile t = getTile(point1);
            t.setMark(mark);
            return;
        }
        
        // If on the same row
        if (point1.getY() == point2.getY()) {
            
            for (int i = 0; i < COLS; i++) {
                
                Tile thisTile = getTile(new Coord2D(i, point1.getY()));
                thisTile.setMark(mark);
            }
        }
        
        // Else, they're on the same column
        else {
            
            for (int i = 0; i < ROWS; i++) {
                
                Tile thisTile = getTile(new Coord2D(point1.getX(), i));
                thisTile.setMark(mark);
            }
        }
    }
    
    public void setTypeLine(Coord2D point1, Coord2D point2, Tile.TileType type) {
        
        assertBounds(point1);
        assertBounds(point2);
        
        assert point1.getX() == point2.getX() || point1.getY() == point2.getY()
                : " point1 " + point1.toString()
                + " and point2 " + point2.toString()
                + " must lie on a straight line";
        
        if (point1.equals(point2)) {
            Tile t = getTile(point1);
            t.setType(type);
            return;
        }
        
        // If on the same row
        if (point1.getY() == point2.getY()) {
            
            // Iterate through from least x to greatest x,
            // whichever is which
            for (int i = (point1.getX() <= point2.getX() ? point1.getX() : point2.getX());
                    i <= (point1.getX() >  point2.getX() ? point1.getX() : point2.getX());
                    i++) {
                
                Tile thisTile = getTile(new Coord2D(i, point1.getY()));
                thisTile.setType(type);
            }
        }
        
        // Else, they're on the same column
        else {
            
            // Iterate through from least y to greatest y,
            // whichever is which
            for (int i = (point1.getY() <= point2.getY() ? point1.getY() : point2.getY());
                    i <= (point1.getY() >  point2.getY() ? point1.getY() : point2.getY());
                    i++) {
                
                Tile thisTile = getTile(new Coord2D(point1.getX(), i));
                thisTile.setType(type);
            }
        }
    }

    public void setTypeRect(Coord2D lowerLeft, Coord2D upperRight, Tile.TileType type) {
        
        assertBounds(lowerLeft);
        assertBounds(upperRight);
        
        assert lowerLeft.getX() <= upperRight.getX() : " Invalid argument order";
        assert lowerLeft.getY() <= upperRight.getY() : " Invalid argument order";
        
        
        
        if (lowerLeft.getX() == upperRight.getX() || lowerLeft.getY() == upperRight.getY()) {
            setTypeLine(lowerLeft, upperRight, type);
            return;
        }
        
        // If we're here, then we're marking a non-line rectangle,
        // and the arguments were provided in correct order
        for (int thisY = lowerLeft.getY(); thisY <= upperRight.getY(); thisY++) {
            
            // Mark row by row
            
            Coord2D thisRowLeft = new Coord2D(lowerLeft.getX(), thisY);
            Coord2D thisRowRight = new Coord2D(upperRight.getX(), thisY);
            
            setTypeLine(thisRowLeft, thisRowRight, type);
        }
    }
    
    /**
     * Mark a rectangular region between the two points
     * @param lowerLeft The lower left point of the desired region
     * @param upperRight The upper right point of the desired region
     * @param mark The desired mark for all of these rectangles
     */
    public void markRect(Coord2D lowerLeft, Coord2D upperRight, boolean mark) {
        
        assertBounds(lowerLeft);
        assertBounds(upperRight);
        
        assert lowerLeft.getX() <= upperRight.getX() : " Invalid argument order";
        assert lowerLeft.getY() <= upperRight.getY() : " Invalid argument order";
        
        
        
        if (lowerLeft.getX() == upperRight.getX() || lowerLeft.getY() == upperRight.getY()) {
            markLine(lowerLeft, upperRight, mark);
            return;
        }
        
        // If we're here, then we're marking a non-line rectangle,
        // and the arguments were provided in correct order
        for (int thisY = lowerLeft.getY(); thisY <= upperRight.getY(); thisY++) {
            
            // Mark row by row
            
            Coord2D thisRowLeft = new Coord2D(lowerLeft.getX(), thisY);
            Coord2D thisRowRight = new Coord2D(upperRight.getX(), thisY);
            
            markLine(thisRowLeft, thisRowRight, mark);
        }
    }
    
    public Set<Coord2D> getNeighbors(Coord2D location) {
        
        Set<Coord2D> neighbors = new HashSet<Coord2D>();
        
        if (canGoUp(location)) {
            neighbors.add(new Coord2D(location.getX(), location.getY() + 1));
        }
        
        if (canGoDown(location)) {
            neighbors.add(new Coord2D(location.getX(), location.getY() - 1));
        }
        
        if (canGoLeft(location)) {
            neighbors.add(new Coord2D(location.getX() - 1, location.getY()));
        }
        
        if (canGoRight(location)) {
            neighbors.add(new Coord2D(location.getX() + 1, location.getY()));
        }
        
        return neighbors;
    }
    
    private final int ROWS;
    private final int COLS;
    private Tile[][] grid;
    
    
}
