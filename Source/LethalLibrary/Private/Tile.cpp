// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

Tile::Tile()
{
	Shorthand = "";
	North = -1;
	East = -1;
	South = -1;
	West = -1;
}

Tile::Tile(int north, int east, int south, int west)
{
	Shorthand = "";
	North = north;
	East = east;
	South = south;
	West = west;
}

Tile::~Tile()
{
}

void Tile::UpdateShorthand() 
{
	Shorthand = "";
	if (North == 1) Shorthand += "N";
	if (East == 1) Shorthand += "E";
	if (South == 1) Shorthand += "S";
	if (West == 1) Shorthand += "W";
	if (Shorthand.Len() == 0) Shorthand = "Empty";
}
