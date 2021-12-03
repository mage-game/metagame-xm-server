
#ifndef CACHENODE_H
#define CACHENODE_H


class CachedNode
{
public:
	CachedNode(const DataAdapter&data):m_node(data), m_update_mode(0)
	{
	}
	/*void SetNode(const DataAdapter& node)
	{
		m_node.Update(node);
	}*/
	DataAdapter* GetNode()
	{
		return &m_node;
	}
	void SetUpdateMode(int update_mode)
	{
		m_update_mode = update_mode;
	}

	int GetUpdateMode() const
	{
		return m_update_mode;
	}
private:
	DataAdapter m_node;
	int m_update_mode;
};

#endif

