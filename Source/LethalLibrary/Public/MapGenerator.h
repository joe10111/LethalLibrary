// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "Cell.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapGenerator.generated.h"

USTRUCT()
struct FCell2DArray {
	GENERATED_BODY()
public:

	TArray<Cell> Cells;

	Cell operator[] (int i) {
		return Cells[i];
	}

	void Add(Cell cell) {
		Cells.Add(cell);
	}

	void Update(Cell cell) {
		Cells[cell.Col] = cell;
	}

	Cell* GetCellPointer(int i) {
		return &Cells[i];
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETHALLIBRARY_API UMapGenerator : public UActorComponent
{
	GENERATED_BODY()

public:
	TArray<FCell2DArray> CellGrid;

	// Sets default values for this component's properties
	UMapGenerator();

	UFUNCTION(BlueprintCallable)
	FString TestFunction();

	UFUNCTION(BlueprintCallable)
	void GenerateNewGrid(int rows, int cols);

	UFUNCTION(BlueprintCallable)
	FString GetGridAsString();

private:
	int NumRows;
	int NumColumns;

	TArray<Tile> GetTileOptions();
	Cell Collapse(Cell cell);
	void UpdateCellInGrid(Cell cell);
	void UpdateSurroundingOptions(Cell cell, TArray<Tile> tileOptions);
};
