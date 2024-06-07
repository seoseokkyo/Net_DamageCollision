// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "MyEnemy.h"
#include "Kismet/KismetSystemLibrary.h"

AMyProjectCharacter::AMyProjectCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMyProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMyProjectCharacter::ServerRPC_Attack_Implementation(FVector dir)
{
	MultiRPC_Attack(dir);
}

void AMyProjectCharacter::MultiRPC_Attack_Implementation(FVector dir)
{
	TArray<FHitResult> results;

	FVector start = GetActorLocation();
	FVector end = start + GetActorForwardVector() * 300;

	FCollisionShape myCapsule = FCollisionShape::MakeCapsule(FVector(30, 100, 30));
	bool bResult = GetWorld()->SweepMultiByChannel(results, start, end, FQuat::Identity, ECC_Pawn, myCapsule);

	DrawDebugCapsule(GetWorld(), start + GetActorForwardVector() * 50, myCapsule.GetCapsuleHalfHeight(), myCapsule.GetCapsuleRadius(), FQuat::Identity, FColor::Red, true);

	if (bResult)
	{
		for (auto result : results)
		{
			auto enemyCheck = Cast<AMyEnemy>(result.GetActor());

			if (enemyCheck != nullptr)
			{
				UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Enemy HP Before : %d"), enemyCheck->MyHp));
				
				enemyCheck->ServerRPC_AddHP(-10);

				UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Enemy HP After : %d"), enemyCheck->MyHp));
			}
		}
	}
}
