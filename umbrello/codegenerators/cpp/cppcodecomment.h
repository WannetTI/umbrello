/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003      Brian Thomas <thomas@mail630.gsfc.nasa.gov>   *
 *   copyright (C) 2004-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef CPPCODECOMMENT_H
#define CPPCODECOMMENT_H

#include "codecomment.h"

#include <QString>

/**
 * A CPP code comment. There is only a single styles of comments:
 * these are simply started with double slash sequence and no terminating
 * characters.
 */
class CPPCodeComment : virtual public CodeComment
{
public:

    /**
     * Constructors.
     */
    explicit CPPCodeComment (CodeDocument * doc, const QString & text = QString());

    /**
     * Empty Destructor.
     */
    virtual ~CPPCodeComment ();

    /**
     * Save the XMI representation of this object.
     */
    virtual void saveToXMI1 (QDomDocument & doc, QDomElement & root);

    /**
     * @return  QString
     */
    QString toString () const;

    /**
     * UnFormat a long text string. Typically, this means removing
     * the indentation (linePrefix) and/or newline chars from each line.
     */
    virtual QString unformatText (const QString & text, const QString & indent = QString());

    /**
     * A special version here because we want to not only indent
     * the new line, but to add the "//" sequence as well.
     */
    virtual QString getNewEditorLine (int amount);

};

#endif // CPPCODECOMMENT_H
