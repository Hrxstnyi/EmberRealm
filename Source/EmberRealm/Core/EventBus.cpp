// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/EventBus.h"

FEventBus& FEventBus::Get()
{
	static FEventBus Instance;
	return Instance;
}

void FEventBus::Broadcast(const FString& EventName, const FString& Payload)
{
	EventLog.Add(TPair<FString, FString>(EventName, Payload));
	if (EventLog.Num() > 1000)
	{
		EventLog.RemoveAt(0);
	}

	UE_LOG(LogTemp, Log, TEXT("[EventBus] 事件: %s, 数据: %s"), *EventName, *Payload);

	if (TArray<FEventCallback>* Callbacks = Subscribers.Find(EventName))
	{
		for (const FEventCallback& Callback : *Callbacks)
		{
			if (Callback.IsBound())
			{
				Callback.Execute(EventName);
			}
		}
	}
}

void FEventBus::Unsubscribe(const FString& EventName, void* Object)
{
	if (TArray<FEventCallback>* Callbacks = Subscribers.Find(EventName))
	{
		Callbacks->RemoveAll([](const FEventCallback& Cb) { return !Cb.IsBound(); });
	}
}

void FEventBus::Clear()
{
	Subscribers.Empty();
	EventLog.Empty();
}
