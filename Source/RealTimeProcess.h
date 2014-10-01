#ifndef GameProcess_H
#define GameProcess_H

class GameProcess
{
	friend class ProcessManager;
public:	

	enum State
	{
		UNINITIALIZED, 
		REMOVED,
		RUNNING,
		PRIORITY_RUN,
		PAUSED,
		COMPLETED,
		FAILED,
		ABORTED,
	};

	GameProcess();
	virtual ~GameProcess();

	//Set the process state
	bool SetCompleted();
	bool SetFail();
	bool SetPause();
	bool SetUnPause();

	//Check the process state
	State GetState() const;
	bool IsAlive() const;
	bool IsDead() const;
	bool IsRemoved() const;
	bool IsPaused() const;
	bool IsPriority() const;

protected:
	//For overloading
	virtual void Init() {m_State = RUNNING;} 
	virtual void Update() = 0;
	virtual void Complete() { } 
	virtual void Abort() { }
	virtual void Failed() { }

private:
	void SetState(State state) {m_State = state;}

private:
	State m_State;  
	unsigned int m_Time;
};


#endif