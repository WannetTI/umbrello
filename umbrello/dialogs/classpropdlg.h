/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef CLASSPROPDLG_H
#define CLASSPROPDLG_H

#include "dialogbase.h"

class AssocPage;
class ClassGenPage;
class ClassifierListPage;
class ClassOptionsPage;
class ConstraintListPage;
class KFontChooser;
class PkgContentsPage;
class ObjectWidget;
class UMLDoc;
class UMLObject;
class UMLWidget;
class UMLWidgetStylePage;

/**
 * @author Paul Hensgen <phensgen@techie.com>
 * @version 1.0
 * Bugs and comments to umbrello-devel@kde.org or http://bugs.kde.org
 */
class ClassPropDlg : public DialogBase
{
    Q_OBJECT
public:
    ClassPropDlg(QWidget *parent, UMLObject *c, bool assoc = false);
    ClassPropDlg(QWidget *parent, ObjectWidget * o);
    ClassPropDlg(QWidget *parent, UMLWidget * o);

    ~ClassPropDlg();

    enum Page{page_gen = 0, page_att, page_op, page_entatt, page_constraint, page_template,
              page_assoc, page_options, page_color, page_font};

protected slots:
    void slotOk();
    void slotApply();

protected:
    void setupPages(bool assoc = false);
    void setupGeneralPage();
    void setupStylePage();
    void setupDisplayPage();
    void setupAttributesPage();
    void setupOperationsPage();
    void setupTemplatesPage();
    void setupEnumLiteralsPage();
    void setupEntityAttributesPage();
    void setupEntityConstraintsPage();
    void setupContentsPage();
    void setupAssociationsPage();
    void setupInstancePages();
    void setupFontPage();

private:
    KFontChooser*        m_pChooser;
    ClassGenPage*        m_pGenPage;
    ClassifierListPage*  m_pAttPage;
    ClassifierListPage*  m_pOpsPage;
    ClassifierListPage*  m_pTemplatePage;
    ClassifierListPage*  m_pEnumLiteralPage;
    ClassifierListPage*  m_pEntityAttributePage;
    ConstraintListPage*  m_pEntityConstraintPage;
    PkgContentsPage*     m_pPkgContentsPage;
    AssocPage*           m_pAssocPage;
    ClassOptionsPage*    m_pOptionsPage;
    UMLWidgetStylePage*  m_pStylePage;

    UMLDoc*              m_doc;
    UMLObject*           m_pObject;
    UMLWidget*           m_pWidget;

    enum Page_Type {
        pt_Object = 1,    // Show General page + Assoc. page if Class i.e. no colors page
        pt_ObjectWidget,  // Shows pages needed for an ObjectWidget
        pt_Widget         // Shows pages needed for any other widget
    };

    Page_Type m_Type;

private:
    void init();

};

#endif
