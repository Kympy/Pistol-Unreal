// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol_Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APistol_Gun::APistol_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void APistol_Gun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APistol_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APistol_Gun::PullTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("In Pulltrigger"));
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("Muzzle"));
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController();
	if(OwnerController == nullptr) return;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange;
	FHitResult Hit;
	bool Success = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1);
	if(Success)
	{
		FVector AimDirection = -Rotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Impact, Hit.Location, AimDirection.Rotation());
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		AActor* HitActor = Hit.GetActor();
		if(HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(PrimaryDamage, Hit, AimDirection, nullptr);
			HitActor->TakeDamage(PrimaryDamage, DamageEvent, OwnerController, this);
		}
	}
}

