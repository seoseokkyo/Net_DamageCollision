// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	//DOREPLIFETIME(AMyEnemy, MyHp);
}

void AMyEnemy::MultiRPC_SetHP_Implementation(int32 value)
{
	MyHp = value;
}

void AMyEnemy::MultiRPC_Die_Implementation()
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("EnemyDie")));

	Destroy();
}

void AMyEnemy::ServerRPC_AddHP_Implementation(int32 value)
{
	MyHp += value;

	MultiRPC_SetHP(MyHp);

	if (MyHp <= 0)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Call MultiRPC_Die")));

		MultiRPC_Die();
	}
}

