#ifndef	POP_CONTROL_H
#define POP_CONTROL_H

#include <vector>
#include <d3dx9math.h>

class PopControl
{
public:
	PopControl();
	~PopControl();
	void ClosePopOptions(unsigned int stakeID);
	void ChangeStake(unsigned int stakeID);
	void ShowHide(unsigned int stakeID);
	void Initialise(const std::vector<unsigned int>& stakes, unsigned int defaultStakeID);
	bool AreOptionsVisible() const;
private:
	std::vector<D3DXVECTOR2> m_popPositions;
	bool m_popOptionsVisible;
};

#endif