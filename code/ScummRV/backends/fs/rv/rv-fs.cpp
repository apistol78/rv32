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
#include <stdio.h>
#include <stdlib.h>

class RvFilesystemNode : public FilesystemNode {
public:
	RvFilesystemNode();
	RvFilesystemNode(const RvFilesystemNode *node);

	virtual String displayName() const { return ""; }
	virtual bool isValid() const { return false; }
	virtual bool isDirectory() const { return false; }
	virtual String path() const { return ""; }

	virtual FSList *listDir(ListMode) const;
	virtual FilesystemNode *parent() const;
	virtual FilesystemNode *clone() const { return new RvFilesystemNode(this); }
};

FilesystemNode *FilesystemNode::getRoot() {
	return new RvFilesystemNode();
}

RvFilesystemNode::RvFilesystemNode() {
}

RvFilesystemNode::RvFilesystemNode(const RvFilesystemNode *node) {
}

FSList *RvFilesystemNode::listDir(ListMode mode) const {
	FSList *myList = new FSList();
	return myList;
}

FilesystemNode *RvFilesystemNode::parent() const {
	RvFilesystemNode *p = new RvFilesystemNode();
	return p;
}

#endif // __RV__
