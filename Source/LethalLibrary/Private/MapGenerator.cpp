// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"

// Sets default values for this component's properties
UMapGenerator::UMapGenerator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	NumRows = rows;
	NumColumns = cols;

	// List of possible tiles for each cell
	TArray<Tile> tileOptions = GetTileOptions();

	// List of all cells in the grid to be sorted later
	TArray<Cell> cellsToCollapse = TArray<Cell>();

	// Create grid of empty cells
	for (int row = 0; row < rows; row++) 
	{
		CellGrid.Add(FCell2DArray());
		for (int col = 0; col < cols; col++) 
		{
			Cell cell = Cell();
			cell.Row = row;
			cell.Col = col;
			cell.Options.Append(tileOptions);

			CellGrid[row].Add(cell);
			cellsToCollapse.Add(cell);
			
			// update method attempt
			cell.Value.Shorthand = "Updated";
			UpdateCellInGrid(cell);
		}
	}

	while (cellsToCollapse.Num() > 0)
	{
		cellsToCollapse.Sort(Cell::CompareOptionsCount);
		Cell nextCell = cellsToCollapse[0];
		Cell updatedCell = Collapse(nextCell);
		UpdateCellInGrid(updatedCell);
		UpdateSurroundingOptions(updatedCell, tileOptions);
		cellsToCollapse.RemoveAt(0);

		for (int i = 0; i < cellsToCollapse.Num(); i++)
		{
			Cell cellToUpdate = cellsToCollapse[i];
			cellsToCollapse[i] = CellGrid[cellToUpdate.Row][cellToUpdate.Col];
		}
	}
}

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
