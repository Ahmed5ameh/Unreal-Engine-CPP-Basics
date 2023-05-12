// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent_GDESS.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GDESSYT_API UHealthComponent_GDESS : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent_GDESS();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly);
	float Health;
	
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage,
		const class UDamageType* DamageType, class AController* InstigatedBy,
		AActor* DamageCauser);
	
public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
