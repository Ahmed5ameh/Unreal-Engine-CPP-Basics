// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "CharacterBase_GDESS.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UCameraShakeBase;
class AInteractableBase;
class UCurveFloat;


UCLASS()
class GDESSYT_API ACharacterBase_GDESS : public ACharacter
{
	GENERATED_BODY()


	
public:
	// Sets default values for this character's properties
	ACharacterBase_GDESS();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player");
	UStaticMeshComponent* MeshComp;

	/** Instantiating objects during runtime*/
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AInteractableBase> ActorToSpawn;

	/** Stretch Animation*/
	UPROPERTY(EditAnywhere, Category="Timeline")
	UCurveFloat* CurveFloat;


	
protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);
	void InteractPressed();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera");
	float BaseTurnAtRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera");
	float BaseLookUpAtRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction");
	float TraceDistance;

	UFUNCTION(BlueprintNativeEvent)
	void TraceForward();
	void TraceForward_Implementation();

	/** OnTriggerEnter()*/
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	/** Impulse */
	UPROPERTY(EditAnywhere) float ImpulseForce;
	void FireForward();

	/** Radial Impulse */
	UPROPERTY(EditAnywhere) bool bApplyRadialImpulse;
	UPROPERTY(EditAnywhere) float ImpactRadius;
	UPROPERTY(EditAnywhere) float RadialImpactForce;
	TArray<FHitResult> HitActors;

	/** Camera Shake */
	UPROPERTY(EditAnywhere) TSubclassOf<UCameraShakeBase> CamShake;

	/** Instantiating objects during runtime*/
	UFUNCTION()
	void SpawnObject_AKA_Instantiate(FVector Location, FRotator Rotation);


	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Stretch Animation*/
	virtual void Landed(const FHitResult& Hit) override;

	
	
private:
	AActor* FocusedActor;

	/** Stretch Animation*/
	FVector StartScale;
	FVector TargetScale;
	FTimeline SquashTimeline;
	UFUNCTION()
	void SquashProgress(float Value);
};
