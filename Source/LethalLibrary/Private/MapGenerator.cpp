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


// Called when the game starts
void UMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMapGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

