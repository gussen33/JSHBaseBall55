// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SHPawn.generated.h"

UCLASS()
class JSHBASEBALL_API ASHPawn : public APawn
{
	GENERATED_BODY()



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;


};
