// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
    // 폰 변수를 만들고 플레이어 인덱스(0)로 플레이어 폰을 저장
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    // 바라보기
    SetFocus(PlayerPawn);
}
