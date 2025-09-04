// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SHPlayerController.h"

#include "JSHBaseBall.h"
#include "Kismet/KismetSystemLibrary.h"
#include  "UI/SHChatInput.h"
#include "JSHBaseBall.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/SHGameModeBase.h"
#include "SHPlayerState.h"
#include "Net/UnrealNetwork.h"

ASHPlayerController::ASHPlayerController()
{
	bReplicates = true;
}

void ASHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;

	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass)== true)
	{
		ChatInputWidgetInstance = CreateWidget<USHChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}
	PrintChatMessageString(ChatMessageString);
	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void ASHPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	
	if (IsLocalController() == true)
	{
		// ServerRPCPrintChatMessageString(InChatMessageString);

		ASHPlayerState* SHPS = GetPlayerState<ASHPlayerState>();
		if (IsValid(SHPS) == true)
		{
			//FString CombinedMessageString = SHPS->PlayerNameString + TEXT(": ") + InChatMessageString;
			FString CombinedMessageString = SHPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}



void ASHPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	//UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Red, 5.0f);

	/*FString NetModeString = JSHBaseBallFunctionLibrary::GetNetModeString(this);
	FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	JSHBaseBallFunctionLibrary::MyPrintString(this, CombinedMessageString, 10.f);
	// 문제 상황이 생기면, 위와 같은 로깅 함수로 다양한 변수의 값들과 함수이름을 확인해서 
	// 문제의 원인을 적극적으로 찾아보세요!*/
	
	JSHBaseBallFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void ASHPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ASHPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ASHGameModeBase* SHGM = Cast<ASHGameModeBase>(GM);
		if (IsValid(SHGM) == true)
		{
			SHGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void ASHPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}