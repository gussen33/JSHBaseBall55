// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SHGameModeBase.h"
#include "Game/SHGameStateBase.h"
#include "Player/SHPlayerController.h"
#include "EngineUtils.h"
#include "Player/SHPlayerState.h"

void ASHGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	ASHPlayerController* SHPlayerController = Cast<ASHPlayerController>(NewPlayer);
	if (IsValid(SHPlayerController) == true)
	{
		SHPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		AllPlayerControllers.Add(SHPlayerController);

		ASHPlayerState* SHPS = SHPlayerController->GetPlayerState<ASHPlayerState>();
		if (IsValid(SHPS) == true)
		{
			SHPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ASHGameStateBase* SHGameStateBase =  GetGameState<ASHGameStateBase>();
		if (IsValid(SHGameStateBase) == true)
		{
			SHGameStateBase->MulticastRPCBroadcastLoginMessage(SHPS->PlayerNameString);
		}
	}
}

FString ASHGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ASHGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	// 입력된 문자열의 길이가 3인지 확인합니다.
	if (InNumberString.Len() != 3)
	{
		return false;
	}

	// 각 문자가 숫자이고 '0'이 아닌지 확인합니다.
	for (TCHAR C : InNumberString)
	{
		if (!FChar::IsDigit(C) || C == '0')
		{
			return false;
		}
	}
    
	// 모든 검사를 통과하면 유효한 입력으로 간주합니다.
	// (이전 코드와 달리 중복 숫자를 허용)
	return true;
}
FString ASHGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	// 비밀 숫자에 있는 고유한 숫자들을 TSet에 저장하여 검색을 빠르게 합니다.
	TSet<TCHAR> SecretDigits;
	for (TCHAR C : InSecretNumberString)
	{
		SecretDigits.Add(C);
	}
    
	// 추측한 숫자의 고유한 숫자들을 TSet에 저장합니다. 555는 { '5' }가 됩니다.
	TSet<TCHAR> GuessedDigits;
	for (TCHAR C : InGuessNumberString)
	{
		GuessedDigits.Add(C);
	}

	// 스트라이크(위치+숫자 일치)를 먼저 계산합니다.
	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
	}
    
	// 볼(숫자 일치, 위치 불일치)을 계산합니다.
	for (TCHAR C : GuessedDigits)
	{
		if (SecretDigits.Contains(C))
		{
			BallCount++;
		}
	}
    
	// 볼 카운트에서 스트라이크 카운트를 빼서 중복을 제거합니다.
	// 스트라이크는 볼의 조건도 만족하므로, 볼 카운트에 포함되어 있기 때문입니다.
	BallCount -= StrikeCount;

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}
void ASHGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
}

void ASHGameModeBase::PrintChatMessageString(ASHPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
    int Index = InChatMessageString.Len() - 3;
    FString GuessNumberString = InChatMessageString.RightChop(Index);

    // 1. GuessNumberString이 올바른 형식인지 확인
    if (IsGuessNumberString(GuessNumberString) == true)
    {
       // 2. 플레이어의 시도 횟수 가져오기
       ASHPlayerState* SHPS = InChattingPlayerController->GetPlayerState<ASHPlayerState>();
       
       if (IsValid(SHPS) == true)
       {
          // 3. 현재 시도 횟수가 최대 시도 횟수를 넘었는지 확인
          if (SHPS->CurrentGuessCount >= SHPS->MaxGuessCount)
          {
             // 4. 시도 횟수 초과 메시지를 모든 플레이어에게 전달
             for (TActorIterator<ASHPlayerController> It(GetWorld()); It; ++It)
             {
                ASHPlayerController* SHPlayerController = *It;
                if (IsValid(SHPlayerController) == true)
                {
                   SHPlayerController->ClientRPCPrintChatMessageString(TEXT("You have exceeded the maximum number of guesses. Game Over!"));
                   ResetGame();
                }
             }
             return; // 함수 종료
          }
       }
       
       // 5. 시도 횟수가 남았다면 게임 진행
       FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
       IncreaseGuessCount(InChattingPlayerController);
       
       for (TActorIterator<ASHPlayerController> It(GetWorld()); It; ++It)
       {
          ASHPlayerController* SHPlayerController = *It;
          if (IsValid(SHPlayerController) == true)
          {
             FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
             SHPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
             int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
             JudgeGame(InChattingPlayerController, StrikeCount);
          }
       }
    }
    else
    {
       // 시도 횟수 초과 메시지가 아니라면 일반 채팅 메시지 처리
       for (TActorIterator<ASHPlayerController> It(GetWorld()); It; ++It)
       {
          ASHPlayerController* SHPlayerController = *It;
          if (IsValid(SHPlayerController) == true)
          {
             SHPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
          }
       }
    }
}
void ASHGameModeBase::IncreaseGuessCount(ASHPlayerController* InChattingPlayerController)
{
	ASHPlayerState* SHPS = InChattingPlayerController->GetPlayerState<ASHPlayerState>();
	if (IsValid(SHPS) == true)
	{
		SHPS->CurrentGuessCount++;
	}
}
void ASHGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& SHPlayerController : AllPlayerControllers)
	{
		ASHPlayerState* SHPS = SHPlayerController->GetPlayerState<ASHPlayerState>();
		if (IsValid(SHPS) == true)
		{
			SHPS->CurrentGuessCount = 0;
		}
	}
}

void ASHGameModeBase::JudgeGame(ASHPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ASHPlayerState* SHPS = InChattingPlayerController->GetPlayerState<ASHPlayerState>();
		for (const auto& SHPlayerController : AllPlayerControllers)
		{
			if (IsValid(SHPS) == true)
			{
				FString CombinedMessageString = SHPS->PlayerNameString + TEXT(" has won the game.");
				SHPlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& SHPlayerController : AllPlayerControllers)
		{
			ASHPlayerState* SHPS = SHPlayerController->GetPlayerState<ASHPlayerState>();
			if (IsValid(SHPS) == true)
			{
				if (SHPS->CurrentGuessCount < SHPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& SHPlayerController : AllPlayerControllers)
			{
				SHPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}