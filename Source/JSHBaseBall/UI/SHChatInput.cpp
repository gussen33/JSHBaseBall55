// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SHChatInput.h"

#include "Components/EditableTextBox.h"
#include "Player/SHPlayerController.h"

void USHChatInput::NativeConstruct()
{
	Super::NativeConstruct();
    
	// 이벤트를 바인딩하기 전에 null 체크를 추가하는 것이 안전합니다.
	if (EditableTextBox_ChatInput)
	{
		// 위젯이 생성될 때 이벤트를 바인딩합니다.
		// 중복 바인딩을 막기 위해 이미 바인딩되었는지 확인합니다.
		if (!EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
		{
			EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
		}
	}
}

void USHChatInput::NativeDestruct()
{
	// Super::NativeDestruct()가 호출되기 전에 이벤트를 언바인딩합니다.
	if (EditableTextBox_ChatInput)
	{
		// 위젯이 소멸될 때 이벤트를 제거합니다.
		if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
		{
			EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
		}
	}
    
	Super::NativeDestruct();
}

void USHChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			ASHPlayerController* OwningSHPlayerController = Cast<ASHPlayerController>(OwningPlayerController);
			if (IsValid(OwningSHPlayerController) == true)
			{
				OwningSHPlayerController->SetChatMessageString(Text.ToString());

				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
