// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SHGameModeBase.generated.h"

/**
 * 
 */

class ASHPlayerController;

UCLASS()
class JSHBASEBALL_API ASHGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:	
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;
	
	void PrintChatMessageString(ASHPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(ASHPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(ASHPlayerController* InChattingPlayerController, int InStrikeCount);
	
protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ASHPlayerController>> AllPlayerControllers;
	
};
