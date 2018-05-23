/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

// own header
#include "listpopupmenu.h"

// app includes
#include "activitywidget.h"
#include "associationline.h"
#include "associationwidget.h"
#include "category.h"
#include "classifier.h"
#include "classifierwidget.h"
#include "combinedfragmentwidget.h"
#include "debug_utils.h"
#include "floatingtextwidget.h"
#include "folder.h"
#include "forkjoinwidget.h"
#include "layoutgenerator.h"
#include "model_utils.h"
#include "objectnodewidget.h"
#include "objectwidget.h"
#include "notewidget.h"
#include "pinportbase.h"
#include "preconditionwidget.h"
#include "signalwidget.h"
#include "statewidget.h"
#include "uml.h"
#include "umldoc.h"
#include "umlscene.h"
#include "umlview.h"
#include "umllistview.h"
#include "umllistviewitem.h"
#include "widget_utils.h"
#include "widgetbase.h"

// kde includes
#include <KLocalizedString>
#include <kactioncollection.h>

DEBUG_REGISTER_DISABLED(ListPopupMenu)

static const bool CHECKABLE = true;

// uncomment to see not handled switch cases
//#define CHECK_SWITCH

#define DEBUG_AddAction(m) m_debugActions.append(m)

/**
 * Constructs the popup menu
 *
 * @param parent   The parent to ListPopupMenu.
 * @param type     The type of menu to display.
 */
ListPopupMenu::ListPopupMenu(QWidget *parent, MenuType type)
    : KMenu(parent),
    m_isListView(false)
{
    if (type != mt_Undefined) {
        setupMenu(type);
        setupActionsData();
    }
}

/**
 * Standard destructor.
 */
ListPopupMenu::~ListPopupMenu()
{
    foreach (QAction* action, m_actions) {
        delete action;
    }
    m_actions.clear();
    m_debugActions.clear();
}

/**
 * Shortcut for the frequently used addAction() calls.
 *
 * @param m   The MenuType for which to insert a menu item.
 */
void ListPopupMenu::insert(MenuType m)
{
    insert(m, this);
}

/**
 * Shortcut for the frequently used addAction() calls.
 *
 * @param m      The MenuType for which to insert a menu item.
 * @param menu   The KMenu for which to insert a menu item.
 */
void ListPopupMenu::insert(const MenuType m, KMenu* menu)
{
    DEBUG_AddAction(m);
    Q_ASSERT(menu != 0);
    switch (m) {
    case mt_Subsystem:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Subsystem), i18n("Subsystem"));
        break;
    case mt_Component:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Component), i18n("Component"));
        break;
    case mt_Port:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Port), i18n("Port"));
        break;
    case mt_Artifact:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Artifact), i18n("Artifact"));
        break;
    case mt_Component_Diagram:
        {
            QAction* action = UMLApp::app()->actionCollection()->action(QLatin1String("new_component_diagram"));
            insert(mt_Component_Diagram, menu, action->icon(), action->text());
        }
        break;
    case mt_Node:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Node), i18n("Node"));
        break;
    case mt_Deployment_Diagram:
        {
            QAction* action = UMLApp::app()->actionCollection()->action(QLatin1String("new_deployment_diagram"));
            insert(mt_Deployment_Diagram, menu, action->icon(), action->text());
        }
        break;
    case mt_Deployment_Folder:
    case mt_Component_Folder:
    case mt_UseCase_Folder:
    case mt_EntityRelationship_Folder:
        m_actions[m] = menu->addAction(Icon_Utils::BarIcon(Icon_Utils::it_Folder), i18n("Folder"));
        break;
    case mt_Entity:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Entity), i18n("Entity"));
        break;
    case mt_EntityRelationship_Diagram:
        {
            QAction* action = UMLApp::app()->actionCollection()->action(QLatin1String("new_entityrelationship_diagram"));
            insert(mt_EntityRelationship_Diagram, menu, action->icon(), action->text());
        }
        break;
    case mt_Category:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Category), i18n("Category"));
        break;
    case mt_Actor:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Actor), i18n("Actor"));
        break;
    case mt_UseCase:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_UseCase), i18n("Use Case"));
        break;
    case mt_UseCase_Diagram:
        {
            QAction* action = UMLApp::app()->actionCollection()->action(QLatin1String("new_use_case_diagram"));
            insert(mt_UseCase_Diagram, menu, action->icon(), action->text());
        }
        break;
    case mt_FloatText:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Text), i18n("Text Line..."));
        break;
    case mt_Note:
        insert(mt_Note, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Note), i18n("Note..."));
        break;
    case mt_Properties:
        m_actions[m] = addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Properties), i18n("Properties"));
        break;
    case mt_Rename:
        m_actions[m] = addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Rename), i18n("Rename..."));
        break;
    case mt_Rename_Object:
        insert(m, i18n("Rename Object..."));
        break;
    case mt_Resize:
        insert(m, i18n("Resize"));
        break;
    case mt_Show:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Show), i18n("Show"));
        break;
    case mt_Delete:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Delete), i18n("Delete"));
        break;
    case mt_Cut:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Cut), i18n("Cut"));
        break;
    case mt_Copy:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Copy), i18n("Copy"));
        break;
    case mt_Paste:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Paste), i18n("Paste"));
        break;
    case mt_Change_Font:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Change_Font), i18n("Change Font..."));
        break;
    case mt_Line_Color:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Color_Line), i18n("Line Color..."));
        break;
    case mt_Expand_All:
        m_actions[m] = menu->addAction(i18n("Expand All"));
        break;
    case mt_Collapse_All:
        m_actions[m] = menu->addAction(i18n("Collapse All"));
        break;
    case mt_Clone:
        m_actions[m] = menu->addAction(Icon_Utils::BarIcon(Icon_Utils::it_Duplicate), i18nc("duplicate action", "Duplicate"));
        break;
    case mt_Externalize_Folder:
        m_actions[m] = menu->addAction(i18n("Externalize Folder..."));
        break;
    case mt_Internalize_Folder:
        m_actions[m] = menu->addAction(i18n("Internalize Folder"));
        break;
    case mt_Import_Class:
        m_actions[m] = menu->addAction(Icon_Utils::BarIcon(Icon_Utils::it_Import_File), i18n("Import File(s)..."));
        break;
    case mt_Import_Project:
        m_actions[m] = menu->addAction(Icon_Utils::BarIcon(Icon_Utils::it_Import_Project), i18n("Import from Directory..."));
        break;
    case mt_Reset_Label_Positions:
        m_actions[m] = menu->addAction(i18n("Reset Label Positions"));
        break;
    case mt_New_Parameter:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Parameter_New), i18n("New Parameter..."));
        break;
    case mt_New_Operation:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Operation_Public_New), i18n("New Operation..."));
        break;
    case mt_New_Attribute:
    case mt_New_InstanceAttribute:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Attribute_New), i18n("New Attribute..."));
        break;
    case mt_New_Template:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Template_New), i18n("New Template..."));
        break;
    case mt_New_EnumLiteral:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Literal_New), i18n("New Literal..."));
        break;
    case mt_New_EntityAttribute:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Entity_Attribute_New), i18n("New Entity Attribute..."));
        break;
    case mt_Export_Image:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Export_Picture), i18n("Export as Picture..."));
        break;
    case mt_InstanceAttribute:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Attribute_New), i18n("New Attribute..."));
        break;
    case mt_Remove:
        m_actions[m] = menu->addAction(Icon_Utils::SmallIcon(Icon_Utils::it_Remove), i18n("Remove"));
        break;
    default:
        uWarning() << "called on unimplemented MenuType " << toString(m);
        break;
    }
}

/**
 * Shortcut for the frequently used addAction() calls.
 *
 * @param m      The MenuType for which to insert a menu item.
 * @param icon   The icon for this action.
 * @param text   The text for this action.
 */
void ListPopupMenu::insert(const MenuType m, const QIcon & icon, const QString & text)
{
    DEBUG_AddAction(m);
    m_actions[m] = addAction(icon, text);
}

/**
 * Shortcut for the frequently used addAction() calls.
 *
 * @param m           The MenuType for which to insert a menu item.
 * @param text        The text for this action.
 * @param checkable   Sets the action to checkable.
 */
void ListPopupMenu::insert(const MenuType m, const QString & text, const bool checkable)
{
    DEBUG_AddAction(m);
    m_actions[m] = addAction(text);
    if (checkable) {
        QAction* action = getAction(m);
        if (action)
            action->setCheckable(checkable);
    }
}

/**
 * Shortcut for the frequently used addAction() calls.
 *
 * @param m      The MenuType for which to insert a menu item.
 * @param menu   The KMenu for which to insert a menu item.
 * @param icon   The icon for this action.
 * @param text   The text for this action.
 */
void ListPopupMenu::insert(const MenuType m, KMenu* menu, const QIcon & icon, const QString & text)
{
    DEBUG_AddAction(m);
    m_actions[m] = menu->addAction(icon, text);
}

/**
 * Shortcut for the frequently used addAction() calls.
 *
 * @param m      The MenuType for which to insert a menu item.
 * @param menu   The KMenu for which to insert a menu item.
 * @param text   The text for this action.
 * @param checkable   Sets the action to checkable.
 */
void ListPopupMenu::insert(const MenuType m, KMenu* menu, const QString & text, const bool checkable)
{
    DEBUG_AddAction(m);
    m_actions[m] = menu->addAction(text);
    if (checkable) {
        QAction* action = getAction(m);
        if (action)
            action->setCheckable(checkable);
    }
}

/**
 * Shortcut for inserting standard model items (Class, Interface,
 * Datatype, Enum, Package) as well as diagram choices.
 *
 * @param folderAndDiagrams Set this true if folders and diagram
 *                          types shall be included as choices.
 * @param packages          Set this true if packages
 *                          shall be included as choices.
 */
void ListPopupMenu::insertContainerItems(bool folderAndDiagrams, bool packages)
{
    KMenu* menu = new KMenu(i18nc("new container menu", "New"), this);
    menu->setIcon(Icon_Utils::SmallIcon(Icon_Utils::it_New));
    insertContainerItems(menu, folderAndDiagrams, packages);
    addMenu(menu);
}

/**
 * Shortcut for inserting standard model items (Class, Interface,
 * Datatype, Enum, Package) as well as diagram choices.
 *
 * @param menu              Menu to add the menu entries
 * @param folderAndDiagrams Set this true if folders and diagram
 *                          types shall be included as choices.
 * @param packages          Set this true if packages
 *                          shall be included as choices.
 */
void ListPopupMenu::insertContainerItems(KMenu* menu, bool folderAndDiagrams, bool packages)
{
    if (folderAndDiagrams)
        insert(mt_Logical_Folder, menu, Icon_Utils::BarIcon(Icon_Utils::it_Folder), i18n("Folder"));
    insert(mt_Class, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Class), i18nc("new class menu item", "Class"));
    insert(mt_Interface, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Interface), i18n("Interface"));
    insert(mt_Datatype, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Datatype), i18n("Datatype"));
    insert(mt_Enum, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Enum), i18n("Enum"));
    if (packages)
        insert(mt_Package, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Package), i18n("Package"));
    if (folderAndDiagrams) {
        QAction *action = UMLApp::app()->actionCollection()->action(QLatin1String("new_class_diagram"));
        insert(mt_Class_Diagram, menu, action->icon(), action->text());
        action = UMLApp::app()->actionCollection()->action(QLatin1String("new_sequence_diagram"));
        insert(mt_Sequence_Diagram, menu, action->icon(), action->text());
        action = UMLApp::app()->actionCollection()->action(QLatin1String("new_collaboration_diagram"));
        insert(mt_Collaboration_Diagram, menu, action->icon(), action->text());
        action = UMLApp::app()->actionCollection()->action(QLatin1String("new_state_diagram"));
        insert(mt_State_Diagram, menu, action->icon(), action->text());
        action = UMLApp::app()->actionCollection()->action(QLatin1String("new_activity_diagram"));
        insert(mt_Activity_Diagram, menu, action->icon(), action->text());
    }
}

/**
 * Inserts a menu item for an association related text
 * (such as name, role, multiplicity etc.)
 *
 * @param label   The menu text.
 * @param mt      The menu type.
 */
void ListPopupMenu::insertAssociationTextItem(const QString &label, MenuType mt)
{
    insert(mt, label);
    insert(mt_Change_Font);
    insert(mt_Reset_Label_Positions);
    insert(mt_Properties);
}

/**
 * Convenience method to extract the ListPopupMenu type from an action.
 * @param action   the action which was called
 * @return menu type enum value
 */
ListPopupMenu::MenuType ListPopupMenu::typeFromAction(QAction *action)
{
    ListPopupMenu *menu = ListPopupMenu::menuFromAction(action);
    if (menu) {
        return menu->getMenuType(action);
    }
    else {
        uError() << "Action's data field does not contain ListPopupMenu pointer!";
        return mt_Undefined;
    }
}

/**
 * Utility: Convert a MenuType value to an ObjectType value.
 */
UMLObject::ObjectType ListPopupMenu::convert_MT_OT(MenuType mt)
{
    UMLObject::ObjectType type =  UMLObject::ot_UMLObject;

    switch (mt) {
    case mt_UseCase:
        type = UMLObject::ot_UseCase;
        break;
    case mt_Actor:
        type = UMLObject::ot_Actor;
        break;
    case mt_Class:
        type = UMLObject::ot_Class;
        break;
    case mt_Datatype:
        type = UMLObject::ot_Datatype;
        break;
    case mt_Attribute:
        type = UMLObject::ot_Attribute;
        break;
    case mt_Interface:
        type = UMLObject::ot_Interface;
        break;
    case mt_Template:
        type = UMLObject::ot_Template;
        break;
    case mt_Enum:
        type = UMLObject::ot_Enum;
        break;
    case mt_EnumLiteral:
        type = UMLObject::ot_EnumLiteral;
        break;
    case mt_EntityAttribute:
        type = UMLObject::ot_EntityAttribute;
        break;
    case mt_Operation:
        type = UMLObject::ot_Operation;
        break;
    case mt_Category:
        type = UMLObject::ot_Category;
        break;
    case mt_InstanceAttribute:
        type = UMLObject::ot_InstanceAttribute;
        break;
    default:
        break;
    }
    return type;
}

/**
 * Returns the data from the given action to the given key.
 */
QVariant ListPopupMenu::dataFromAction(DataType key, QAction* action)
{
    QVariant data = action->data();
    QMap<QString, QVariant> map = data.toMap();
    return map[ListPopupMenu::toString(key)];
}

/**
 * Convenience method to extract the ListPopupMenu pointer stored in QAction
 * objects belonging to ListPopupMenu.
 */
ListPopupMenu* ListPopupMenu::menuFromAction(QAction *action)
{
    if (action) {
        QVariant value = dataFromAction(dt_MenuPointer, action);
        if (value.canConvert<ListPopupMenu*>()) {
            return qvariant_cast<ListPopupMenu*>(value);
        }
    }
    return 0;
}

/**
 * Shortcut for commonly used sub menu initializations.
 *
 * @param type   The MenuType for which to set up the menu.
 */
void ListPopupMenu::insertSubMenuNew(MenuType type, KMenu *menu)
{
    if (!menu) {
        menu = makeNewMenu();
    }
    switch (type) {
        case mt_Class:
            insert(mt_Attribute, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Public_Attribute), i18n("Attribute"));
            insert(mt_Operation, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Public_Method), i18n("Operation"));
            insert(mt_Template, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Template_Class), i18n("Template"));
            insertContainerItems(menu, false, false);
            break;
        case mt_Component:
            insert(mt_Component, menu);
            if (Settings::optionState().generalState.uml2)
                insert(mt_Port, menu);
            insert(mt_Artifact, menu);
            break;
        case mt_Interface:
            insert(mt_Operation, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Public_Method), i18n("Operation"));
            insert(mt_Template, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Template_Interface), i18n("Template"));
            insertContainerItems(menu, false, false);
            break;
        case mt_Entity:
            insert(mt_EntityAttribute, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Entity_Attribute), i18n("Entity Attribute..."));
            insert(mt_PrimaryKeyConstraint, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Constraint_PrimaryKey), i18n("Primary Key Constraint..."));
            insert(mt_UniqueConstraint, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Constraint_Unique), i18n("Unique Constraint..."));
            insert(mt_ForeignKeyConstraint, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Constraint_ForeignKey), i18n("Foreign Key Constraint..."));
            insert(mt_CheckConstraint, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Constraint_Check), i18n("Check Constraint..."));
            break;
        case mt_Enum:
            insert(mt_EnumLiteral, menu, Icon_Utils::SmallIcon(Icon_Utils::it_Enum_Literal), i18n("Enum Literal..."));
            break;
        case mt_Object:
            break;
        case mt_New_Activity:
        case mt_Activity_Selected:
            break;
        case mt_Subsystem:
            insert(mt_Subsystem, menu);
            insert(mt_Component, menu);
            insert(mt_Artifact, menu);
            break;
        default:
            break;
    }
    addMenu(menu);
}

/**
 * Shortcut for commonly used menu initializations.
 *
 * @param type   The MenuType for which to set up the menu.
 * @TODO This method need to be cleaned up
 */
void ListPopupMenu::setupMenu(MenuType type)
{
    DEBUG(DBG_SRC) << "ListPopupMenu created for MenuType=" << toString(type);
    switch(type) {
    case mt_New_Parameter:
        insert(mt_New_Parameter);
        break;

    case mt_New_Operation:
        insert(mt_New_Operation);
        break;

    case mt_New_Attribute:
        insert(mt_New_Attribute);
        break;

    case mt_New_InstanceAttribute:
        insert(mt_New_InstanceAttribute);
        break;

    case mt_New_Template:
        insert(mt_New_Template);
        break;

    case mt_New_EnumLiteral:
        insert(mt_New_EnumLiteral);
        break;

    case mt_New_EntityAttribute:
        insert(mt_New_EntityAttribute);
        break;

    case mt_New_UniqueConstraint:
        insert(mt_New_UniqueConstraint);
        break;

    case mt_New_PrimaryKeyConstraint:
        insert(mt_New_PrimaryKeyConstraint);
        break;

    case mt_New_ForeignKeyConstraint:
        insert(mt_New_ForeignKeyConstraint);
        break;

    case mt_New_CheckConstraint:
        insert(mt_New_CheckConstraint);
        break;

    case mt_New_Activity:
        insertSubMenuNew(type);
        break;

    case mt_Activity_Selected:
        insertSubMenuNew(type);
        insert(mt_Rename);
        insert(mt_Delete);
        break;

    case mt_Parameter_Selected:
        insert(mt_New_Parameter);
        insert(mt_Rename);
        insert(mt_Delete);
        insert(mt_Properties);
        break;

    case mt_Operation_Selected:
        insert(mt_New_Operation);
        insert(mt_Delete);
        insert(mt_Properties);
        break;

    case mt_Attribute_Selected:
        insert(mt_New_Attribute);
        insert(mt_Delete);
        insert(mt_Properties);
        break;

    case mt_InstanceAttribute_Selected:
        insert(mt_New_InstanceAttribute);
        insert(mt_Delete);
        insert(mt_Properties);
        break;

    case mt_Template_Selected:
        insert(mt_New_Attribute, Icon_Utils::SmallIcon(Icon_Utils::it_Template_New), i18n("New Template..."));
        insert(mt_Delete);
        insert(mt_Properties);
        break;

    case mt_EnumLiteral_Selected:
        insert(mt_New_EnumLiteral);
        insert(mt_Delete);
        insert(mt_Properties);
        break;

    case mt_EntityAttribute_Selected:
        insert(mt_New_EntityAttribute);
        insert(mt_Delete);
        insert(mt_Properties);
        break;

    case mt_UniqueConstraint_Selected:
        insert(mt_New_UniqueConstraint);
        insert(mt_Delete);
        insert(mt_Properties);
        break;

    case mt_PrimaryKeyConstraint_Selected:
        insert(mt_New_PrimaryKeyConstraint);
        insert(mt_Delete);
        insert(mt_Properties);
        break;

    case mt_ForeignKeyConstraint_Selected:
        insert(mt_New_ForeignKeyConstraint);
        insert(mt_Delete);
        insert(mt_Properties);
        break;

    case mt_CheckConstraint_Selected:
        insert(mt_New_ForeignKeyConstraint);
        insert(mt_Delete);
        insert(mt_Properties);
        break;

    case mt_Anchor:
        insert(mt_Delete, Icon_Utils::SmallIcon(Icon_Utils::it_Delete), i18n("Delete Anchor"));
        break;

    case mt_RoleNameA:
        insertAssociationTextItem(i18n("Change Role A Name..."), mt_Rename_RoleAName);
        break;

    case mt_RoleNameB:
        insertAssociationTextItem(i18n("Change Role B Name..."), mt_Rename_RoleBName);
        break;

    case mt_MultiB:
        insertAssociationTextItem(i18n("Change Multiplicity..."), mt_Rename_MultiB);
        break;

    case mt_MultiA:
        insertAssociationTextItem(i18n("Change Multiplicity..."), mt_Rename_MultiA);
        break;

    case mt_Name:
        insertAssociationTextItem(i18n("Change Name"), mt_Rename_Name);
        break;

    case mt_Model:
        insert(mt_Model, i18n("Rename..."));
        break;

    case mt_Properties:
        insert(mt_Expand_All);
        insert(mt_Collapse_All);
        insert(mt_Properties);
        break;

    case mt_Properties_AutoLayout:
    case mt_Properties_Class:
    case mt_Properties_CodeImport:
    case mt_Properties_CodeGeneration:
    case mt_Properties_CodeViewer:
    case mt_Properties_Font:
    case mt_Properties_General:
    case mt_Properties_UserInterface:
        insert(mt_Properties);
        break;

    default:
        uWarning() << "unknown menu type " << type;
        break;
    }//end switch
}

/**
 * Create the 'new' menu
 * @return menu instance
 */
KMenu *ListPopupMenu::makeNewMenu()
{
    KMenu *menu = new KMenu(i18nc("new sub menu", "New"), this);
    menu->setIcon(Icon_Utils::SmallIcon(Icon_Utils::it_New));
    return menu;
}

/**
 * Creates a popup menu for a single category Object
 * @param category The UMLCategory for which the category menu is created
 */
KMenu* ListPopupMenu::makeCategoryTypeMenu(UMLCategory* category)
{
    KMenu* catTypeMenu = new KMenu(this);
    insert(mt_DisjointSpecialisation, catTypeMenu, i18n("Disjoint(Specialisation)"), CHECKABLE);
    insert(mt_OverlappingSpecialisation, catTypeMenu, i18n("Overlapping(Specialisation)"), CHECKABLE);
    insert(mt_Union, catTypeMenu, i18n("Union"), CHECKABLE);
    setActionChecked(mt_DisjointSpecialisation, category->getType()==UMLCategory::ct_Disjoint_Specialisation);
    setActionChecked(mt_OverlappingSpecialisation, category->getType()==UMLCategory::ct_Overlapping_Specialisation);
    setActionChecked(mt_Union, category->getType()==UMLCategory::ct_Union);

    return catTypeMenu;
}

/**
 * Get the action from the menu type as index.
 */
QAction* ListPopupMenu::getAction(MenuType idx)
{
    return m_actions.value(idx, 0);
}

// /**
//  * Get the MenuType from the action.
//  */
// ListPopupMenu::MenuType ListPopupMenu::getMenuType(KAction* action)
// {
//     return m_actions.key(action);
// }

/**
 * Get the MenuType from the action.
 */
ListPopupMenu::MenuType ListPopupMenu::getMenuType(QAction* action)
{
    QList<MenuType> keyList = m_actions.keys(action);
    if (keyList.empty() || /* all key-value pairs are unique*/ keyList.count() > 1) {
        return mt_Undefined;
    } else {
        // we return the first (only) value
        return keyList.first();
    }
}

/**
 * Utility method to fetch owner of the menu.
 *
 * @return The owned WidgetBase if this menu is owned by a
 *         widget. Otherwise returns 0.
 */
WidgetBase* ListPopupMenu::ownerWidget() const
{
    if (m_TriggerObjectType == tot_Widget) {
        return m_TriggerObject.m_Widget;
    }
    return 0;
}

/**
 * Checks the action item.
 *
 * @param idx     The MenuType for which to check the menu item.
 * @param value   The value.
 */
void ListPopupMenu::setActionChecked(MenuType idx, bool value)
{
    QAction* action = getAction(idx);
    if (action && action->isCheckable()) {
        action->setChecked(value);
    }
    else {
        DEBUG(DBG_SRC) << "called on unknown MenuType " << toString(idx);
    }
}

/**
 * Enables the action item.
 *
 * @param idx     The MenuType for which to enable the menu item.
 * @param value   The value.
 */
void ListPopupMenu::setActionEnabled(MenuType idx, bool value)
{
    QAction* action = getAction(idx);
    if (action) {
        action->setEnabled(value);
    }
    else {
        DEBUG(DBG_SRC) << "called on unknown MenuType " << toString(idx);
    }
}

/**
 * Sets up actions added to the ListPopupMenu to have their data field set to
 * pointer to this ListPopupMenu object, so that this menu pointer can be
 * retrieved in UMLWidget::slotMenuSelection
 *
 * @note This might seem like an ugly hack, but this is the only solution which
 *       helped in avoiding storage of ListPopupMenu pointer in each UMLWidget.
 */
void ListPopupMenu::setupActionsData()
{
    foreach (QAction *action, m_actions) {
        QMap<QString, QVariant> map = action->data().toMap();
        map[toString(dt_MenuPointer)] = qVariantFromValue(this);
        action->setData(QVariant(map));
    }

}

/**
 * Convert enum MenuType to string.
 */
QString ListPopupMenu::toString(MenuType menu)
{
    return QLatin1String(ENUM_NAME(ListPopupMenu, MenuType, menu));
}

/**
 * Convert enum DataType to string.
 */
QString ListPopupMenu::toString(DataType data)
{
    return QLatin1String(ENUM_NAME(ListPopupMenu, DataType, data));
}

/**
 * dump collected actions
 * @param type optional menu type
 */
void ListPopupMenu::dumpActions(const QString &title)
{
    qDebug().nospace() << title;
    foreach(MenuType keyType, m_debugActions) {
        qDebug().nospace() << "    " << toString(keyType);
    }
}
