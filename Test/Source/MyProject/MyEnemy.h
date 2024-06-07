// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnemy.generated.h"

UCLASS()
class MYPROJECT_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION(Server, Reliable)
	void ServerRPC_AddHP(int32 value);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetHP(int32 value);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Die();


	UPROPERTY(/*Replicated*/)
	int32 MyHp = 100;
};
