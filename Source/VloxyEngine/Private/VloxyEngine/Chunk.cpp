// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseWrapper.h"

// Sets default values
AChunk::AChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Noise = CreateDefaultSubobject<UFastNoiseWrapper>("Noise");

	Noise->SetupFastNoise(
        EFastNoise_NoiseType::Perlin,
        1337,
        0.03f,
        EFastNoise_Interp::Quintic,
        EFastNoise_FractalType::FBM,
        5
    );
	
	Blocks = new EBlock[4096];
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
	
}
