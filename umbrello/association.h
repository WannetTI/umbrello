/***************************************************************************
 *									   *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.				   *
 *									   *
 ***************************************************************************/

#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include "umlnamespace.h"
#include "umlobject.h"

class UMLDoc;
class UMLRole;


/**
 *	This class contains the non-graphic representation of an association.
 *	An association can be an aggregation or composition.
 *
 *	@short Sets up association information.
 *	@author Oliver Kellogg <okellogg@users.sourceforge.net>
 *	@see	UMLObject
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */

class UMLAssociation : public UMLObject {
	friend class AssociationWidget;
	Q_OBJECT
public:
	/**
	 *	Sets up an association.
	 *
	 *	@param	parent	The parent of this UMLAssociation.
	 */
	UMLAssociation(UMLDoc* parent);

	/**
	 * 	Overloaded '==' operator
	 */
	bool operator==(UMLAssociation &rhs);

	/**
	 *	Standard deconstructor.
	 */
	~UMLAssociation() {}

	/**
	 *	Converts a Uml::Association_Type to its string representation.
	 *
	 *	@param		The assocType.
	 *	@return		The string representation of the assocType.
	 */
	QString toString(Uml::Association_Type atype);

	/**
	 *	Converts an association type string to the corresponding
	 *	Uml::Association_Type.
	 *
	 *	@param		The string representation of the assocType.
	 *	@return		The assocType.
	 */
	Uml::Association_Type toAssocType(QString atype);

	/**
	 *	Returns the assocType of the UMLAssociation.
	 *
	 *	@return		The assocType of the UMLAssociation.
	 */
	Uml::Association_Type getAssocType() const;

	/**
	 *	Returns the UMLObject assigned to the role A.
	 *	@return		pointer to the UMLObject in role A.
	 */
	UMLObject* getObjectA();

	/**
	 *	Returns the UMLObject assigned to the role B.
	 *	@return		pointer to the UMLObject in role B.
	 */
	UMLObject* getObjectB();

	/**
	 *	Returns the ID of the UMLObject assigned to the role A.
	 *
	 *	@return		Pointer to the UMLObject of role A.
	 */
	int getRoleAId() const;

	/**
	 *	Returns the ID of the UMLObject assigned to the role B.
	 *
	 *	@return		Pointer to the UMLObject of role B.
	 */
	int getRoleBId() const;

	/**
	 *	Returns the Changeablity of role A.
	 *
	 *	@return		Changeability_Type of roleA.
	 */
	Changeability_Type getChangeabilityA() const;

	/**
	 *	Returns the Changeablity of role B.
	 *
	 *	@return		Changeability_Type of roleB.
	 */

	Changeability_Type getChangeabilityB() const;

	/**
	 *	Returns the Visibility of role A.
	 *
	 *	@return		Scope of roleA.
	 */
	Scope getVisibilityA() const;

	/**
	 *	Returns the Visibility of role B.
	 *	
	 *	@return		Scope of roleB.
	 */
	Scope getVisibilityB() const;

	/**
	 *	Returns the multiplicity assigned to the role A.
	 *
	 *	@return		The multiplicity assigned to the role A.
	 */
	QString getMultiA() const;

	/**
	 *	Returns the multiplicity assigned to the role B.
	 *
	 *	@return		The multiplicity assigned to the role B.
	 */
	QString getMultiB() const;

	/**
	 *	Returns the name assigned to the role A.
	 *
	 *	@return		The name assigned to the role A.
	 */
	QString getRoleNameA() const;

	/**
	 *	Returns the name assigned to the role B.
	 *
	 *	@return		The name assigned to the role B.
	 */
	QString getRoleNameB() const;

	/**
	 *	Returns the documentation assigned to the role A.
	 */
	QString getRoleADoc() const;

	/**
	 *	Returns the documentation assigned to the role B.
	*/
	QString getRoleBDoc() const;

	/**
	 *	Sets the assocType of the UMLAssociation.
	 *
	 *	@param	assocType	The assocType of the UMLAssociation.
	 */
	void setAssocType(Uml::Association_Type assocType);

	/**
	 *	Sets the ID of the UMLObject playing role A in the association.
	 *	@param	id of the UMLObject of role A.
	 */
	void setRoleAId(int roleA);

	/**
	 *	Sets the ID of the UMLObject playing role B in the association.
	 *	@param	id of the UMLObject of role B.
	 */
	void setRoleBId(int roleB);

	/**
	 *	Sets the UMLObject playing role A in the association.
	 *	@param	pointer to the UMLObject of role A.
	 */
	void setObjectA(UMLObject *obj);

	/**
	 *	Sets the UMLObject playing role B in the association.
	 *	@param	pointer to the UMLObject of role B.
	 */
	void setObjectB(UMLObject *obj);

	/**
	 *	Sets the visibility of role A of the UMLAssociation.
	 *
	 *	@param	value 	Scope of role A visibility.
	 */
	void setVisibilityA(Scope value);

	/**
	 *	Sets the visibility of role B of the UMLAssociation.
	 *
	 *	@param	value 	Scope of role A visibility.
	 */
	void setVisibilityB(Scope value);

	/**
	 *	Sets the changeability of role A of the UMLAssociation.
	 *
	 *	@param	value 	Changeability_Type of role A changeability.
	 */
	void setChangeabilityA(Changeability_Type value);

	/**
	 *	Sets the changeability of role B of the UMLAssociation.
	 *
	 *	@param	value 	Changeability_Type of role B changeability.
	 */

	void setChangeabilityB(Changeability_Type value);
		 
	/**
	 *	Sets the multiplicity of role A of the UMLAssociation.
	 *
	 *	@param	multiA	The multiplicity of role A of the UMLAssociation.
	 */
	void setMultiA(QString multiA);

	/**
	 *	Sets the multiplicity of role B of the UMLAssociation.
	 *
	 *	@param	multiB	The multiplicity of role B of the UMLAssociation.
	 */
	void setMultiB(QString multiB);

	/**
	 *	Sets the name of role A of the UMLAssociation.
	 *
	 *	@param	roleNameA	The name of role A of the UMLAssociation.
	 */
	void setRoleNameA(QString roleNameA);

	/**
	 * 	Sets the documentation on role A in the association.
	 * 	@param	doc	The string with the documentation.
	 */
	void setRoleADoc(QString doc);

	/**
	 *	Sets the name of role B of the UMLAssociation.
	 *
	 *	@param	roleNameB	The name of role B of the UMLAssociation.
	 */
	void setRoleNameB(QString roleNameB);

	/**
	 * 	Sets the documentation on role B in the association.
	 * 	@param	doc	The string with the documentation.
	 */
	void setRoleBDoc(QString doc);

	bool saveToXMI(QDomDocument& qDoc, QDomElement& qElement);

	bool loadFromXMI(QDomElement& element);

	/**
         * Convert Changeability_Type value into QString representation. 
	 */
	static QString ChangeabilityToString(Uml::Changeability_Type type);

        /**
         * Convert Scope value into QString representation. 
         */
	static QString ScopeToString(Uml::Scope scope);

	/**
	 * Get the respective, underlying UMLRole object for either role A or B.
	 */
	UMLRole * getUMLRoleA(); 

	UMLRole * getUMLRoleB(); 

protected:

	/** do some initialization at construction time */
	void init ();

	/**
	 * If the type Uml::Association_Type is changed then also the following
	 * must be changed accordingly:
	 * atypeFirst, atypeLast, assocTypeStr[], toAssocType(), toString().
	 * The ordering within assocTypeStr must match the enumeration
	 * order of Uml::Association_Type.
	 */
	static const Uml::Association_Type atypeFirst;
	static const Uml::Association_Type atypeLast;
	static const unsigned nAssocTypes;
	static const QString assocTypeStr[];

	UMLRole * m_pRoleA;
	UMLRole * m_pRoleB;

	Uml::Association_Type m_AssocType;
	QString m_Name;

/*
	int m_RoleAId;
	int m_RoleBId;
	QString m_RoleADoc, m_RoleBDoc;
	UMLObject* m_pObjectA;
	UMLObject* m_pObjectB;
	QString m_RoleNameA, m_RoleNameB;
	QString m_MultiA, m_MultiB;
	Scope m_VisibilityA, m_VisibilityB;
	Changeability_Type m_ChangeabilityA, m_ChangeabilityB;
*/

};

#endif
