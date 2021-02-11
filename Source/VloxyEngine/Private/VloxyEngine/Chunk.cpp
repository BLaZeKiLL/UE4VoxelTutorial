// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"

#include <system_error>

#include "Enums.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseWrapper.h"

// Sets default values
AChunk::AChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Components
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Noise = CreateDefaultSubobject<UFastNoiseWrapper>("Noise");

	// Setup Noise
	Noise->SetupFastNoise(
        EFastNoise_NoiseType::Perlin,
        1337,
        0.03f,
        EFastNoise_Interp::Quintic,
        EFastNoise_FractalType::FBM,
        5
    );

	// Initialize Blocks
	Blocks.SetNum(Size * Size * Size);
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	GenerateBlocks();
	
	GenerateMesh();
	
	ApplyMesh();
}

void AChunk::GenerateBlocks()
{
	const auto Location = GetActorLocation();
	
	for (int x = 0; x < Size; x++)
	{
		for (int y = 0; y < Size; y++)
		{
			const float Xpos = (x * 100 + Location.X) / 100;
			const float ypos = (y * 100 + Location.Y) / 100;
			
			const int Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise2D(Xpos, ypos) + 1) * Size / 2), 0, Size);

			for (int z = 0; z < Height; z++)
			{
				Blocks[GetBlockIndex(x,y,z)] = EBlock::Stone;
			}

			for (int z = Height; z < Size; z++)
			{
				Blocks[GetBlockIndex(x,y,z)] = EBlock::Air;
			}
			
		}
	}
}


void AChunk::GenerateMesh()
{
	for (int x = 0; x < Size; x++)
	{
		for (int y = 0; y < Size; y++)
		{
			for (int z = 0; z < Size; z++)
			{
				if (Blocks[GetBlockIndex(x,y,z)] != EBlock::Air)
				{
					const auto Position = FVector(x,y,z);
					
					for (auto Direction : {EDirection::Forward, EDirection::Right, EDirection::Back, EDirection::Left, EDirection::Up, EDirection::Down})
					{
						if (Check(GetPositionInDIrection(Direction, Position)))
						{
							CreateFace(Direction, Position * 100);
						}
					}
				}
			}
		}
	}
}


void AChunk::ApplyMesh() const
{
	Mesh->CreateMeshSection(0, VertexData, TriangleData, TArray<FVector>(), UVData, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}

void AChunk::CreateFace(const EDirection Direction, const FVector Position)
{
	VertexData.Append(GetFaceVertices(Direction, Position));
	UVData.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });
	TriangleData.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
	VertexCount += 4;
}


bool AChunk::Check(const FVector Position) const
{
	if (Position.X >= Size || Position.Y >= Size || Position.X < 0 || Position.Y < 0 || Position.Z < 0) return true;
	if (Position.Z >= Size) return true;
	return Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)] == EBlock::Air;
}

TArray<FVector> AChunk::GetFaceVertices(EDirection Direction, const FVector Position) const
{
	TArray<FVector> Vertices;

	for (int i = 0; i < 4; i++)
	{
		Vertices.Add(BlockVertexData[BlockTriangleData[i + static_cast<int>(Direction) * 4]] * Scale + Position);
	}
	
	return Vertices;
}


FVector AChunk::GetPositionInDIrection(const EDirection Direction, const FVector Position) const
{
	switch (Direction)
	{
	case EDirection::Forward: return Position + FVector(1,0,0);
	case EDirection::Back: return Position + FVector(-1,0,0);
	case EDirection::Left: return Position + FVector(0,-1,0);
	case EDirection::Right: return Position + FVector(0,1,0);
	case EDirection::Up: return Position + FVector(0,0,1);
	case EDirection::Down: return Position + FVector(0,0,-1);
	default: throw std::invalid_argument("Invalid direction");
	}
}

inline int AChunk::GetBlockIndex(const int X, const int Y, const int Z) const
{
	return Z * Size * Size + Y * Size + X;
}
