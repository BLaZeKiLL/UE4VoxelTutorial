// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

enum EBlock;
class UProceduralMeshComponent;
class UFastNoiseWrapper;

UCLASS()
class AChunk final : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UProceduralMeshComponent* Mesh;
	UFastNoiseWrapper* Noise;

	EBlock* Blocks;

	TArray<FVector> VertexData;
	TArray<int> TriangleData;
	TArray<FVector2D> UVData;

	const int Size = 32;
	const int Scale = 1;

	const FVector BlockVertexData[8] = {
		FVector(100,100,100),
        FVector(100,0,100),
        FVector(100,0,0),
        FVector(100,100,0),
        FVector(0,0,100),
        FVector(0,100,100),
        FVector(0,100,0),
        FVector(0,0,0)
    };

	const int BlockTriangleData[24] = {
		0,1,2,3,5,0,3,6,4,5,6,7,1,4,7,2,5,4,1,0,3,2,7,6
    };
};
