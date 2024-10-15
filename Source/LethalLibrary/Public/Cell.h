// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "CoreMinimal.h"

/**
 * 
 */
class LETHALLIBRARY_API Cell
{
public:
	Tile Value;
	bool Collapsed;
	TArray<Tile> Options;
	int Row;
	int Col;

	Cell();
	~Cell();
};
