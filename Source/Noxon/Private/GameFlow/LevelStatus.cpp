#include "Public/GameFlow/LevelStatus.h"

bool FLevelProgressData::GetTimeDiff(FTimespan& outDiff)
{
	if (clearTime == FDateTime::MinValue())
	{
		return false;
	}
	outDiff = clearTime - firstEnterTime;
	return true;
}
