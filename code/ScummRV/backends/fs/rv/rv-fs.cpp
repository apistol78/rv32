/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header: /cvsroot/scummvm/scummvm/backends/fs/palmos/palmos-fs.cpp,v 1.4 2004/01/20 14:35:37 chrilith Exp $
 */

#if defined(__RV__)

#include "stdafx.h"
#include "../fs.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "Runtime/File.h"

class RvFilesystemNode : public FilesystemNode
{
public:
	RvFilesystemNode() = default;
	RvFilesystemNode(const RvFilesystemNode *node);

	virtual String displayName() const { return m_displayName.c_str(); }
	virtual bool isValid() const { return true; }
	virtual bool isDirectory() const { return m_isDirectory; }
	virtual String path() const { return m_path.c_str(); }

	virtual FSList *listDir(ListMode) const;
	virtual FilesystemNode *parent() const;
	virtual FilesystemNode *clone() const { return new RvFilesystemNode(this); }

private:
	std::string m_displayName = "Root";
	std::string m_path = "\\";
	bool m_isDirectory = true;
};

FilesystemNode *FilesystemNode::getRoot()
{
	printf("FilesystemNode::getRoot\n");
	return new RvFilesystemNode();
}

RvFilesystemNode::RvFilesystemNode(const RvFilesystemNode* node)
:	m_displayName(node->m_displayName)
,	m_path(node->m_path)
,	m_isDirectory(node->m_isDirectory)
{
}

FSList *RvFilesystemNode::listDir(ListMode mode) const
{
	printf("FilesystemNode::listDir %s\n", m_path.c_str());
	FSList* list = new FSList();

	file_enumerate(m_path.c_str(), list, [](void* user, const char* filename, uint32_t size, uint8_t directory)
	{
		if (strcmp(filename, ".") == 0 || strcmp(filename, ".."))
			return;

		FSList* list = (FSList*)user;

		RvFilesystemNode entry;
		entry.m_displayName = filename;
		entry.m_path = filename;
		entry.m_isDirectory = (bool)(directory != 0);
		list->push_back(entry);
	});

	return list;
}

FilesystemNode *RvFilesystemNode::parent() const
{
	printf("FilesystemNode::parent\n");
	
	RvFilesystemNode* p = new RvFilesystemNode();

	p->m_displayName = m_displayName;
	p->m_path = m_path;
	p->m_isDirectory = true;

	return p;
}

#endif // __RV__
