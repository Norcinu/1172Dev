#ifndef SHOW_STAKE_PROCESS_H
#define SHOW_STAKE_PROCESS_H

#include <GameProcess.h>
#include <d3dx9math.h>
class Instance2D;

typedef enum
{
	SHOW_OPTIONS,
	HIDE_OPTIONS,
	UPDATE_OPTIONS
}ChangeType;

class ShowStakeOptionsProcess : public GameProcess
{
public:
	ShowStakeOptionsProcess(const std::vector<D3DXVECTOR2>& positions, ChangeType type, int oldStakeID);
	~ShowStakeOptionsProcess();
	void Init() override;
	void Complete() override;
	void Update() override;
private:
	std::vector<Instance2D*> pOptions;
	std::vector<D3DXVECTOR2> mEndPositions;
	std::vector<bool> mInPosition;
	Instance2D* pStakeButton;
	Instance2D* pOptionOverlay;
	ChangeType mChangeType;
	int mOldStakeID;
	float mSpeed;
	float mBaseMoveSpeed;
};

#endif