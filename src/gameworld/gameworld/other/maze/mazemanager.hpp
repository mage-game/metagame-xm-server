#ifndef __MAZE_MANAGER_HPP__
#define __MAZE_MANAGER_HPP__

#include "servercommon/mazedef.hpp"
#include "obj/character/attribute.hpp"

class Role;

class MazeManager
{
public:
	MazeManager();
	~MazeManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const MazeParam &maze_param);
	void GetOtherInitParam(MazeParam *param);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void StartMaze();
	void BuyMove();
	void OnQueryMaze(short reason = 0);
	void OnMoveReq(int target_x, int target_y);

	void OnFetchJifenReward(int seq);

private:
	Role *m_role;

	void SetBox(int x, int y);
	void SetBomb(int x, int y);
	void SetOpen(int x, int y);

	bool IsBox(int x, int y);
	bool IsBomb(int x, int y);
	bool IsOpen(int x, int y);
	
	bool IsValidPos(int x, int y);
	bool Connectable(int src_x, int src_y, int next_x, int next_y);

	void RandShowBox();
	void PrintMaze();

	void SendMoveResult(int result, int param);
	
	MazeParam m_maze_param;	
};

#endif // __MOUNT_MANAGER_HPP__


