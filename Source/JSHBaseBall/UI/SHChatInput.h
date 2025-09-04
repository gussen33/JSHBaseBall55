// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "SHChatInput.generated.h"


class UEditableTextBox;

/**
 * 
 */
UCLASS()
class JSHBASEBALL_API USHChatInput : public UUserWidget
{
	GENERATED_BODY()
public:	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;

protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
};
