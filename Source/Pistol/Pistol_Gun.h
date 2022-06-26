// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pistol_Gun.generated.h"

UCLASS()
class PISTOL_API APistol_Gun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APistol_Gun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float FireRate = 1.5;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	// Muzzle
	UPROPERTY(EditAnywhere, Category="Attack")
	UParticleSystem* MuzzleFlash;

	// Impact
	UPROPERTY(EditAnywhere, Category="Attack")
	UParticleSystem* Impact;

	UPROPERTY(EditAnywhere, Category="Attack")
	float MaxRange = 10000;

	UPROPERTY(EditAnywhere, Category="Attack")
	float PrimaryDamage = 10;
};
