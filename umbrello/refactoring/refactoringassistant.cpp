/***************************************************************************
                          refactoringassistant.cpp
                             -------------------
    copyright            : (C) 2003 Luis De la Parra
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "refactoringassistant.h"

#include "../umlnamespace.h"
#include "../umldoc.h"
#include "../classifier.h"
#include "../class.h"
#include "../interface.h"
#include "../attribute.h"
#include "../operation.h"
#include "../dialogs/classpropdlg.h"
#include "../dialogs/umloperationdialog.h"
#include "../dialogs/umlattributedialog.h"

#include <qpoint.h>
#include <qpopupmenu.h>

#include <typeinfo>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kdebug.h>

using std::type_info;


namespace Umbrello {


RefactoringAssistant::RefactoringAssistant( UMLDoc *doc, UMLClassifier *obj, QWidget *parent, const char *name ):
		KListView( parent, name ), m_doc( doc )
{
	loadPixmaps();

	setRootIsDecorated( true );
	setAcceptDrops( true );
	setDropVisualizer( false );
	setItemsMovable( true );
	setSelectionModeExt( Single );
	setShowToolTips( true );
	setTooltipColumn( 0 );
	setDragEnabled( true );
	setDropHighlighter( true );
	setFullWidth( true );
	setSorting( -1 );

	addColumn("Name ");

	m_menu = new QPopupMenu(this);

	connect(this,SIGNAL(doubleClicked(QListViewItem*)),this,SLOT(itemExecuted(QListViewItem*)));
	connect(this,SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
		this,SLOT(showContextMenu(KListView*,QListViewItem*,const QPoint&)));

	resize(300,400);
	
	refactor( obj );
}

RefactoringAssistant::~RefactoringAssistant()
{
	m_umlObjectMap.clear();
	clear();
}

void RefactoringAssistant::refactor( UMLClassifier *obj )
{
	clear();
	m_umlObjectMap.clear();
	m_umlObject = obj;
	if (! m_umlObject )
	{
		return;
	}

	addClassifier( obj, 0, true, true, true );
	QListViewItem *item = firstChild();
	item->setOpen(true);
	for( item = item->firstChild(); item ; item = item->nextSibling() )
		item->setOpen(true);
}


UMLObject* RefactoringAssistant::findUMLObject( const QListViewItem *item )
{
	QListViewItem *i = const_cast<QListViewItem*>(item);
	if( m_umlObjectMap.find(i) == m_umlObjectMap.end() )
	{
		kdWarning()<<"RefactoringAssistant::findUMLObject( QListViewItem *item )"
			   <<"item with text "<<item->text(0)<<"not found in uml map!"<<endl;
		return 0L;
	}
	return m_umlObjectMap[i];

}

QListViewItem* RefactoringAssistant::findListViewItem( const UMLObject *obj )
{
	UMLObjectMap::iterator it;
	for( it = m_umlObjectMap.begin() ; it != m_umlObjectMap.end() ; ++it )
		if( (*it).second == obj )
			return (*it).first;
	kdWarning()<<"RefactoringAssistant::findListViewItem( UMLObject *obj )"
			   <<"object id "<<obj->getID()<<"does not have s ListItem"<<endl;
	return 0L;
}


void RefactoringAssistant::itemExecuted( QListViewItem *item )
{
	UMLObject *o = findUMLObject( item );
	if(o) editProperties( );
}

void RefactoringAssistant::setVisibilityIcon( QListViewItem *item , const UMLObject *obj )
{
	switch(obj->getScope())
	{
		case Uml::Public:
			item->setPixmap(0,m_pixmaps.Public);
			break;
		case Uml::Protected:
			item->setPixmap(0,m_pixmaps.Protected);
			break;
		case Uml::Private:
			item->setPixmap(0,m_pixmaps.Private);
			break;
	}
}

void RefactoringAssistant::umlObjectModified( const UMLObject *obj )
{
	if( !obj )
		obj = dynamic_cast<const UMLObject*>(sender());
	QListViewItem *item = findListViewItem( obj );
	if( !item )
		return;
	item->setText( 0, obj->getName() );
	if( typeid(*obj) == typeid(UMLOperation) ||
	    typeid(*obj) == typeid(UMLAttribute) )
	{
		setVisibilityIcon( item, obj );
	}
}

void RefactoringAssistant::operationAdded( UMLOperation *op )
{
	UMLClassifier *c = dynamic_cast<UMLClassifier*>(op->parent());
	if(!c)
	{
		kdWarning()<<"RefactoringAssistant::operationAdded( UMLObject *obj ) "
			   <<" - Parent of operation is not a classifier!"<<endl;
		return;
	}
	QListViewItem *item = findListViewItem( c );
	if( !item )
	{
		return;
	}
	for( QListViewItem *folder = item->firstChild(); folder; folder = folder->nextSibling() )
	{
		if( folder->text(1) == "operations" )
		{
			item = new KListViewItem( folder, op->getName() );
			m_umlObjectMap[item] = op;
			connect( op, SIGNAL( modified() ), this, SLOT( umlObjectModified() ) );
			setVisibilityIcon( item, op );
			break;
		}
	}	
}

void RefactoringAssistant::operationRemoved( UMLOperation *op )
{
	QListViewItem *item = findListViewItem( op );
	if( !item )
	{
		return;
	}
	disconnect( op, SIGNAL( modified() ), this, SLOT( umlObjectModified() ) );
	m_umlObjectMap.erase(item);
	delete item;
}


void RefactoringAssistant::attributeAdded( UMLAttribute *att )
{
	UMLClass *c = dynamic_cast<UMLClass*>(att->parent());
	if(!c)
	{
		kdWarning()<<"RefactoringAssistant::attributeAdded( UMLAttribute * ) "
			   <<" - Parent is not a class!"<<endl;
		return;
	}
	QListViewItem *item = findListViewItem( c );
	if( !item )
	{
		return;
	}
	for( QListViewItem *folder = item->firstChild(); folder; folder = folder->nextSibling() )
	{
		if( folder->text(1) == "attributes" )
		{
			item = new KListViewItem( folder, att->getName() );
			m_umlObjectMap[item] = att;
			connect( att, SIGNAL( modified() ), this, SLOT( umlObjectModified() ) );
			setVisibilityIcon( item, att );
			break;
		}
	}
}

void RefactoringAssistant::attributeRemoved( UMLAttribute *att )
{
	QListViewItem *item = findListViewItem( att );
	if( !item )
	{
		return;
	}
	disconnect( att, SIGNAL( modified() ), this, SLOT( umlObjectModified() ) );
	m_umlObjectMap.erase(item);
	delete item;
}

void RefactoringAssistant::editProperties( )
{
	QListViewItem *item = selectedItem();
	if( item )
	{
		UMLObject *o = findUMLObject( item );
		if( o ) editProperties( o );
	}
}

void RefactoringAssistant::editProperties( UMLObject *obj )
{
	KDialogBase *dia(0);
	if(typeid(*obj) == typeid(UMLClass) || typeid(*obj) == typeid(UMLInterface))
	{
		dia = new ClassPropDlg(this,obj,0,true);
	}
	else if(typeid(*obj) == typeid(UMLOperation))
	{
		dia = new UMLOperationDialog(this,static_cast<UMLOperation*>(obj));
	}
	else if(typeid(*obj) == typeid(UMLAttribute))
	{
		dia = new UMLAttributeDialog(this,static_cast<UMLAttribute*>(obj));
	}
	else
	{
		kdWarning()<<"RefactoringAssistant::editProperties( UMLObject *o ) caled for unknown type "<<typeid(*obj).name()<<endl;
		return;
	}
	if( dia && dia->exec() )
	{
		// need to update something?
	}
	delete dia;
}

void RefactoringAssistant::showContextMenu(KListView* ,QListViewItem *item, const QPoint &p)
{
	m_menu->clear();
	UMLObject *obj = findUMLObject( item );
	if(obj)
	{// Menu for UMLObjects
		if(typeid(*obj) == typeid(UMLClass))
		{
		m_menu->insertItem(i18n("Add Base Class"),this,SLOT(addBaseClassifier()));
		m_menu->insertItem(i18n("Add Derived Class"),this,SLOT(addDerivedClassifier()));
// 		m_menu->insertItem(i18n("Add Interface Implementation"),this,SLOT(addInterfaceImplementation()));
		m_menu->insertItem(i18n("Add Operation"),this,SLOT(createOperation()));
		m_menu->insertItem(i18n("Add Attribute"),this,SLOT(createAttribute()));
		}
		else if(typeid(*obj) == typeid(UMLInterface))
		{
		m_menu->insertItem(i18n("Add Base Interface"),this,SLOT(addSuperClassifier()));
		m_menu->insertItem(i18n("Add Derived Interface"),this,SLOT(addDerivedClassifier()));
		m_menu->insertItem(i18n("Add Operation"),this,SLOT(createOperation()));
		}
// 		else
// 		{
// 		kdDebug()<<"No context menu for objects of type "<<typeid(*obj).name()<<endl;
// 		return;
// 		}
		m_menu->insertSeparator();
		m_menu->insertItem(i18n("Properties"),this,SLOT(editProperties()));
	}
	else
	{//menu for other ViewItems
		if( item->text(1) == "operations" )
		{
			m_menu->insertItem(i18n("Add Operation"),this,SLOT(createOperation()));
		}
		else if( item->text(1) == "attributes" )
		{
			m_menu->insertItem(i18n("Add Attribute"),this,SLOT(createAttribute()));
		}
		else
		{
			kdWarning()<<"RefactoringAssistant::showContextMenu() "
				   <<"called for extraneous item"<<endl;
			return;
		}
	}
	m_menu->exec(p);
}

void RefactoringAssistant::addBaseClassifier()
{
	QListViewItem *item = selectedItem();
	if(!item)
	{
		kdWarning()<<"RefactoringAssistant::addBaseClassifier() "
			   <<"called with no item selected"<<endl;
		return;
	}
	UMLObject *obj = findUMLObject( item );
	if( !dynamic_cast<UMLClassifier*>(obj) )
	{
		kdWarning()<<"RefactoringAssistant::addBaseClassifier() "
			   <<"called for a non-classifier object"<<endl;
		return;
	}
	
	//classes have classes and interfaces interfaces as super/derived classifiers
	UMLClassifier *super = static_cast<UMLClassifier*>(m_doc->createUMLObject( typeid(*obj) ) );
	if(!super)
		return;
	m_doc->createUMLAssociation( obj, super, Uml::at_Generalization );
//////////////////////   Manually add the classifier to the assitant - would be nicer to do it with
/////////////////////    a signal, like operations and attributes
	QListViewItem *baseFolder = item->firstChild();
	while( baseFolder->text(0) != i18n("Base Classifiers") )
		baseFolder = baseFolder->nextSibling();
	if(!baseFolder)
	{
		kdWarning()<<"Cannot find Base Folder"<<endl;
		return;
	}
	item = new KListViewItem( baseFolder, super->getName() );
	item->setPixmap(0,m_pixmaps.Generalization);
	item->setExpandable( true );
	m_umlObjectMap[item] = super;
	addClassifier( super, item, true, false, true);
/////////////////////////	
}

void RefactoringAssistant::addDerivedClassifier()
{
	QListViewItem *item = selectedItem();
	if(!item)
	{
		kdWarning()<<"RefactoringAssistant::addDerivedClassifier() "
			   <<"called with no item selected"<<endl;
		return;
	}
	UMLObject *obj = findUMLObject( item );
	if( !dynamic_cast<UMLClassifier*>(obj) )
	{
		kdWarning()<<"RefactoringAssistant::addDerivedClassifier() "
			   <<"called for a non-classifier object"<<endl;
		return;
	}
	
	//classes have classes and interfaces interfaces as super/derived classifiers
	UMLClassifier *derived = static_cast<UMLClassifier*>(m_doc->createUMLObject( typeid(*obj) ));
	if(!derived)
		return;
	m_doc->createUMLAssociation( derived, obj, Uml::at_Generalization );
	
//////////////////////   Manually add the classifier to the assitant - would be nicer to do it with
/////////////////////    a signal, like operations and attributes
	QListViewItem *derivedFolder = item->firstChild();
	while( derivedFolder->text(0) != i18n("Derived Classifiers") )
		derivedFolder = derivedFolder->nextSibling();
	if(!derivedFolder)
	{
		kdWarning()<<"Cannot find Derived Folder"<<endl;
		return;
	}
	item = new KListViewItem( derivedFolder, derived->getName() );
	item->setPixmap(0,m_pixmaps.Subclass);
	item->setExpandable( true );
	m_umlObjectMap[item] = derived;
	addClassifier( derived, item, false, true, true);
/////////////////////////	
}

void RefactoringAssistant::addInterfaceImplementation()
{
	kdWarning()<<"RefactoringAssistant::addInterfaceImplementation()"
		   <<"not implemented... finish addSuperClassifier() first!!"<<endl;
	return;
// 	QListViewItem *item = selectedListViewItem( );
// 	UMLObject *obj = findUMLObject( item );
// 	if( !dynamic_cast<UMLClassifier*>(obj) )
// 		return;
// 	UMLObject *n = m_doc->createUMLObject( typeid(UMLInterface) );
// 	if(!n)
// 		return;
// 	m_doc->createUMLAssociation( n, obj, Uml::at_Realization );
// 	//refresh, add classifier to assistant	
}

void RefactoringAssistant::createOperation()
{
	QListViewItem *item = selectedItem();
	if(!item)
	{
		kdWarning()<<"RefactoringAssistant::createOperation() "
			   <<"called with no item selected"<<endl;
		return;
	}
	UMLClassifier *c = dynamic_cast<UMLClassifier*>(findUMLObject( item ));
	if( !c )
		return;
	m_doc->createOperation( c );
}

void RefactoringAssistant::createAttribute()
{
	QListViewItem *item = selectedItem();
	if(!item)
	{
		kdWarning()<<"RefactoringAssistant::createAttribute() "
			   <<"called with no item selected"<<endl;
		return;
	}
	UMLClass *c = dynamic_cast<UMLClass*>(findUMLObject( item ));
	if( !c )
		return;
	m_doc->createAttribute( c );
}


void RefactoringAssistant::addClassifier( UMLClassifier *classifier, QListViewItem *parent, bool addSuper, bool addSub, bool recurse)
{
	QListViewItem *classifierItem, *item;
	if( parent )
	{
		classifierItem = parent;
	}
	else
	{
		classifierItem= new KListViewItem( this, classifier->getName() );
		m_umlObjectMap[classifierItem] = classifier;
	}

	connect( classifier, SIGNAL( modified() ), this, SLOT( umlObjectModified() ) );
	
	if( dynamic_cast<UMLClass*>(classifier) )
	{// only Classes have attributes...
		connect( classifier, SIGNAL(attributeAdded(UMLAttribute*)),
			this,SLOT( attributeAdded(UMLAttribute*)));
		connect( classifier, SIGNAL(attributeRemoved(UMLAttribute*)),
			this,SLOT( attributeRemoved(UMLAttribute*)));
			
		QListViewItem *attsFolder = new KListViewItem( classifierItem, i18n("Attributes"), "attributes" );
		attsFolder->setPixmap(0,SmallIcon("folder_green_open"));
		attsFolder->setExpandable( true );
		UMLAttributeList* atts = static_cast<UMLClass*>(classifier)->getFilteredAttributeList();
		for( UMLAttribute *att = atts->first(); att; att = atts->next() )
		{
			attributeAdded( att );
		}
		
	}
	
	// add operations
	connect( classifier, SIGNAL(operationAdded(UMLOperation*)),
		this,SLOT( operationAdded(UMLOperation*)));
	connect( classifier, SIGNAL(operationRemoved(UMLOperation*)),
		this,SLOT( operationRemoved(UMLOperation*)));
		
	QListViewItem *opsFolder = new KListViewItem( classifierItem, i18n("Operations"), "operations" );
	opsFolder->setPixmap(0,SmallIcon("folder_blue_open"));
	opsFolder->setExpandable( true );
	UMLOperationList *ops = classifier->getFilteredOperationsList();
	for( UMLOperation *op = ops->first(); op ; op = ops->next() )
	{
		operationAdded( op );
	}

	//if add parents
	if(addSuper)
	{
		QListViewItem *superFolder = new KListViewItem( classifierItem, i18n("Base Classifiers") );
		superFolder->setExpandable( true );
		UMLClassifierList super = classifier->findSuperClassConcepts( m_doc );
		for( UMLClassifier *cl = super.first(); cl ; cl = super.next() )
		{
			item = new KListViewItem( superFolder, cl->getName() );
			item->setPixmap(0,m_pixmaps.Generalization);
			item->setExpandable( true );
			m_umlObjectMap[item] = cl;
			if( recurse )
			{
				addClassifier( cl, item, true, false, true);
			}
	
		}
	}
	if(addSub)
	{
	//add derived classifiers
		QListViewItem *derivedFolder = new KListViewItem( classifierItem, i18n("Derived Classifiers") );
		derivedFolder->setExpandable( true );
		UMLClassifierList derived = classifier->findSubClassConcepts ( m_doc );
		for( UMLClassifier *d = derived.first(); d ; d = derived.next() )
		{
			item = new KListViewItem( derivedFolder, d->getName() );
			item->setPixmap(0,m_pixmaps.Subclass);
			item->setExpandable( true );
			m_umlObjectMap[item] = d;
			if( recurse )
			{
				addClassifier( d, item, false, true, true);
			}
	
		}
	}
}


bool RefactoringAssistant::acceptDrag(QDropEvent *event) const
{
	//first check if we can accept drops at all, and if the operation
	// is a move within the list itself
	if( !acceptDrops() || !itemsMovable() || (event->source()!=viewport()))
	{
		return false;
	}

	RefactoringAssistant *me = const_cast<RefactoringAssistant*>(this);
	
	//ok, check if the move is valid
	QListViewItem *movingItem = 0, *afterme = 0, *parentItem = 0;
	me->findDrop(event->pos(), parentItem, afterme);	
	for( movingItem = firstChild(); movingItem != 0; movingItem = movingItem->itemBelow() )
	{
		if( movingItem->isSelected() )
			break;
	}
	if(!movingItem || !parentItem)
	{	kdDebug()<<"moving/parent items not found - can't accept drag!"<<endl;
		return false;
	}
	
	UMLObject *movingObject;
	if( !(movingObject = me->findUMLObject(movingItem)) )
	{
		kdDebug()<<"Moving object not found in uml map!"<<movingItem->text(0)<<endl;
		return false;
	}
	if( (typeid(*movingObject) != typeid(UMLAttribute)) && (typeid(*movingObject) != typeid(UMLOperation)) )
	{
		kdDebug()<<"only operations and attributes are movable! - return false"<<endl;
		return false;
	}
	
	kdDebug()<<"parent item is "<<parentItem->text(0)<<endl;
	UMLObject *parentObject = me->findUMLObject(parentItem);
	if( parentObject && dynamic_cast<UMLClassifier*>(parentObject) )
	{
		//droping to a classifier, ok
	}
	else
	{//parent is not a classifier, so maybe  it's a folder.. check types
		if( ( parentItem->text(1) == "operations" && typeid(*movingObject) == typeid(UMLOperation) )
		    || ( parentItem->text(1) == "attributes" && typeid(*movingObject) == typeid(UMLAttribute)))
		{
			parentObject = me->findUMLObject( parentItem->parent() );
		}
		else
		{
			kdDebug()<<"moving to item "<<parentItem->text(0)<<" -- "<<parentItem->text(1)<<" not valid"<<endl;
			return false;	
		}
	}
	if( (typeid(*movingObject) == typeid(UMLAttribute)) && dynamic_cast<UMLClass*>( parentObject ) )
	{
		return true;
	}
	if( (typeid(*movingObject) == typeid(UMLOperation)) && dynamic_cast<UMLClassifier*>( parentObject ) )
	{
		return true;
	}
	
	kdDebug()<<"how did I get here? return false!!"<<endl;
	return false;
}


void RefactoringAssistant::movableDropEvent (QListViewItem* parentItem, QListViewItem* afterme)
{
	//when dropping on a class, we have to put the item in the appropriate folder!
	UMLObject *movingObject;
	UMLClassifier *newClassifier;
	QListViewItem *movingItem;

	for( movingItem = firstChild(); movingItem != 0; movingItem = movingItem->itemBelow() )
	{
		if( movingItem->isSelected() )
			break;
	}
	if( !movingItem || (movingItem == afterme) || !(movingObject = findUMLObject(movingItem)) )
	{
		kdWarning()<<"Moving item not found or dropping after itself or item not found in uml obj map. aborting. (drop had already been accepted)"<<endl;
		return;
	}
	newClassifier = dynamic_cast<UMLClassifier*>( findUMLObject( parentItem ) );
	if(!newClassifier)
	{
		if( ( parentItem->text(1) == "operations" && typeid(*movingObject) == typeid(UMLOperation) )
		    || ( parentItem->text(1) == "attributes" && typeid(*movingObject) == typeid(UMLAttribute)))
		{
			newClassifier = dynamic_cast<UMLClassifier*>( findUMLObject( parentItem->parent() ) );
		}
		if(!newClassifier)
		{
			kdWarning()<<"New parent of object is not a Classifier - Drop had already been accepted - check!"<<endl;
			return;
		}
	}	
	if( typeid(*movingObject) == typeid(UMLOperation) )
	{kdDebug()<<"moving operation"<<endl;
		UMLOperation *op = static_cast<UMLOperation*>(movingObject);
		if(!newClassifier->checkOperationSignature(op))
		{
			QString msg = QString(i18n("An operation with that signature already exists in %1.\n")).arg(newClassifier->getName())
				+
			      QString(i18n("Choose a different name or parameter list." ));
			KMessageBox::error(this, msg, i18n("Operation Name Invalid"), false);
			return;
		}
		UMLClassifier *oldClassifier = dynamic_cast<UMLClassifier*>(op->parent());
		if(oldClassifier)
			oldClassifier->removeOperation( op );
		newClassifier->addOperation( op );
	}
	else if( typeid(*movingObject) == typeid(UMLAttribute) )
	{kdDebug()<<"moving attribute - not implemented"<<endl;
// 		UMLAttribute *att = static_cast<UMLAttribute*>(movingObject);
// 		UMLClass *newClass = dynamic_cast<UMLClass*>(newClassifier);
// 		if(!newClass)
// 		{
// 			kdWarning()<<"ay"<<endl;
// 			return;
// 		}
// 		if(!newClass->checkAttributeSignature(att))
// 		{
// 			QString msg = QString(i18n("An attribute with that signature already exists in %1.\n")).arg(newClassifier->getName())
// 				+
// 			      QString(i18n("Choose a different name or parameter list." ));
// 			KMessageBox::error(this, msg, i18n("Operation Name Invalid"), false);
// 			return;
// 		}
// 		UMLClass *oldClass = dynamic_cast<UMLClass*>(att->parent());
// 		if(oldClass)
// 			oldClassifier->removeAttribute( att );
// 		newClass->addAttribute( att );
	}
	//emit moved(moving, afterFirst, afterme);
	emit moved();
}

void RefactoringAssistant::loadPixmaps()
{
	KStandardDirs *dirs = KGlobal::dirs();
	QString dataDir = dirs -> findResourceDir( "data", "umbrello/pics/public.png" );
	dataDir += "/umbrello/pics/";

	m_pixmaps.Public.load( dataDir + "public.png" );
	m_pixmaps.Protected.load( dataDir + "protected.png" );
	m_pixmaps.Private.load( dataDir + "private.png" );
	m_pixmaps.Generalization.load( dataDir + "generalization.xpm" );
	m_pixmaps.Subclass.load( dataDir + "uniassoc.xpm" );


}

}  //namespace Umbrello





#include "refactoringassistant.moc"
