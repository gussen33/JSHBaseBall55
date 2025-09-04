// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SHGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SHPlayerController.h"


void ASHGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ASHPlayerController* SHPC = Cast<ASHPlayerController>(PC);
			if (IsValid(SHPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				SHPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}


