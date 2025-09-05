// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(CustomLogInfo, Display, All)

#define CALL_INFO (TEXT(__FUNCTION__) + FString("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define LOG_INFO() UE_LOG(CustomLogInfo, Warning, TEXT("%s"), *CALL_INFO)

#define LOG_TEXT(...) \
UE_LOG(CustomLogInfo, Display, TEXT("%s : %s"), *CALL_INFO, \
*FString::Printf(##__VA_ARGS__))