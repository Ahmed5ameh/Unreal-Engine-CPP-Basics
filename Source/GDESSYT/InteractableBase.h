// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "MyFirstInterface_GDESS.h" //Interact Interface
#include "InteractableBase.generated.h"

class UCurveFloat;

UCLASS()
class GDESSYT_API AInteractableBase : public AActor, public IMyFirstInterface_GDESS
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

	/** Timeline */
	UFUNCTION()
	void TimelineProgress(float Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Timeline */
	FTimeline CurveTimeline;
	UPROPERTY(EditAnywhere, Category="Timeline") UCurveFloat* CurveFloat;
	UPROPERTY() FVector StartLocation;
	UPROPERTY() FVector EndLocation;
	UPROPERTY(EditAnywhere, Category="Timeline") float ZOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* caller);
	virtual void OnInteract_Implementation(AActor* Caller) override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartFocus();
	virtual void StartFocus_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndFocus();
	virtual void EndFocus_Implementation() override;
};
