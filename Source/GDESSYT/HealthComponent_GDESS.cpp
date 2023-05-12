// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent_GDESS.h"

// Sets default values for this component's properties
UHealthComponent_GDESS::UHealthComponent_GDESS()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
	Health = MaxHealth;
}


// Called when the game starts
void UHealthComponent_GDESS::BeginPlay()
{
	Super::BeginPlay();

	if(AActor* Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent_GDESS::TakeDamage);
	}
	
}



// // Called every frame
// void UHealthComponent_GDESS::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	// ...
// }


void UHealthComponent_GDESS::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0)	return;

	Health = FMath::Clamp(Health-Damage, 0.0f, MaxHealth);
}

