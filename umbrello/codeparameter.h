
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by: 
 *      Author : thomas
 *      Date   : Fri Jun 20 2003
 */



#ifndef CODEPARAMETER_H
#define CODEPARAMETER_H


#include "umlnamespace.h"
#include "codecomment.h"

/**
  * class CodeParameter
  * A parameter on some type of code.
  */

class CodeGenerator;
class ClassifierCodeDocument;
class UMLObject;

class CodeParameter : public QObject  
{
	Q_OBJECT
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	CodeParameter ( ClassifierCodeDocument * doc, UMLObject * parentObj );

	/**
	 * Empty Destructor
	 */
	virtual ~CodeParameter ( );

	// Public attributes
	//  

	// Public attribute accessor methods
	//  

        /**
         * Get the parent Code Document
         */
        ClassifierCodeDocument * getParentDocument ( );

        /**
         * Get the parent UMLObject
         */
        UMLObject * getParentObject ( );

       /**
         * Utility method. Get the value of parent abstract value.
         * @return the value
         */
        bool getAbstract ( );

        /**
         * Utility method. Get the value of parent Static
         * Whether or not this is static.
         * @return the value
         */
        bool getStatic ( );

	/**
	 * The name of this code parameter.
	 * @return the value of parameter name
	 */
	QString getName ( ) const;

	/**
	 * Get the value of m_typeName
	 * the typeName of this parameters (e.g. boolean, int, etc or perhaps Class name of
	 * an object)
	 * @return the value of m_typeName
	 */
	virtual QString getTypeName ( );

	/**
	 * Get the value of m_visibility
	 * The visibility of this code parameter.
	 * @return the value of m_visibility
	 */
	Uml::Scope getVisibility ( ) const;

	/**
	 * Set the value of m_initialValue
	 * The initial value of this code parameter
	 * @param new_var the new value of m_initialValue
	 */
	virtual void setInitialValue ( QString new_var );

	/**
	 * Get the value of m_initialValue
	 * The initial value of this code parameter
	 * @return the value of m_initialValue
	 */
	virtual QString getInitialValue ( );

	/**
	 * Set a Comment 
	 */
	void setComment ( CodeComment * comment );

	/**
	 * get any Comment object on this 
	 */
	CodeComment * getComment ( );

	// the id of this parameter is the same as the parent UMLObject id.
	QString getID (); 

protected:

        // a little utility method..
        CodeGenerator * getParentGenerator ( );

	virtual void updateContent() = 0;

        /** set attributes of the node that represents this class
         * in the XMI document.
         */
        virtual void setAttributesOnNode ( QDomDocument & doc, QDomElement & blockElement);

        /** set the class attributes of this object from
         * the passed element node.
         */
        virtual void setAttributesFromNode ( QDomElement & element);

private:

	ClassifierCodeDocument * m_parentDocument;
	UMLObject * m_parentObject;
	CodeComment * m_comment;

	// The initial value of this code parameter
	QString m_initialValue;

	void initFields ( ClassifierCodeDocument * doc, UMLObject * obj);

public slots:

	void syncToParent ();

};

#endif // CODEPARAMETER_H
