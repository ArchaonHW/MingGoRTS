#include "Tournament/MingRTSTournamentSystem.h"
#include "Misc/DateTime.h"

UMingRTSTournamentSystem::UMingRTSTournamentSystem()
{
}

void UMingRTSTournamentSystem::InitializeTournamentSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Tournament System Initialized"));
}

FString UMingRTSTournamentSystem::CreateTournament(const FTournamentInfo& Info)
{
    FString TournamentID = FString::Printf(TEXT("TOURNAMENT_%s_%d"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")), FMath::Rand());
    
    FTournamentInfo NewTournament = Info;
    NewTournament.TournamentID = TournamentID;
    NewTournament.CurrentParticipants = 0;
    NewTournament.Status = ETournamentStatus::Registration;
    
    Tournaments.Add(TournamentID, NewTournament);
    TournamentParticipants.Add(TournamentID, TArray<FTournamentParticipant>());
    TournamentMatches.Add(TournamentID, TArray<FTournamentMatch>());
    
    OnTournamentCreated.Broadcast(TournamentID, NewTournament);
    
    UE_LOG(LogTemp, Log, TEXT("Tournament created: %s - %s"), *TournamentID, *NewTournament.Name);
    return TournamentID;
}

void UMingRTSTournamentSystem::DeleteTournament(const FString& TournamentID)
{
    Tournaments.Remove(TournamentID);
    TournamentParticipants.Remove(TournamentID);
    TournamentMatches.Remove(TournamentID);
    
    UE_LOG(LogTemp, Log, TEXT("Tournament deleted: %s"), *TournamentID);
}

bool UMingRTSTournamentSystem::RegisterPlayer(const FString& TournamentID, const FString& PlayerID, const FString& Password)
{
    FTournamentInfo* Tournament = Tournaments.Find(TournamentID);
    if (!Tournament)
    {
        UE_LOG(LogTemp, Warning, TEXT("Tournament not found: %s"), *TournamentID);
        return false;
    }
    
    if (Tournament->Status != ETournamentStatus::Registration)
    {
        UE_LOG(LogTemp, Warning, TEXT("Tournament not in registration phase: %s"), *TournamentID);
        return false;
    }
    
    if (Tournament->bRequiresPassword && Tournament->Password != Password)
    {
        UE_LOG(LogTemp, Warning, TEXT("Incorrect password for tournament: %s"), *TournamentID);
        return false;
    }
    
    if (Tournament->CurrentParticipants >= Tournament->MaxParticipants)
    {
        UE_LOG(LogTemp, Warning, TEXT("Tournament is full: %s"), *TournamentID);
        return false;
    }
    
    TArray<FTournamentParticipant>* Participants = TournamentParticipants.Find(TournamentID);
    if (Participants)
    {
        // 檢查是否已經報名
        for (const FTournamentParticipant& P : *Participants)
        {
            if (P.PlayerID == PlayerID)
            {
                UE_LOG(LogTemp, Warning, TEXT("Player already registered: %s"), *PlayerID);
                return false;
            }
        }
        
        FTournamentParticipant NewParticipant;
        NewParticipant.PlayerID = PlayerID;
        NewParticipant.PlayerName = PlayerID; // 簡化處理
        NewParticipant.Seed = Participants->Num() + 1;
        
        Participants->Add(NewParticipant);
        Tournament->CurrentParticipants = Participants->Num();
        
        OnPlayerRegistered.Broadcast(TournamentID, PlayerID);
        
        UE_LOG(LogTemp, Log, TEXT("Player %s registered for tournament %s"), *PlayerID, *TournamentID);
        return true;
    }
    
    return false;
}

void UMingRTSTournamentSystem::UnregisterPlayer(const FString& TournamentID, const FString& PlayerID)
{
    TArray<FTournamentParticipant>* Participants = TournamentParticipants.Find(TournamentID);
    if (Participants)
    {
        for (int32 i = 0; i < Participants->Num(); ++i)
        {
            if ((*Participants)[i].PlayerID == PlayerID)
            {
                Participants->RemoveAt(i);
                
                FTournamentInfo* Tournament = Tournaments.Find(TournamentID);
                if (Tournament)
                {
                    Tournament->CurrentParticipants = Participants->Num();
                }
                
                UE_LOG(LogTemp, Log, TEXT("Player %s unregistered from tournament %s"), *PlayerID, *TournamentID);
                return;
            }
        }
    }
}

bool UMingRTSTournamentSystem::StartTournament(const FString& TournamentID)
{
    FTournamentInfo* Tournament = Tournaments.Find(TournamentID);
    if (!Tournament)
    {
        return false;
    }
    
    if (Tournament->Status != ETournamentStatus::Registration)
    {
        return false;
    }
    
    // 檢查最小參賽人數
    TArray<FTournamentParticipant>* Participants = TournamentParticipants.Find(TournamentID);
    if (!Participants || Participants->Num() < 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough participants to start tournament: %s"), *TournamentID);
        return false;
    }
    
    Tournament->Status = ETournamentStatus::InProgress;
    Tournament->TournamentStart = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
    
    // 生成賽程
    GenerateBracket(TournamentID);
    
    OnTournamentStarted.Broadcast(TournamentID, Tournament->TournamentStart);
    
    UE_LOG(LogTemp, Log, TEXT("Tournament started: %s with %d participants"), *TournamentID, Participants->Num());
    return true;
}

void UMingRTSTournamentSystem::EndTournament(const FString& TournamentID)
{
    FTournamentInfo* Tournament = Tournaments.Find(TournamentID);
    if (Tournament)
    {
        Tournament->Status = ETournamentStatus::Finished;
        Tournament->TournamentEnd = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
        
        // 找出冠軍
        FString WinnerID;
        TArray<FTournamentParticipant>* Participants = TournamentParticipants.Find(TournamentID);
        if (Participants)
        {
            for (const FTournamentParticipant& P : *Participants)
            {
                if (!P.bEliminated)
                {
                    WinnerID = P.PlayerID;
                    break;
                }
            }
        }
        
        OnTournamentEnded.Broadcast(TournamentID, WinnerID);
        
        UE_LOG(LogTemp, Log, TEXT("Tournament ended: %s, Winner: %s"), *TournamentID, *WinnerID);
    }
}

bool UMingRTSTournamentSystem::SubmitMatchResult(const FString& MatchID, const FString& WinnerID, int32 Score1, int32 Score2)
{
    // 在所有比賽中查找
    for (auto& Pair : TournamentMatches)
    {
        for (FTournamentMatch& Match : Pair.Value)
        {
            if (Match.MatchID == MatchID)
            {
                Match.WinnerID = WinnerID;
                Match.Player1Score = Score1;
                Match.Player2Score = Score2;
                Match.Status = EMatchStatus::Finished;
                Match.EndTime = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
                
                // 更新參賽者統計
                AdvanceWinner(MatchID, WinnerID);
                
                // 更新排行榜
                bool bPlayer1Won = (Match.Player1ID == WinnerID);
                UpdateLeaderboard(Match.Player1ID, bPlayer1Won);
                UpdateLeaderboard(Match.Player2ID, !bPlayer1Won);
                
                OnMatchCompleted.Broadcast(MatchID, WinnerID, Match.Round);
                
                UE_LOG(LogTemp, Log, TEXT("Match %s completed. Winner: %s (Score: %d - %d)"), 
                    *MatchID, *WinnerID, Score1, Score2);
                return true;
            }
        }
    }
    
    return false;
}

TArray<FTournamentInfo> UMingRTSTournamentSystem::GetTournamentList(ETournamentStatus StatusFilter) const
{
    TArray<FTournamentInfo> Result;
    
    for (const auto& Pair : Tournaments)
    {
        if (StatusFilter == ETournamentStatus::Registration || Pair.Value.Status == StatusFilter)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

bool UMingRTSTournamentSystem::GetTournamentInfo(const FString& TournamentID, FTournamentInfo& OutInfo) const
{
    const FTournamentInfo* Info = Tournaments.Find(TournamentID);
    if (Info)
    {
        OutInfo = *Info;
        return true;
    }
    return false;
}

TArray<FTournamentParticipant> UMingRTSTournamentSystem::GetParticipants(const FString& TournamentID) const
{
    const TArray<FTournamentParticipant>* Participants = TournamentParticipants.Find(TournamentID);
    if (Participants)
    {
        return *Participants;
    }
    return TArray<FTournamentParticipant>();
}

TArray<FTournamentMatch> UMingRTSTournamentSystem::GetMatches(const FString& TournamentID) const
{
    const TArray<FTournamentMatch>* Matches = TournamentMatches.Find(TournamentID);
    if (Matches)
    {
        return *Matches;
    }
    return TArray<FTournamentMatch>();
}

TArray<FLeaderboardEntry> UMingRTSTournamentSystem::GetLeaderboard(int32 TopCount) const
{
    TArray<FLeaderboardEntry> Result;
    
    for (const auto& Pair : Leaderboard)
    {
        Result.Add(Pair.Value);
    }
    
    // 按排名排序
    Result.Sort([](const FLeaderboardEntry& A, const FLeaderboardEntry& B) {
        return A.Rank < B.Rank;
    });
    
    // 限制數量
    if (Result.Num() > TopCount)
    {
        Result.SetNum(TopCount);
    }
    
    return Result;
}

void UMingRTSTournamentSystem::UpdatePlayerRating(const FString& PlayerID, int32 RatingChange)
{
    FLeaderboardEntry* Entry = Leaderboard.Find(PlayerID);
    if (!Entry)
    {
        FLeaderboardEntry NewEntry;
        NewEntry.PlayerID = PlayerID;
        NewEntry.PlayerName = PlayerID;
        Leaderboard.Add(PlayerID, NewEntry);
        Entry = Leaderboard.Find(PlayerID);
    }
    
    if (Entry)
    {
        Entry->Rating += RatingChange;
        
        // 重新計算排名
        int32 NewRank = 1;
        for (const auto& Pair : Leaderboard)
        {
            if (Pair.Value.Rating > Entry->Rating)
            {
                NewRank++;
            }
        }
        Entry->Rank = NewRank;
    }
}

int32 UMingRTSTournamentSystem::GetPlayerRank(const FString& PlayerID) const
{
    const FLeaderboardEntry* Entry = Leaderboard.Find(PlayerID);
    if (Entry)
    {
        return Entry->Rank;
    }
    return -1;
}

TArray<FString> UMingRTSTournamentSystem::GetPlayerTournamentHistory(const FString& PlayerID) const
{
    const TArray<FString>* History = PlayerHistory.Find(PlayerID);
    if (History)
    {
        return *History;
    }
    return TArray<FString>();
}

void UMingRTSTournamentSystem::SetTournamentSpectatorRoom(const FString& TournamentID, const FString& RoomID)
{
    UE_LOG(LogTemp, Log, TEXT("Tournament %s spectator room set to %s"), *TournamentID, *RoomID);
}

void UMingRTSTournamentSystem::GenerateBracket(const FString& TournamentID)
{
    TArray<FTournamentParticipant>* Participants = TournamentParticipants.Find(TournamentID);
    TArray<FTournamentMatch>* Matches = TournamentMatches.Find(TournamentID);
    
    if (!Participants || !Matches || Participants->Num() < 2)
    {
        return;
    }
    
    Matches->Empty();
    
    int32 NumParticipants = Participants->Num();
    int32 Rounds = FMath::CeilLogTwo(NumParticipants);
    int32 MatchesInFirstRound = FMath::Pow(2, Rounds - 1);
    
    // 簡化的單敗淘汰賽生成
    int32 CurrentMatch = 0;
    for (int32 i = 0; i < NumParticipants - 1; i += 2)
    {
        FTournamentMatch Match;
        Match.MatchID = GenerateMatchID();
        Match.TournamentID = TournamentID;
        Match.Round = 1;
        Match.Player1ID = (*Participants)[i].PlayerID;
        Match.Player2ID = (i + 1 < NumParticipants) ? (*Participants)[i + 1].PlayerID : TEXT("BYE");
        Match.Status = EMatchStatus::Scheduled;
        Match.bIsFinal = (NumParticipants <= 2);
        
        Matches->Add(Match);
        CurrentMatch++;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d matches for tournament %s"), CurrentMatch, *TournamentID);
}

void UMingRTSTournamentSystem::AdvanceWinner(const FString& MatchID, const FString& WinnerID)
{
    // 標記被淘汰的玩家
    for (auto& Pair : TournamentParticipants)
    {
        for (FTournamentParticipant& P : Pair.Value)
        {
            TArray<FTournamentMatch>* Matches = TournamentMatches.Find(Pair.Key);
            if (Matches)
            {
                for (const FTournamentMatch& M : *Matches)
                {
                    if (M.MatchID == MatchID)
                    {
                        if (M.Player1ID == WinnerID)
                        {
                            if (P.PlayerID == M.Player2ID)
                            {
                                P.bEliminated = true;
                                P.Losses++;
                            }
                            else if (P.PlayerID == WinnerID)
                            {
                                P.Wins++;
                            }
                        }
                        else if (M.Player2ID == WinnerID)
                        {
                            if (P.PlayerID == M.Player1ID)
                            {
                                P.bEliminated = true;
                                P.Losses++;
                            }
                            else if (P.PlayerID == WinnerID)
                            {
                                P.Wins++;
                            }
                        }
                    }
                }
            }
        }
    }
}

void UMingRTSTournamentSystem::UpdateLeaderboard(const FString& PlayerID, bool bWon)
{
    FLeaderboardEntry* Entry = Leaderboard.Find(PlayerID);
    if (!Entry)
    {
        FLeaderboardEntry NewEntry;
        NewEntry.PlayerID = PlayerID;
        NewEntry.PlayerName = PlayerID;
        Leaderboard.Add(PlayerID, NewEntry);
        Entry = Leaderboard.Find(PlayerID);
    }
    
    if (Entry)
    {
        if (bWon)
        {
            Entry->Wins++;
        }
        else
        {
            Entry->Losses++;
        }
        
        Entry->TournamentsPlayed++;
        
        // 更新評分
        int32 RatingChange = bWon ? 15 : -10;
        UpdatePlayerRating(PlayerID, RatingChange);
    }
    
    // 記錄歷史
    TArray<FString>* History = PlayerHistory.Find(PlayerID);
    if (!History)
    {
        PlayerHistory.Add(PlayerID, TArray<FString>());
        History = PlayerHistory.Find(PlayerID);
    }
}

FString UMingRTSTournamentSystem::GenerateMatchID()
{
    return FString::Printf(TEXT("MATCH_%s_%d"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")), FMath::Rand());
}

int32 UMingRTSTournamentSystem::CalculateNewRating(int32 CurrentRating, int32 OpponentRating, bool bWon)
{
    // 簡化的Elo評分系統
    float ExpectedScore = 1.0f / (1.0f + FMath::Pow(10.0f, (OpponentRating - CurrentRating) / 400.0f));
    float KFactor = 32.0f;
    float ActualScore = bWon ? 1.0f : 0.0f;
    
    return CurrentRating + FMath::RoundToInt(KFactor * (ActualScore - ExpectedScore));
}
