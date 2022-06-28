// Fill out your copyright notice in the Description page of Project Settings.


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
	Health = MaxHealth; // 체력 초기화
	CanShoot = true; // 처음 시작 시에는 첫 발 사격이 가능함
	FireTimer = 0; // 발사 시간 초기화
	GetWorldTimerManager().SetTimer(Handle, this, &APlayerCharacter::CheckFireRate, 0.2f, true); // 타이머를 일정 초 마다 호출
	Gun = GetWorld()->SpawnActor<APistol_Gun>(GunClass); // 무기를 소환
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket")); // 소환한 무기를 캐릭터의 소켓에 부착
	Gun->SetOwner(this); // 무기의 owner를 캐릭터 본인으로 설정
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
// PC 조작
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

// Controller 조작
	// Gamepad Camera Control
	PlayerInputComponent->BindAxis(TEXT("LookVerticalRate"), this, &APlayerCharacter::LookVerticalRate);
	PlayerInputComponent->BindAxis(TEXT("LookHorizontalRate"), this, &APlayerCharacter::LookHorizontalRate);
}
// 전방 후방 이동
void APlayerCharacter::MoveVertical(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

// 좌 우 이동
void APlayerCharacter::MoveHorizontal(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

// 게임 패드 조작 시 카메라 시점 조작
void APlayerCharacter::LookVerticalRate(float AxisValue) // 수직
{
	AddControllerPitchInput(AxisValue * controllerRotationRate * GetWorld()->GetDeltaSeconds());
}
void APlayerCharacter::LookHorizontalRate(float AxisValue) // 수평
{
	AddControllerYawInput(AxisValue * controllerRotationRate * GetWorld()->GetDeltaSeconds());
}

// 사격
void APlayerCharacter::Shoot()
{
	if(IsAiming) // 조준을 해야 사격 가능
	{
		if(CanShoot) // 발사 간격 시간이 다시 돌아왔다면
		{
			Gun->PullTrigger(); // 방아쇠 당기기
			CanShoot = false; // 쐈으니까 발사 간격이 돌아오기 전까지 못 쏨
			PlayAnimMontage(ShootAnimation, 1.67f); // 사격 애니메이션 몽타주 재생
			//UE_LOG(LogTemp, Warning, TEXT("One Shot"));
		}
	}
}
// 조준 해제
void APlayerCharacter::Release()
{
	
}
// 발사 간격 체크
void APlayerCharacter::CheckFireRate()
{
	FireTimer = FireTimer - 0.2f; // 일정 시간에 한 번씩 체크
	if(FireTimer <= 0.0f) // FireTimer 초기값에서 매 시간 마다 감소 되다가 만약 0 이하라면 
	{
		ResetFireRate(); // 발사 타이머 초기화
		CanShoot = true; // 다시 쏠 수 있는 상태가 된다.
	}
}
// 발사 시간 초기화
void APlayerCharacter::ResetFireRate()
{
	FireTimer = 0.8f;
}
// 마우스 우클릭으로 조준, 조준 해제 시에 호출
void APlayerCharacter::Aim()
{ 
	IsAiming = !IsAiming;
}
// 데미지 적용 함수
float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Left Health : %f"), Health);
	return DamageToApply;
}
// 죽는지 확인하기
bool APlayerCharacter::IsDead() const
{
	return Health <= 0;
}
