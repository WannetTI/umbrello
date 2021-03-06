/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2012                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef CODECLASSFIELDLIST_H
#define CODECLASSFIELDLIST_H

#include <QList>

// forward declarations
class CodeClassField;

typedef QList<CodeClassField*> CodeClassFieldList;
typedef QListIterator<CodeClassField*> CodeClassFieldListIt;

#endif
