#pragma once

#include "CoreMinimal.h"


DECLARE_LOG_CATEGORY_EXTERN(CustomLogInfo, Display, All)

#define CALL_INFO (TEXT(__FUNCTION__) + FString("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define LOG_INFO() UE_LOG(CustomLogInfo, Warning, TEXT("%s"), *CALL_INFO)

// ... = TEXT(fmt, value) | (ex) fmt: "%s" value: *FString("Hello World")
#define LOG_TEXT(fmt, ...) \
	UE_LOG(CustomLogInfo, Display, TEXT("%s : %s"), *CALL_INFO, *FString::Printf(fmt, ##__VA_ARGS__))

// ... = TEXT(fmt, value) | (ex) fmt: "%s" value: *FString("Hello World") 
#define PRINT_TEXT(key, time, ...) \
GEngine->AddOnScreenDebugMessage(key, time, FColor::Purple, FString::Printf(##__VA_ARGS__));