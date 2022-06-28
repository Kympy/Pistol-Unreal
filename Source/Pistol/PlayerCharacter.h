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
	float controllerRotationRate = 100;	 // 컨트롤러 회전 감도

	// Player Info
	UPROPERTY(VisibleAnywhere, Category="Player Info")
	float MaxHealth = 100; // 최대 체력
	float Health = MaxHealth; // 현재 체력


	// Weapon
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<APistol_Gun> GunClass;
	APistol_Gun* Gun; // 총 클래스

	// Attack
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	float FireTimer; // 발사 간격을 체크할 타이머
	FTimerHandle Handle; // 타이머를 조정할 handle 변수

	void CheckFireRate(); // 발사 간격 체크
	void ResetFireRate(); // 발사 시간 초기화 (계속 초기화 해야 총을 쏠 수 있다)

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage *ShootAnimation; // 발사 애니메이션 몽타주

public :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack")
	bool IsAiming; // 우클릭으로 조준 중 인지 체크
	UPROPERTY(BlueprintReadWrite, Category="Attack")
	bool CanShoot = true; // 발사 간격에 따라 쏠 수 있는 지 체크

	UFUNCTION(BlueprintCallable)
	void Aim(); // 조준
	UFUNCTION(BlueprintCallable)
	void Shoot(); // 사격
	UFUNCTION(BlueprintCallable)
	void Release(); // 조준 해제
	UFUNCTION(BlueprintPure)
	bool IsDead() const; // 사망 함수
};
