/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003      Brian Thomas <thomas@mail630.gsfc.nasa.gov>   *
 *   copyright (C) 2004-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef CODEBLOCK_H
#define CODEBLOCK_H

#include "textblock.h"

/**
 * A "chunk" of code within the code document.
 */
class CodeBlock : public TextBlock
{
public:

    /**
     * Describes the creation of the content.
     * @enum AutoGenerated  the content was generated by code generation itself
     * @enum UserGenerated  the content was entered by the user
     */
    enum ContentType {AutoGenerated=0, UserGenerated };

    explicit CodeBlock ( CodeDocument * parent, const QString & body = "");
    virtual ~CodeBlock ( );

    void setContentType ( ContentType new_var );

    ContentType getContentType ( ) const;

    static QString enumToString(const ContentType& val);

    virtual void saveToXMI ( QDomDocument & doc, QDomElement & root );
    virtual void loadFromXMI ( QDomElement & root );

    virtual void setAttributesFromObject (TextBlock * obj);

protected:

    virtual void setAttributesOnNode ( QDomDocument & doc, QDomElement & blockElement);

    virtual void setAttributesFromNode ( QDomElement & element);

    friend QTextStream& operator<<(QTextStream& str, const CodeBlock& obj);

private:

    // specifies whether the content (text) of this object
    // was generated by the code generator or was supplied by the user (or some other way).
    ContentType m_contentType;

};

QTextStream& operator<<(QTextStream& str, const CodeBlock& obj);

#endif // CODEBLOCK_H
