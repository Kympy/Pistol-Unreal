// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class APistol_Gun;

UCLASS(Blueprintable)
class PISTOL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Keyboard
	void MoveVertical(float AxisValue);
	void MoveHorizontal(float AxisValue);

	// Gamepad
	void LookVerticalRate(float AxisValue);
	void LookHorizontalRate(float AxisValue);
	// Gamepad Value
	UPROPERTY(EditAnywhere, Category="Controller Setting")
	float controllerRotationRate = 100;	

	// Player Info
	UPROPERTY(VisibleAnywhere, Category="Player Info")
	float Health;
	float MaxHealth = 200;

	// Weapon
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<APistol_Gun> GunClass;
	APistol_Gun* Gun;

	// Attack
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	float FireTimer;
	FTimerHandle Handle;
	void CheckFireRate();
	void ResetFireRate();

public :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack")
	bool IsAiming;
	UPROPERTY(BlueprintReadWrite, Category="Attack")
	bool IsShoot;
	UPROPERTY(BlueprintReadWrite, Category="Attack")
	bool CanShoot;

	UFUNCTION(BlueprintCallable)
	void Aim();
	UFUNCTION(BlueprintCallable)
	void Shoot();
	UFUNCTION(BlueprintCallable)
	void Release();


};
