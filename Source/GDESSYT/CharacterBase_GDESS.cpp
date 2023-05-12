// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase_GDESS.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "MyFirstInterface_GDESS.h"
#include "InteractableBase.h"


// Sets default values
ACharacterBase_GDESS::ACharacterBase_GDESS()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance
 	// if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase_GDESS::OnOverlapBegin);

	BaseTurnAtRate = 45.0f;
	BaseLookUpAtRate = 45.0f;
	TraceDistance = 2000.0f;
	
	ImpulseForce = 500.0f;

	bApplyRadialImpulse = true;
	ImpactRadius = 200.0f;
	RadialImpactForce = 2000.0f;

	/** Stretch Animation*/
	StartScale = FVector(1.0f,1.0f,1.0f);
	TargetScale = FVector(1.3f, 1.3f, 0.8f);
}

// // Called when the game starts or when spawned
// void ACharacterBase_GDESS::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// }


//Called every frame
void ACharacterBase_GDESS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceForward();
	SquashTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ACharacterBase_GDESS::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//Bind Actions to their corresponding user defined functions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACharacterBase_GDESS::InteractPressed);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterBase_GDESS::FireForward);

	//Bind Axis to their corresponding user defined functions
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase_GDESS::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase_GDESS::MoveRight);
	PlayerInputComponent->BindAxis("TurnAtRate", this, &ACharacterBase_GDESS::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &ACharacterBase_GDESS::LookUpAtRate);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}



#pragma region HANZO_LEARNING_UNREAL_CPP__USERDEFINED_FUNCTIONS

void ACharacterBase_GDESS::MoveForward(float Value)
{
	if(IsValid(Controller) && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACharacterBase_GDESS::MoveRight(float Value)
{
	if(IsValid(Controller) && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ACharacterBase_GDESS::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnAtRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase_GDESS::LookUpAtRate(float Value)
{
	AddControllerYawInput(Value * BaseLookUpAtRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase_GDESS::InteractPressed()
{
	TraceForward();
	if(FocusedActor)
	{
		if(IMyFirstInterface_GDESS* _interface = Cast<IMyFirstInterface_GDESS>(FocusedActor))
		{
			_interface->Execute_OnInteract(FocusedActor, this);
		}
	}
	
}

void ACharacterBase_GDESS::TraceForward_Implementation()
{
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Loc, Rot);
	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * TraceDistance);

	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);

	if(bHit)
	{
		//DrawDebugBox(GetWorld(),hit.ImpactPoint,FVector(5,5,5),FColor::Emerald,false,2.0f);
		
		if(AActor* Interactable = Hit.GetActor())
		{
			if(Interactable != FocusedActor)
			{
				if(FocusedActor)
				{
					if(IMyFirstInterface_GDESS* Interface = Cast<IMyFirstInterface_GDESS>(FocusedActor))
					{
						Interface->Execute_EndFocus(FocusedActor);
					}
				}

				if(IMyFirstInterface_GDESS* _interface = Cast<IMyFirstInterface_GDESS>(Interactable))
				{
					_interface->Execute_StartFocus(Interactable);
				}
				FocusedActor = Interactable;
			}
		}
		else
		{
			if(FocusedActor)
			{
				if(IMyFirstInterface_GDESS* Interface = Cast<IMyFirstInterface_GDESS>(FocusedActor))
				{
					Interface->Execute_EndFocus(FocusedActor);
				}
			}
			FocusedActor = nullptr;
		}
	}
}

void ACharacterBase_GDESS::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(const IMyFirstInterface_GDESS* Interface = Cast<IMyFirstInterface_GDESS>(OtherActor))
	{
		Interface->Execute_OnInteract(OtherActor, this);
	}
}

void ACharacterBase_GDESS::FireForward()
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CamShake, 1.0f);
	
	
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Loc, Rot);
	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * TraceDistance);

	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	if(bHit)
	{
		SpawnObject_AKA_Instantiate(Hit.Location, Rot);
		if(Hit.GetActor()->IsRootComponentMovable())
		{
			if(UStaticMeshComponent* MeshCom = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent()))
			{
				FVector CameraForward = CameraComp->GetForwardVector();
				MeshCom->AddImpulse(CameraForward * ImpulseForce * MeshCom->GetMass());
			}
		}

		/* Radial Impulse*/
		if(bApplyRadialImpulse)
		{
			FCollisionShape SphereColl = FCollisionShape::MakeSphere(ImpactRadius);
			bool bSweepHit = GetWorld()->SweepMultiByChannel(HitActors, Hit.Location,
				Hit.Location + FVector(0.01f, 0.01f, 0.01f), FQuat::Identity,
				ECC_WorldStatic, SphereColl);

			DrawDebugSphere(GetWorld(), Hit.Location, ImpactRadius, 50, FColor::Purple, false, 2.0f);
			if(bSweepHit)
			{
				for(auto& hit : HitActors)
				{
					if(UStaticMeshComponent* MeshCom = Cast<UStaticMeshComponent>(hit.GetActor()->GetRootComponent()))
					{
						MeshCom->AddRadialImpulse(hit.Location, ImpactRadius, RadialImpactForce,
							ERadialImpulseFalloff::RIF_Constant, true);
					}
				}
			}
		}
	}
}


void ACharacterBase_GDESS::SpawnObject_AKA_Instantiate(FVector Location, FRotator Rotation)
{
	FActorSpawnParameters SpawnParameters;
	AInteractableBase* SpawnedActorRef =
		GetWorld()->SpawnActor<AInteractableBase>(ActorToSpawn, Location, Rotation, SpawnParameters);

	/* If you want to use it after spawning it */
	//SpawnedActorRef->OnInteract();
}


void ACharacterBase_GDESS::SquashProgress(float Value)
{
	FVector NewScale = FMath::Lerp(StartScale, TargetScale, Value);
	MeshComp->SetWorldScale3D(NewScale);
}


void ACharacterBase_GDESS::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if(CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("SquashProgress"));
		SquashTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		SquashTimeline.SetLooping(false);
		SquashTimeline.PlayFromStart();
	}
}



#pragma endregion HANZO_LEARNING_UNREAL_CPP__USERDEFINED_FUNCTIONS

