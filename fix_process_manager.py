#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
修复 ProcessManager 头文件编码问题
"""

import os

# 修复后的 MingRTSProcessManager.h 内容
MINGRTS_PROCESS_MANAGER_H = '''// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Process Manager - Operating System Core
// Provides process management, scheduling, and resource allocation

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/CriticalSection.h"
#include "Containers/Queue.h"
#include "Containers/Map.h"
#include "MingRTSProcessManager.generated.h"

// Scheduling algorithms
UENUM(BlueprintType)
enum class ESchedulingAlgorithm: uint8 {
    FIFO            UMETA(DisplayName = "First In First Out"),
    Priority        UMETA(DisplayName = "Priority Scheduling"),
    RoundRobin      UMETA(DisplayName = "Round Robin"),
    SJF             UMETA(DisplayName = "Shortest Job First"),
    MLFQ            UMETA(DisplayName = "Multi-Level Feedback Queue")
};

// Scheduler states
UENUM(BlueprintType)
enum class ESchedulerState: uint8 {
    Idle            UMETA(DisplayName = "Idle"),
    Running         UMETA(DisplayName = "Running"),
    Paused          UMETA(DisplayName = "Paused"),
    Error           UMETA(DisplayName = "Error")
};

// Process queue node
USTRUCT(BlueprintType)
struct FProcessQueueNode
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Process Queue")
    FString ProcessID;

    UPROPERTY(BlueprintReadOnly, Category = "Process Queue")
    int32 Priority;

    UPROPERTY(BlueprintReadOnly, Category = "Process Queue")
    float ArrivalTime;

    UPROPERTY(BlueprintReadOnly, Category = "Process Queue")
    float BurstTime;

    UPROPERTY(BlueprintReadOnly, Category = "Process Queue")
    int32 TimeSlice;

    FProcessQueueNode()
        : Priority(0)
        , ArrivalTime(0.0f)
        , BurstTime(0.0f)
        , TimeSlice(100)  // Default time slice 100ms
    {}
};

// Process control block
USTRUCT(BlueprintType)
struct FProcessControlBlock
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    FString ProcessID;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    FString ProcessName;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    int32 Priority;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    ESchedulerState State;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    float CPUUsage;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    float MemoryUsage;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    int32 ThreadCount;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "Process")
    float ExecutionTime;

    FProcessControlBlock()
        : Priority(0)
        , State(ESchedulerState::Idle)
        , CPUUsage(0.0f)
        , MemoryUsage(0.0f)
        , ThreadCount(1)
        , ExecutionTime(0.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProcessCreated, FString, ProcessID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProcessTerminated, FString, ProcessID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessStateChanged, FString, ProcessID, ESchedulerState, NewState);

/**
 * RTS Process Manager - Operating System Core Component
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSProcessManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSProcessManager();

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void InitializeProcessManager();

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void ShutdownProcessManager();

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FString CreateProcess(const FString& ProcessName, int32 Priority = 0);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool TerminateProcess(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool SuspendProcess(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    bool ResumeProcess(const FString& ProcessID);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void SetProcessPriority(const FString& ProcessID, int32 NewPriority);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    int32 GetProcessPriority(const FString& ProcessID) const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void SetSchedulingAlgorithm(ESchedulingAlgorithm Algorithm);

    UFUNCTION(BlueprintPure, Category = "Process Manager")
    ESchedulingAlgorithm GetSchedulingAlgorithm() const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    void UpdateScheduler(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    TArray<FProcessControlBlock> GetActiveProcesses() const;

    UFUNCTION(BlueprintCallable, Category = "Process Manager")
    FProcessControlBlock GetProcessInfo(const FString& ProcessID) const;

    UFUNCTION(BlueprintPure, Category = "Process Manager")
    int32 GetActiveProcessCount() const;

    UFUNCTION(BlueprintPure, Category = "Process Manager")
    bool IsProcessRunning(const FString& ProcessID) const;

    UPROPERTY(BlueprintAssignable, Category = "Process Manager Events")
    FOnProcessCreated OnProcessCreated;

    UPROPERTY(BlueprintAssignable, Category = "Process Manager Events")
    FOnProcessTerminated OnProcessTerminated;

    UPROPERTY(BlueprintAssignable, Category = "Process Manager Events")
    FOnProcessStateChanged OnProcessStateChanged;

protected:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    ESchedulingAlgorithm CurrentAlgorithm;

    UPROPERTY()
    ESchedulerState SchedulerState;

    UPROPERTY()
    TArray<FProcessControlBlock> ProcessTable;

    UPROPERTY()
    TArray<FProcessQueueNode> ReadyQueue;

    UPROPERTY()
    FString CurrentProcessID;

    float TimeQuantum;
    float CurrentTimeSlice;

    void InitializeScheduler();
    void ScheduleProcesses();
    void ContextSwitch(const FString& NextProcessID);
    void UpdateProcessStates(float DeltaTime);
    FString GenerateProcessID() const;
    int32 FindProcessIndex(const FString& ProcessID) const;
};
'''

def fix_file(filepath, content):
    """修复单个文件"""
    try:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"Fixed: {filepath}")
        return True
    except Exception as e:
        print(f"Error fixing {filepath}: {e}")
        return False

def main():
    """主函数"""
    process_file = r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Process\MingRTSProcessManager.h'
    
    success = True
    
    if os.path.exists(process_file):
        if not fix_file(process_file, MINGRTS_PROCESS_MANAGER_H):
            success = False
    else:
        print(f"Not found: {process_file}")
        success = False
    
    if success:
        print("\nDone! Process Manager file fixed successfully.")
    else:
        print("\nFile could not be fixed.")
    
    return 0 if success else 1

if __name__ == '__main__':
    exit(main())
