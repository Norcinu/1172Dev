#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <list>
#include <queue>
#include <GameProcess.h>

class ProcessManager
{
public:
	ProcessManager();
	~ProcessManager();
	void ClearListProcesses();
	void ClearQueueProcesses();
	void UpdateProcesses();
	void AddProcessToList(GameProcess* GameProcess, bool Priority);
	void AddProcessToQueue(GameProcess* GameProcess);
	void PauseAllProcesses();
	unsigned int GetNumListProcesses() const;
	unsigned int GetNumQueueProcesses() const;
private:
	void ClearAllProcesses(); 
private:	
	typedef std::list<GameProcess*> ProcessList;
	typedef std::queue<GameProcess*> ProcessQueue;
	ProcessList m_processList;
	ProcessQueue m_processQueue;
	GameProcess* m_priorityProcess;
};


#endif