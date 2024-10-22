// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"

// Sets default values for this component's properties
UMapGenerator::UMapGenerator()
{
	// Default values in case, should be overwritten
	NumRows = 5;
	NumColumns = 5;
}

FString UMapGenerator::TestFunction() 
{
	Tile testTile = Tile();
	testTile.North = 1;
	testTile.West = 1;
	testTile.UpdateShorthand();
	return testTile.Shorthand;
}

void UMapGenerator::GenerateNewGrid(int rows, int cols) 
{
	// pretty sure this does what I think it does
	CellGrid.Reset();

	if (rows > 0) 
	{
		NumRows = rows;
	}
	if (cols > 0)
	{
		NumColumns = cols;
	}

	// List of possible tiles for each cell
	TArray<Tile> tileOptions = GetTileOptions();

	// List of all cells in the grid to be sorted later
	TArray<Cell> cellsToCollapse = TArray<Cell>();

	// Create grid of empty cells
	for (int row = 0; row < rows; row++) 
	{
		// Add empty row
		CellGrid.Add(FCell2DArray());

		// Fill rows with Cells containing their row, column, and all possible tiles 
		for (int col = 0; col < cols; col++) 
		{
			Cell cell = Cell();
			cell.Row = row;
			cell.Col = col;
			cell.Options.Append(tileOptions);

			CellGrid[row].Add(cell);

			// Add cell to TArray of cells that still need to be collapsed
			cellsToCollapse.Add(cell);
		}
	}

	// Initial rules pass
	InitialTileRules(tileOptions);
	// Update cellsToCollapse
	for (int i = 0; i < cellsToCollapse.Num(); i++)
	{
		Cell cellToUpdate = cellsToCollapse[i];
		cellsToCollapse[i] = CellGrid[cellToUpdate.Row][cellToUpdate.Col];
	}

	// Collapse cells until all have been collapsed
	while (cellsToCollapse.Num() > 0)
	{
		// Sort by number of remaining options to find cell with fewest options
		cellsToCollapse.Sort(Cell::CompareOptionsCount);

		// Collapse cell
		Cell nextCell = cellsToCollapse[0];
		Cell updatedCell = Collapse(nextCell);

		// Update cell and surrounding cells
		UpdateCellInGrid(updatedCell);
		UpdateSurroundingOptions(updatedCell, tileOptions);

		// Remove collapsed cell from list of cells to be collapsed
		cellsToCollapse.RemoveAt(0);

		// Update cellsToCollapse
		for (int i = 0; i < cellsToCollapse.Num(); i++)
		{
			Cell cellToUpdate = cellsToCollapse[i];
			cellsToCollapse[i] = CellGrid[cellToUpdate.Row][cellToUpdate.Col];
		}
	}
}

// Returns string representation of the grid using each tile's shorthand (Cell.Value.Shorthand)
FString UMapGenerator::GetGridAsString() 
{
	FString gridAsString = "";

	for (int row = 0; row < NumRows; row++)
	{
		for (int col = 0; col < NumColumns; col++)
		{
			gridAsString += "[";
			gridAsString += CellGrid[row][col].Value.Shorthand;
			//gridAsString += FString::FromInt(CellGrid[row][col].Row) + "," + FString::FromInt(CellGrid[row][col].Col);
			gridAsString += "]";
		}
		gridAsString += "\n";
	}

	return gridAsString;
}

// Get TArray of all possible tiles
TArray<Tile> UMapGenerator::GetTileOptions() 
{
	TArray<Tile> tileOptions = TArray<Tile>();

	Tile empty = Tile(0, 0, 0, 0);
	Tile cross = Tile(1, 1, 1, 1);
	Tile endN = Tile(1, 0, 0, 0);
	Tile endE = Tile(0, 1, 0, 0);
	Tile endS = Tile(0, 0, 1, 0);
	Tile endW = Tile(0, 0, 0, 1);
	Tile hallwayNS = Tile(1, 0, 1, 0);
	Tile hallwayEW = Tile(0, 1, 0, 1);
	Tile elbowNE = Tile(1, 1, 0, 0);
	Tile elbowNW = Tile(1, 0, 0, 1);
	Tile elbowSE = Tile(0, 1, 1, 0);
	Tile elbowSW = Tile(0, 0, 1, 1);
	Tile tetrisN = Tile(1, 1, 0, 1);
	Tile tetrisE = Tile(1, 1, 1, 0);
	Tile tetrisS = Tile(0, 1, 1, 1);
	Tile tetrisW = Tile(1, 0, 1, 1);

	tileOptions.Add(empty);
	tileOptions.Add(cross);
	tileOptions.Add(endN);
	tileOptions.Add(endE);
	tileOptions.Add(endS);
	tileOptions.Add(endW);
	tileOptions.Add(hallwayNS);
	tileOptions.Add(hallwayEW);
	tileOptions.Add(elbowNE);
	tileOptions.Add(elbowNW);
	tileOptions.Add(elbowSE);
	tileOptions.Add(elbowSW);
	tileOptions.Add(tetrisN);
	tileOptions.Add(tetrisE);
	tileOptions.Add(tetrisS);
	tileOptions.Add(tetrisW);

	for (Tile tile : tileOptions)
	{
		tile.UpdateShorthand();
	}

	return tileOptions;
}

// Set a Cell's tile value randomly from remaining tile options
Cell UMapGenerator::Collapse(Cell cell)
{
	if (cell.Options.Num() == 0)
	{
		cell.Value.Shorthand = "Error";
		cell.Collapsed = true;
	}
	else 
	{
		cell.Value = cell.Options[FMath::RandRange(0, cell.Options.Num() - 1)];
		cell.Collapsed = true;
		cell.Value.UpdateShorthand();
	}
	return cell;
}

// Remove conflicting tile options from adjacent Cells in the grid
void UMapGenerator::UpdateSurroundingOptions(Cell cell, TArray<Tile> tileOptions)
{
	// Error Cell
	if (cell.Value.North == -1 || cell.Value.East == -1 || cell.Value.South == -1 || cell.Value.West == -1)
	{
		return;
	}

	int row = cell.Row;
	int col = cell.Col;

	// Update North
	if (row - 1 >= 0)
	{
		Cell northCell = CellGrid[row - 1][col];

		if (cell.Value.North == 1)
		{
			for (Tile tile : tileOptions)
			{
				if (tile.South != 1)
				{
					northCell.Options.Remove(tile);
				}
			}
		}
		else if (cell.Value.North == 0)
		{
			for (Tile tile : tileOptions)
			{
				if (tile.South == 1)
				{
					northCell.Options.Remove(tile);
				}
			}
		}
		UpdateCellInGrid(northCell);
	}

	// Update East
	if (col + 1 < NumColumns)
	{
		Cell eastCell = CellGrid[row][col + 1];

		if (cell.Value.East == 1)
		{
			for (Tile tile : tileOptions)
			{
				if (tile.West != 1)
				{
					eastCell.Options.Remove(tile);
				}
			}
		}
		else if (cell.Value.East == 0)
		{
			for (Tile tile : tileOptions)
			{
				if (tile.West == 1)
				{
					eastCell.Options.Remove(tile);
				}
			}
		}
		UpdateCellInGrid(eastCell);
	}

	// Update South
	if (row + 1 < NumRows)
	{
		Cell southCell = CellGrid[row + 1][col];

		if (cell.Value.South == 1)
		{
			for (Tile tile : tileOptions)
			{
				if (tile.North != 1)
				{
					southCell.Options.Remove(tile);
				}
			}
		}
		else if (cell.Value.South == 0)
		{
			for (Tile tile : tileOptions)
			{
				if (tile.North == 1)
				{
					southCell.Options.Remove(tile);
				}
			}
		}
		UpdateCellInGrid(southCell);
	}

	// Update West
	if (col - 1 >= 0)
	{
		Cell westCell = CellGrid[row][col - 1];

		if (cell.Value.West == 1)
		{
			for (Tile tile : tileOptions)
			{
				if (tile.East != 1)
				{
					westCell.Options.Remove(tile);
				}
			}
		}
		else if (cell.Value.West == 0)
		{
			for (Tile tile : tileOptions)
			{
				if (tile.East == 1)
				{
					westCell.Options.Remove(tile);
				}
			}
		}
		UpdateCellInGrid(westCell);
	}
}

// This is pretty dumb but it's the only way I got this to work right so idk
void UMapGenerator::UpdateCellInGrid(Cell cell)
{
	CellGrid[cell.Row].Update(cell);
}

// Remove Invalid tile options from entire grid once before starting collapsing
void UMapGenerator::InitialTileRules(TArray<Tile> tileOptions)
{
	for (int row = 0; row < NumRows; row++)
	{
		for (int col = 0; col < NumColumns; col++)
		{
			Cell cell = CellGrid[row][col];

			// Top Row
			if (cell.Row == 0)
			{
				for (Tile tile : tileOptions)
				{
					if (tile.North == 1)
					{
						cell.Options.Remove(tile);
					}
				}
			}
			// Bottom Row
			if (cell.Row == NumRows - 1)
			{
				for (Tile tile : tileOptions)
				{
					if (tile.South == 1)
					{
						cell.Options.Remove(tile);
					}
				}
			}
			// Left Column
			if (cell.Col == 0)
			{
				for (Tile tile : tileOptions)
				{
					if (tile.West == 1)
					{
						cell.Options.Remove(tile);
					}
				}
			}
			// Right Column
			if (cell.Col == NumColumns - 1)
			{
				for (Tile tile : tileOptions)
				{
					if (tile.East == 1)
					{
						cell.Options.Remove(tile);
					}
				}
			}

			UpdateCellInGrid(cell);
		}
	}
}
