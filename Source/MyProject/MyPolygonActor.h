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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPolygonActor.generated.h"

UCLASS()
class MYPROJECT_API AMyPolygonActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPolygonActor();

	void BuildMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void BuildPolygonFilled();
	void BuildPolygonEmpty();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "PolygonActor")
	class UProceduralMeshComponent* ProceduralMesh{ nullptr };

	UPROPERTY(EditAnywhere)
		double Radius{ 1000.0f };

	UPROPERTY(EditAnywhere)
		uint32 Side { 32 };

	UPROPERTY(EditAnywhere)
		float Thickness{ 1.0f };

	UPROPERTY(EditAnywhere)
		FLinearColor Color { 1.0f, 0.0f, 0.0f, 1.0f };

	UMaterial* Material{ nullptr };

	UMaterialInstanceDynamic* MaterialInstanceDynamic{ nullptr };
};
