// Fill out your copyright notice in the Description page of Project Settings.

#include "GDESSYT/Public/ForceActor_GDESS.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AForceActor_GDESS::AForceActor_GDESS()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForceMesh"));
	MeshComp->SetupAttachment(RootComponent);
	
	ForceStrength = 1000.0f;
}

// Called when the game starts or when spawned
void AForceActor_GDESS::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AForceActor_GDESS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Up = MeshComp->GetUpVector();
	MeshComp->AddForce(Up * ForceStrength * MeshComp->GetMass());
}

