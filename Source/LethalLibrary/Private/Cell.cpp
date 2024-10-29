// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

Cell::Cell()
{
	Value = Tile();
	Collapsed = false;
	Options = TArray<Tile>();
	Row = -1;
	Col = -1;
}

Cell::~Cell()
{
}
