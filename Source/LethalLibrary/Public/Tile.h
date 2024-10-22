// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LETHALLIBRARY_API Tile
{
public:
	FString Shorthand;
	int North;
	int East;
	int South;
	int West;

	Tile();
	Tile(int north, int east, int south, int west);
	~Tile();

	void UpdateShorthand();
};