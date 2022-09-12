#if defined(__RV__)

#include "stdafx.h"
#include "../fs.h"

#include <algorithm>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include "Runtime/File.h"

class RvFilesystemNode : public FilesystemNode
{
public:
	RvFilesystemNode() = default;
	RvFilesystemNode(const RvFilesystemNode *node);

	virtual String displayName() const
	{
		std::string data = m_fileName;
		std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c){ return std::tolower(c); });		
		return data.c_str();
	}

	virtual bool isValid() const { return true; }
	virtual bool isDirectory() const { return m_isDirectory; }
	virtual String path() const { return m_path.c_str(); }

	virtual FSList *listDir(ListMode) const;
	virtual FilesystemNode *parent() const;
	virtual FilesystemNode *clone() const { return new RvFilesystemNode(this); }

private:
	std::string m_path = "";
	std::string m_fileName = "";
	bool m_isDirectory = true;
};

FilesystemNode *FilesystemNode::getRoot()
{
	return new RvFilesystemNode();
}

RvFilesystemNode::RvFilesystemNode(const RvFilesystemNode* node)
:	m_path(node->m_path)
,	m_fileName(node->m_fileName)
,	m_isDirectory(node->m_isDirectory)
{
}

FSList *RvFilesystemNode::listDir(ListMode mode) const
{
	FSList* list = new FSList();

	struct User
	{
		const RvFilesystemNode* parent;
		FSList* list;
	}
	user = { this, list };

	file_enumerate(m_path.c_str(), &user, [](void* user, const char* filename, uint32_t size, uint8_t directory)
	{
		User* up = (User*)user;

		if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0)
			return;

		RvFilesystemNode entry;
		
		entry.m_path = up->parent->m_path.empty() ? filename : up->parent->m_path + "\\" + std::string(filename);
		entry.m_fileName = filename;

		printf("... path %s\n", entry.m_path.c_str());
		printf("... name %s\n", entry.m_fileName.c_str());

		entry.m_isDirectory = (bool)(directory != 0);

		up->list->push_back(entry);
	});

	return list;
}

FilesystemNode *RvFilesystemNode::parent() const
{
	RvFilesystemNode* p = new RvFilesystemNode();

	size_t s = m_path.find_last_of('\\');
	if (s != m_path.npos)
	{
		p->m_path = m_path.substr(0, s);
		p->m_fileName = m_path.substr(s + 1);
		p->m_isDirectory = true;
	}
	else
	{
		p->m_path = "";
		p->m_fileName = m_path;
		p->m_isDirectory = true;
	}

	printf("parent\n");
	printf("... path %s\n", p->m_path.c_str());
	printf("... name %s\n", p->m_fileName.c_str());

	return p;
}

#endif // __RV__
