, // Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Pistol_Gun.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	//FireTimer = Gun->FireRate;
	FireTimer = 0;
	GetWorldTimerManager().SetTimer(Handle, this, &APlayerCharacter::CheckFireRate, 0.2f, true);
	Gun = GetWorld()->SpawnActor<APistol_Gun>(GunClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
// PC
	// Keyboard Movement
	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &APlayerCharacter::MoveVertical);
	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &APlayerCharacter::MoveHorizontal);
	// Keyboard Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	// Keyboard Shoot
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &APlayerCharacter::Release);
	// Keyboard Aim
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Aim);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &APlayerCharacter::Aim);
	// Mouse Camera Control
	PlayerInputComponent->BindAxis(TEXT("LookVertical"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookHorizontal"), this, &APawn::AddControllerYawInput);

// Controller
	// Gamepad Camera Control
	PlayerInputComponent->BindAxis(TEXT("LookVerticalRate"), this, &APlayerCharacter::LookVerticalRate);
	PlayerInputComponent->BindAxis(TEXT("LookHorizontalRate"), this, &APlayerCharacter::LookHorizontalRate);
}
// Move Forward & Backward
void APlayerCharacter::MoveVertical(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

// Move Right & Left
void APlayerCharacter::MoveHorizontal(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

// Gamepad Camera Control
void APlayerCharacter::LookVerticalRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * controllerRotationRate * GetWorld()->GetDeltaSeconds());
}
void APlayerCharacter::LookHorizontalRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * controllerRotationRate * GetWorld()->GetDeltaSeconds());
}

// Attack with pistol
void APlayerCharacter::Shoot()
{
	if(IsAiming) // Aiming == true then
	{
		if(CanShoot)
		{
			IsShoot = true;
			Gun->PullTrigger();
			UE_LOG(LogTemp, Warning, TEXT("One Shot"));
			CanShoot = false;
		}
	}
	else IsShoot = false;
}
void APlayerCharacter::Release()
{
	IsShoot = false;
}
void APlayerCharacter::CheckFireRate()
{
	FireTimer = FireTimer - 0.2f;
	if(FireTimer <= 0)
	{
		ResetFireRate();
		CanShoot = true;
	}
}
void APlayerCharacter::ResetFireRate()
{
	FireTimer = 1.2f;
}
// Aiming with right mouse button
void APlayerCharacter::Aim()
{ 
	IsAiming = !IsAiming; 
}

float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Left Health : %f"), Health);
	return DamageToApply;
}
