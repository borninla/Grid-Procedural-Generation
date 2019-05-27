#include <cassert>
#include <string>
#include <unordered_set>

#include "Tile.h"
#include "Coord2D.h"

class Grid2D {
	public:
		Grid2D(Coord2D dimensions);
		Grid2D(Grid2D other);
		~Grid2D();

		std::string toString();
		void setTile(Tile.TileType t, Coord2D location);
		Tile getTile(Coord2D location);
		void assertBounds(Coord2D location);
		bool checkBounds(Coord2D location);
		Coord2D getGridDimensions();
		int size();
		char getChar(Coord2D location);
		bool canGoUp(Coord2D location);
		bool canGoDown(Coord2D location);
		bool canGoLeft(Coord2D location);
		bool canGoRight(Coord2D location);
		Tile getUp(Coord2D fromHere);
		Tile getDown(Coord2D fromHere);
		Tile getLeft(Coord2D fromHere);
		Tile getRight(Coord2D fromHere);
		void markLine(Coord2D point1, Coord2D point2, bool mark);
		void setTypeLine(Coord2D point1, Coord2D point2, Tile.TileType type, bool prioritize);
		void setTypeRect(Coord2D lowerLeft, Coord2D upperRight, Tile.TileType type, bool prioritize);
		void setTypeLine(Coord2D point1, Coord2D point2, Tile.TileType type, int layers, bool prioritize);
		void markRect(Coord2D lowerLeft, Coord2D upperRight, bool mark);
		std::unordered_set<Tile> getTraversableNeighbors(Coord2D location);

		begin();
		end();
		next();
	private:	
		private const int ROWS;
		private const int COLS;
		protected Tile[][] grid;
}

Grid2D::Grid2D(Coord2D dimensions) {
	assert(dimensions.getX() >= 1);
	assert(dimensions.getY() >= 1);

	ROWS = dimensions.getY();
	COLS = dimensions.getX();

	grid = new Tile[ROWS][COLS];

	for (int thisRow = 0; thisRow < ROWS; thisRow++) {
		for (int thisCol = 0; thisCol < COLS; thisCol++) {
				setTile(Tile.TileType.EMPTY, new Coord2D(thisCol, thisRow));
		}
	}
}

Grid2D::Grid2D(Grid2D other) {
	ROWS = other.ROWS;
	COLS = other.COLS;

	grid = new Tile[ROWS][COLS];

	for (int thisRow = 0; thisRow < ROWS; thisRow++) {
		for(int thisCol = 0; thisCol < COLS; thisCol++) {
			Coord2D thisCoord2D = new Coord2D(thisCol, thisRow);
			Tile this_otherTile = other.getTile(thisCoord2D);
			setTile(this_otherTile.getType(), thisCoord2D);
		}
	}
}

Grid2D::~Grid2D() {
    for(int i = 0; i < ROWS; ++i)
        delete[] grid[i];   

    delete[] grid;
}

std::string Grid2D::toString() {
	std::string sb;
	sb.capacity((ROWS + 2) * (COLS + 3) + 1);

	// Top row border
	sb.append('*');
	for (int i = 0; i < COLS; i++)
		sb.append('-');
	sb.append('*');
	sb.append('\n');

	//Actual grid
	for (int thisRow = ROWS - 1; thisRow >= 0; thisRow--) {
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
	}

	// Bottom row border
	sb.append('*');
	for (int i = 0; i < COLS; i++)
		sb.append('-');
	sb.append('*');
	sb.append('\n');

	return sb;
}

void setTile(Tile.TileType t, Coord2D location) {
	assertBounds(location);

	grid[location.getY()][location.getX()] = new Tile(t, new Coord2D(location));
}

Tile getTile(Coord2D location) {
	assertBounds(location);

	return grid[location.getY()][location.getX()];
}

void assertBounds(Coord2D location) {
	assert(checkBounds(location));
}

bool checkBounds(Coord2D location) {
	int x = location.getX();
	int y = location.getY();

	// Make sure they aren't negative
	if (x < 0 || y < 0) return false;

	return x < COLS && y < ROWS;
}

Coord2D getGridDimensions() {
	return new Coord2D(COLS, ROWS);
}

int size() {
	return ROWS * COLS;
}

char getChar(Coord2D location) {
	return getTile(location).getChar();
}

bool canGoUp(Coord2D location) {
	return location.getY() < ROWS - 1;
}

bool canGoDown(Coord2D location) {
	return location.getY() > 0;
}

bool canGoLeft(Coord2D location) {
	return location.getX() > 0;
}

bool canGoRight(Coord2D location) {
	return location.getX() < COLS - 1;
}

Tile getUp(Coord2D fromHere) {
	if (!canGoUp(fromHere)) return null;

	return getTile(new Coord2D(fromHere.getX(), fromHere.getY() + 1));
}

Tile getDown(Coord2D fromHere) {
	if (!canGoDown(fromHere)) return null;

	return getTile(new Coord2D(fromHere.getX(), fromHere.getY() - 1));
}

Tile getLeft(Coord2D fromHere) {
	if (!canGoLeft(fromHere)) return null;

	return getTile(new Coord2D(fromHere.getX() - 1, fromHere.getY()));
}

Tile getRight(Coord2D fromHere) {
	if (!canGoRight(fromHere)) return null;

	return getTile(new Coord2D(fromHere.getX() + 1, fromHere.getY()));
}

/**
 * Mark a STRAIGHT line on this grid between the two points.
 * Points must be within bounds of this grid.
 * @param point1 an endpoint
 * @param point2 another endpoint
 */
void markLine(Coord2D point1, Coord2D point2, bool mark) {
	assertBounds(point1);
	assertBounds(point2);

	assert(point1.getX() == point2.getX() || point1.getY() == point2.getY());

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

void setTypeLine(Coord2D point1, Coord2D point2, Tile.TileType type, bool prioritize) {
	assertBounds(point1);
	assertBounds(point2);

	assert(point1.getX() == point2.getX() || point1.getY() == point2.getY());

	if (point1.equals(point2)) {
		Tile t = getTile(point1);
		if (prioritize || t.getType() == Tile.TileType.EMPTY)
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

			if (prioritize || thisTile.getType() == Tile.TileType.EMPTY)
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

		if (prioritize || thisTile.getType() == Tile.TileType.EMPTY)
			thisTile.setType(type);
		}
	}
}

void setTypeRect(Coord2D lowerLeft, Coord2D upperRight, Tile.TileType type, bool prioritize) {
	assertBounds(lowerLeft);
	assertBounds(upperRight);

	assert(lowerLeft.getX() <= upperRight.getX());
	assert(lowerLeft.getY() <= upperRight.getY());

	if (lowerLeft.getX() == upperRight.getX() || lowerLeft.getY() == upperRight.getY()) {
		setTypeLine(lowerLeft, upperRight, type, prioritize);
		return;
	}

	// If we're here, then we're marking a non-line rectangle,
	// and the arguments were provided in correct order
	for (int thisY = lowerLeft.getY(); thisY <= upperRight.getY(); thisY++) {

		// Mark row by row

		Coord2D thisRowLeft = new Coord2D(lowerLeft.getX(), thisY);
		Coord2D thisRowRight = new Coord2D(upperRight.getX(), thisY);

		setTypeLine(thisRowLeft, thisRowRight, type, prioritize);
	}
}

void setTypeLine(Coord2D point1, Coord2D point2, Tile.TileType type, int layers, bool prioritize) {
	for (int thisLevel = 0; thisLevel <= layers; thisLevel++) {

		// Row (horizontal)
		if (point1.getY() == point2.getY()) {
			// Do row on top, offset by thisLevel
			Coord2D point1Layered = new Coord2D(point1.getX(), point1.getY() + thisLevel);
			Coord2D point2Layered = new Coord2D(point2.getX(), point2.getY() + thisLevel);

			if (checkBounds(point1Layered) && checkBounds(point2Layered))
				setTypeLine(point1Layered, point2Layered, type, prioritize);

			// Do row on bot, offset by thisLevel
			point1Layered = new Coord2D(point1.getX(), point1.getY() - thisLevel);
			point2Layered = new Coord2D(point2.getX(), point2.getY() - thisLevel);

			if (checkBounds(point1Layered) && checkBounds(point2Layered))
				setTypeLine(point1Layered, point2Layered, type, prioritize);
		}

		// Col (vertical)
		else if (point1.getX() == point2.getX()) {
 
			// Do col on left, offset by thisLevel
			Coord2D point1Layered = new Coord2D(point1.getX() - thisLevel, point1.getY());
			Coord2D point2Layered = new Coord2D(point2.getX() - thisLevel, point2.getY());

			if (checkBounds(point1Layered) && checkBounds(point2Layered))
				setTypeLine(point1Layered, point2Layered, type, prioritize);

			// Do col on right, offset by thisLevel
			point1Layered = new Coord2D(point1.getX() + thisLevel, point1.getY());
			point2Layered = new Coord2D(point1.getX() + thisLevel, point2.getY());

			if (checkBounds(point1Layered) && checkBounds(point2Layered))
				setTypeLine(point1Layered, point2Layered, type, prioritize);
		}

		else
			assert(false);
	}
}

void markRect(Coord2D lowerLeft, Coord2D upperRight, bool mark) {
	assertBounds(lowerLeft);
	assertBounds(upperRight);

	assert(lowerLeft.getX() <= upperRight.getX());
	assert(lowerLeft.getY() <= upperRight.getY());

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

std::unordered_set<Tile> getTraversableNeighbors(Coord2D location) {
	std::unordered_set<Tile> neighbors;

	if (canGoUp(location)) {
		Tile upNeighbor = getUp(location);

		if (upNeighbor.getType() != Tile.TileType.NON_TRAVERSABLE)
			neighbors.insert(upNeighbor);
	}

	if (canGoDown(location)) {
		Tile downNeighbor = getDown(location);

		if (downNeighbor.getType() != Tile.TileType.NON_TRAVERSABLE)
			neighbors.insert(downNeighbor);
	}

	if (canGoLeft(location)) {
		Tile leftNeighbor = getLeft(location);

		if (leftNeighbor.getType() != Tile.TileType.NON_TRAVERSABLE)
			neighbors.insert(leftNeighbor);
	}

	if (canGoRight(location)) {
		Tile rightNeighbor = getRight(location);

		if (rightNeighbor.getType() != Tile.TileType.NON_TRAVERSABLE)
			neighbors.insert(rightNeighbor);
	}

	return neighbors;
}
