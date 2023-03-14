// MIT License
// Copyright (c) 2023 devx.3dcodekits.com
// All rights reserved.
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this softwareand associated documentation
// files(the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions :
//
// The above copyright noticeand this permission notice shall be
// included in all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#include "MyPolygonActor.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AMyPolygonActor::AMyPolygonActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	ProceduralMesh->SetupAttachment(RootComponent);

	static FString Path = TEXT("/Game/M_PolygonMaterial");
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialLoader(*Path);

	if (MaterialLoader.Succeeded())
	{
		Material = MaterialLoader.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load material at path: %s"), *Path);
	}
}

void AMyPolygonActor::BeginPlay()
{
	BuildMesh();
}

void AMyPolygonActor::BuildPolygonFilled()
{
	TArray<FVector> Vertices;
	TArray<FLinearColor> Colors;
	TArray<int32> Triangles;

	FVector Point, RotationDir{0}; // Rotation Vector
	FVector V0, LastV0;
	int32 I0, LastI0 = 0;
	float Theta;

	// Center point
	I0 = Vertices.Add(FVector::ZeroVector);
	Colors.Add(Color);

	for (uint32 i = 0; i <= Side; ++i)
	{
		Theta = FMath::DegreesToRadians(i * 360.0f / Side);
		FMath::SinCos<double>(&RotationDir.Y, &RotationDir.X, Theta);

		Point = RotationDir * Radius;

		I0 = Vertices.Add(Point);

		Colors.Add(Color);

		if (i > 0)	// Add triangle indices after first index
		{
			Triangles.Append({ 0, LastI0, I0 });
		}

		LastV0 = V0;		
		LastI0 = I0;
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), Colors, TArray<FProcMeshTangent>(), false);
}

void AMyPolygonActor::BuildPolygonEmpty()
{
	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FLinearColor> Colors;
	TArray<int32> Triangles;

	FVector Point, RotationDir{ 0 }; // Rotation Vector
	FVector V0, V1;
	FVector LastV0, LastV1;
	int32 I0, I1;
	int32 LastI0, LastI1 = 0;
	float Theta;
	float HalfThickness = Thickness * 0.5f;	// Half length of thickness

	for (uint32 i = 0; i <= Side; ++i)
	{
		Theta = FMath::DegreesToRadians(i * 360.0f / Side);
		FMath::SinCos<double>(&RotationDir.Y, &RotationDir.X, Theta);

		Point = RotationDir * Radius;

		V0 = Point + RotationDir * HalfThickness;	// outward point from center
		V1 = Point - RotationDir * HalfThickness;	// inward point from center

		I0 = Vertices.Add(V0);
		I1 = Vertices.Add(V1);

		Normals.Add(RotationDir);
		Normals.Add(-RotationDir);

		Colors.Add(Color);
		Colors.Add(Color);

		if (i > 0)	// Add rectangle (two triangles) indices after first index
		{
			Triangles.Append({ LastI0, I0, I1, I1, LastI1, LastI0 });
		}

		LastV0 = V0;
		LastV1 = V1;

		LastI0 = I0;
		LastI1 = I1;
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, TArray<FVector2D>(), Colors, TArray<FProcMeshTangent>(), false);
}

// Called every frame
void AMyPolygonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPolygonActor::BuildMesh()
{
	if (Thickness > 0.0f)
	{
		BuildPolygonEmpty();
	}
	else
	{
		BuildPolygonFilled();
	}

	if (MaterialInstanceDynamic == nullptr)
	{
		MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(Material, this);
		ProceduralMesh->SetMaterial(0, MaterialInstanceDynamic);
	}
}

void AMyPolygonActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property) 
	{
		BuildMesh();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
