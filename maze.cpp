//
// Created by Yusuf Pisan on 4/18/18.
//

#include "maze.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// print maze object
ostream &operator<<(ostream &out, const Maze &maze) {
  for (int row = 0; row < maze.height; ++row) {
    for (int col = 0; col < maze.width; ++col) {
      out << maze.field[row][col];
    }
    out << endl;
  }
  out << endl;
  return out;
}

// default constructor
Maze::Maze() = default;

// Load maze file from current directory
bool Maze::load(const string &fileName) {
  ifstream inFile;
  inFile.open(fileName);
  if (!inFile.is_open()) {
    cerr << "Unable to open file: " << fileName << endl;
    // next line is just to satisfy clang-tidy
    // otherwise it complains saying function can be const
    width = height = 0;
    return false;
  }
  inFile >> width >> height;
  inFile >> exitRow >> exitColumn;
  inFile >> startRow >> startColumn;
  // ignore leftover charcaters up to newline
  inFile.ignore(INT_MAX, '\n');
  string line;
  for (int row = 0; row < height; ++row) {
    getline(inFile, line);
    field.push_back(line);
  }
  return true;
}

// true if maze can be solved
bool Maze::solve() {
  posRow = startRow;
  posColumn = startColumn;
  markAsPath(startRow, startColumn);
  if (atExit(posRow, posColumn)) {
    return true;
  }
  while (!atExit(posRow, posColumn)) {
    int savePosRow = posRow;
    int savePosCol = posColumn;
    if (!goNorth()) {
      posRow = savePosRow;
      posColumn = savePosCol;
      if (!goSouth()) {
        posRow = savePosRow;
        posColumn = savePosCol;
        if (!goEast()) {
          posRow = savePosRow;
          posColumn = savePosCol;
          if (!goWest()) {
            markAsVisited(posRow, posColumn);
            return false; // no path to exit
          }
        }
      }
    }
  }
  if (atExit(posRow, posColumn)) {
    markAsPath(posRow, posColumn);
    return true;
  }
  return false; // shouldn't get to this point
}

// path to exit
string Maze::getPath() const { return path; }

// go north direction
bool Maze::goNorth() {
  bool success = false;
  if (isInside(posRow - 1, posColumn) && isClear(posRow - 1, posColumn)) {
    posRow--;
    markAsPath(posRow, posColumn);
    path.push_back('N');
    if (atExit(posRow, posColumn)) {
      success = true;
    } else {
      int savePosRow = posRow;
      int savePosCol = posColumn;
      success = goNorth();
      if (!success) {
        posRow = savePosRow;
        posColumn = savePosCol;
        success = goEast();
        if (!success) {
          posRow = savePosRow;
          posColumn = savePosCol;
          success = goWest();
          if (!success) {
            posRow = savePosRow;
            posColumn = savePosCol;
            success = goSouth();
            if (!success) {
              markAsVisited(posRow, posColumn);
              path.pop_back();
              posRow = savePosRow;
              posColumn = savePosCol;
            }
            markAsVisited(posRow, posColumn);
          }
          markAsVisited(posRow, posColumn);
        }
        markAsVisited(posRow, posColumn);
      }
      markAsVisited(posRow, posColumn);
    }
  } else { // if the point is not inside the maze and not clear
    success = false;
  }
  return success;
}

// go south direction
bool Maze::goSouth() {
  bool success = false;
  if (isInside(posRow + 1, posColumn) && isClear(posRow + 1, posColumn)) {
    posRow++;
    markAsPath(posRow, posColumn);
    path.push_back('S');
    if (atExit(posRow, posColumn)) {
      success = true;
    } else {
      int savePosRow = posRow;
      int savePosCol = posColumn;
      success = goSouth();
      if (!success) {
        posRow = savePosRow;
        posColumn = savePosCol;
        success = goWest();
        if (!success) {
          posRow = savePosRow;
          posColumn = savePosCol;
          success = goEast();
          if (!success) {
            success = goNorth();
            if (!success) {
              markAsVisited(posRow, posColumn);
              path.pop_back();
              posRow = savePosRow;
              posColumn = savePosCol;
            }
            markAsVisited(posRow, posColumn);
          }
          markAsVisited(posRow, posColumn);
        }
        markAsVisited(posRow, posColumn);
      }
      markAsVisited(posRow, posColumn);
    }
  } else {
    success = false;
  }
  return success;
}

// go east direction
bool Maze::goEast() {
  bool success = false;
  if (isInside(posRow, posColumn + 1) && isClear(posRow, posColumn + 1)) {
    posColumn++;
    markAsPath(posRow, posColumn);
    path.push_back('E');
    if (atExit(posRow, posColumn)) {
      success = true;
    } else {
      int savePosRow = posRow;
      int savePosCol = posColumn;
      success = goEast();
      if (!success) {
        posRow = savePosRow;
        posColumn = savePosCol;
        success = goNorth();
        if (!success) {
          posRow = savePosRow;
          posColumn = savePosCol;
          success = goSouth();
          if (!success) {
            success = goWest();
            if (!success) {
              markAsVisited(posRow, posColumn);
              path.pop_back();
              posRow = savePosRow;
              posColumn = savePosCol;
            }
            markAsVisited(posRow, posColumn);
          }
          markAsVisited(posRow, posColumn);
        }
        markAsVisited(posRow, posColumn);
      }
      markAsVisited(posRow, posColumn);
    }
  } else {
    success = false;
  }
  return success;
}

// go west direction
bool Maze::goWest() {
  bool success = false;
  if (isInside(posRow, posColumn - 1) && isClear(posRow, posColumn - 1)) {
    posColumn--;
    markAsPath(posRow, posColumn);
    path.push_back('W');
    if (atExit(posRow, posColumn)) {
      success = true;
    } else {
      int savePosRow = posRow;
      int savePosCol = posColumn;
      success = goWest();
      if (!success) {
        posRow = savePosRow;
        posColumn = savePosCol;
        success = goNorth();
        if (!success) {
          posRow = savePosRow;
          posColumn = savePosCol;
          success = goSouth();
          if (!success) {
            success = goEast();
            if (!success) {
              markAsVisited(posRow, posColumn);
              posRow = savePosRow;
              posColumn = savePosCol;
              path.pop_back();
            }
            markAsVisited(posRow, posColumn);
          }
          markAsVisited(posRow, posColumn);
        }
        markAsVisited(posRow, posColumn);
      }
      markAsVisited(posRow, posColumn);
    }
  } else {
    success = false;
  }
  return success;
}

// true if row, column is the exit
bool Maze::atExit(int row, int column) const {
  return row == exitRow && column == exitColumn;
}

// true if row, column is inside the maze
bool Maze::isInside(int row, int col) const {
  return row >= 0 && col >= 0 && row < field.size() && col < field[row].size();
}

// true if row, column is clear to move
bool Maze::isClear(int row, int col) const {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  return field[row][col] == CLEAR;
}

// mark location as part of the path to exit
void Maze::markAsPath(int row, int col) {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  field[row][col] = PATH;
}

// mark location as visited, not part of the path to exit
void Maze::markAsVisited(int row, int col) {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  field[row][col] = VISITED;
}
