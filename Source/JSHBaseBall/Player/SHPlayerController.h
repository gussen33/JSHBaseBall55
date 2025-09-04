// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SHPlayerController.generated.h"

class USHChatInput;
class UUserWidget;
/**
 * 
 */
UCLASS()
class JSHBASEBALL_API ASHPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	ASHPlayerController();
	
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USHChatInput> ChatInputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<USHChatInput> ChatInputWidgetInstance;

protected:
	FString ChatMessageString;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
	
};
