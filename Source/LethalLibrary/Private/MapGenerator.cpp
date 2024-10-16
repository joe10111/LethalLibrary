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

	for (int row = 0; row < rows; row++) 
	{
		CellGrid.Add(FCell2DArray());
		for (int col = 0; col < cols; col++) 
		{
			Cell cell = Cell();
			cell.Row = row;
			cell.Col = col;

			CellGrid[row].Add(cell);
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
			gridAsString += FString::FromInt(row) + "," + FString::FromInt(col);
			gridAsString += "]";
		}
		gridAsString += "\n";
	}

	return gridAsString;
}
