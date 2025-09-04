// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SHPawn.h"
#include "JSHBaseBall.h"


void ASHPawn ::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString =JSHBaseBallFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::BeginPlay() %s [%s]"), *JSHBaseBallFunctionLibrary::GetNetModeString(this), *NetRoleString);
	JSHBaseBallFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ASHPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = JSHBaseBallFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::PossessedBy() %s [%s]"), *JSHBaseBallFunctionLibrary::GetNetModeString(this), *NetRoleString);
	JSHBaseBallFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}
