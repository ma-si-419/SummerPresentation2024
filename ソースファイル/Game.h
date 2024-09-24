#pragma once
namespace Game
{
	namespace InputId
	{
		const char* const kOk = "OK";
		const char* const kPause = "PAUSE";
		const char* const kSelect = "SELECT";
		const char* const kCancel = "CANCEL";
		const char* const kUp = "UP";
		const char* const kDown = "DOWN";
		const char* const kLeft = "LEFT";
		const char* const kRight = "RIGHT";
		const char* const kLb = "LB";
		const char* const kRb = "RB";
		const char* const kA = "A";
		const char* const kB = "B";
		const char* const kX = "X";
		const char* const kY = "Y";

	}
	enum class AttackInfoSort
	{
		kId,
		kName,
		kCost,
		kDamageRate,
		kStanDamage,
		kSpeed,
		kRadius,
		kLifeTime,
		kAttackNum,
		kLaser,
		kScatter,
		kTrack,
		kAttackStartTime,
		kAttackEndTime,
		kActionTotalTime,
		kEnergy,
		kHitEffect,
		kIsSpecial,
		kEffekseerName,
		kAnimationName,
		kSoundName
	};

	enum class UiInfoSort
	{
		kPath,
		kPosX,
		kPosY,
		kScene
	};
	enum class AiInfoSort
	{
		kPlayerState,
		kIdle,
		kMove,
		kDash,
		kDodge,
		kAttack,
		kGuard,
		kCharge
	};
	enum class EffeckInfoSort
	{
		kName,
		kFileName,
		kMag,
		kLoopFrame
	};
	enum class SoundInfo
	{
		kName,
		kScene
	};
	enum class SceneNum
	{
		kTitle,
		kSelect,
		kGame
	};
	constexpr int kWindowWidth = 1536;
	constexpr int kWindowHeight = 864;

}