
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
 *      Date   : Thu Aug 28 2003
 */

/**
  We carve the CPP document up into 2 documents, "source" and "header". 
  This one represents the header portion.
  The sections of each are as follows:

  * header
  * includes 
  * import statements
  * class declaration
  *   guts of the class (e.g. field decl, accessor methods, operations, dependant classes)
*/

#include <kdebug.h>
#include <qregexp.h>

#include "cppheadercodedocument.h"
#include "cppcodegenerator.h"
#include "cppcodedocumentation.h"
#include "cppheadercodeaccessormethod.h"
#include "cppheadercodeoperation.h"
#include "cppheaderclassdeclarationblock.h"
#include "cppheadercodeclassfielddeclarationblock.h"
#include "../umlpackagelist.h"
#include "../package.h"

// Constructors/Destructors
//  

CPPHeaderCodeDocument::CPPHeaderCodeDocument ( UMLClassifier * concept, CPPCodeGenerator *parent) 
   : ClassifierCodeDocument (concept, (CodeGenerator *) parent ) { 
	init ( );
}

CPPHeaderCodeDocument::~CPPHeaderCodeDocument ( ) { }

//  
// Methods
//  

// Accessor methods
//

QString CPPHeaderCodeDocument::getCPPClassName (QString name) {
	CPPCodeGenerator *g = (CPPCodeGenerator*) getParentGenerator();
	return g->getCPPClassName(name);
}

CPPHeaderClassDeclarationBlock * CPPHeaderCodeDocument::getClassDecl() 
{

	if(!classDeclCodeBlock) {
		classDeclCodeBlock = new CPPHeaderClassDeclarationBlock (this); // was deleted before our load
		classDeclCodeBlock->setTag("classDeclarationBlock");
	}
	return classDeclCodeBlock;
}

// Other methods
//

// a little utility method 
bool CPPHeaderCodeDocument::forceDoc () {
	return getParentGenerator()->forceDoc();
}

// Sigh. NOT optimal. The only reason that we need to have this
// is so we can create the CPPHeaderClassDeclarationBlock.
// would be better if we could create a handler interface that each
// codeblock used so all we have to do here is add the handler
void CPPHeaderCodeDocument::loadChildTextBlocksFromNode ( QDomElement & root)
{

	QDomNode tnode = root.firstChild();
	QDomElement telement = tnode.toElement();
	bool loadCheckForChildrenOK = false;
	while( !telement.isNull() ) {
		QString nodeName = telement.tagName();

		if( nodeName == "textblocks" ) {

			QDomNode node = telement.firstChild();
			QDomElement element = node.toElement();

			// if there is nothing to begin with, then we dont worry about it
			loadCheckForChildrenOK = element.isNull() ? true : false;

			while( !element.isNull() ) {
				QString name = element.tagName();

			       if( name == "codecomment" ) {
					CodeComment * block = newCodeComment();
					block->loadFromXMI(element);
					if(!addTextBlock(block))
					{
						kdError()<<"Unable to add codeComment to :"<<this<<endl;
						block->deleteLater();
					} else
						loadCheckForChildrenOK= true;
				} else
				if( name == "codeaccessormethod" ||
				    name == "ccfdeclarationcodeblock"
				  ) {
					QString acctag = element.attribute("tag","");
					// search for our method in the
					TextBlock * tb = findCodeClassFieldTextBlockByTag(acctag);
					if(!tb || !addTextBlock(tb))
					{
						kdError()<<"Unable to add codeclassfield child method to:"<<this<<endl;
						// DONT delete
					} else
						loadCheckForChildrenOK= true;

				} else
				if( name == "codeblock" ) {
					CodeBlock * block = newCodeBlock();
					block->loadFromXMI(element);
					if(!addTextBlock(block))
					{
						kdError()<<"Unable to add codeBlock to :"<<this<<endl;
						block->deleteLater();
					} else
						loadCheckForChildrenOK= true;
				} else
				if( name == "codeblockwithcomments" ) {
					CodeBlockWithComments * block = newCodeBlockWithComments();
					block->loadFromXMI(element);
					if(!addTextBlock(block))
					{
						kdError()<<"Unable to add codeBlockwithcomments to:"<<this<<endl;
						block->deleteLater();
					} else
						loadCheckForChildrenOK= true;
				} else
				if( name == "header" ) {
				       // do nothing.. this is treated elsewhere
				} else
				if( name == "hierarchicalcodeblock" ) {
					HierarchicalCodeBlock * block = newHierarchicalCodeBlock();
					block->loadFromXMI(element);
					if(!addTextBlock(block))
					{
						kdError()<<"Unable to add hierarchicalcodeBlock to:"<<this<<endl;
						block->deleteLater();
					} else
						loadCheckForChildrenOK= true;
				} else
				if( name == "codeoperation" ) {
				       // find the code operation by id
					QString id = element.attribute("parent_id","-1");
					UMLObject * obj = getParentGenerator()->getDocument()->findObjectById(STR2ID(id));
					UMLOperation * op = dynamic_cast<UMLOperation*>(obj);
					if(op) {
						CodeOperation * block = newCodeOperation(op);
						block->loadFromXMI(element);
						if(addTextBlock(block))
							loadCheckForChildrenOK= true;
						else
						{
							kdError()<<"Unable to add codeoperation to:"<<this<<endl;
							block->deleteLater();
						}
					} else
					      kdError()<<"Unable to find operation create codeoperation for:"<<this<<endl;
				}
				else
				if( name == "cppheaderclassdeclarationblock" )
				{
						CPPHeaderClassDeclarationBlock * block = getClassDecl();
						block->loadFromXMI(element);
						// normally this would be populated by the following syncToparent
						// call, but we cant wait for it, so lets just do it now.
						namespaceBlock = getHierarchicalCodeBlock("namespace", "Namespace", 0);

						if(!namespaceBlock || !namespaceBlock->addTextBlock(block))
						{
							kdError()<<"Error:cant add class declaration codeblock"<<endl;
							// DONT delete/release block
							// block->release();
						} else
							loadCheckForChildrenOK= true;

				}
				// only needed for extreme debuging conditions (E.g. making new codeclassdocument loader)
				//else
					//kdDebug()<<" LoadFromXMI: Got strange tag in text block stack:"<<name<<", ignorning"<<endl;

				node = element.nextSibling();
				element = node.toElement();
			}
			break;
		}

		tnode = telement.nextSibling();
		telement = tnode.toElement();
	}

       if(!loadCheckForChildrenOK)
	{
		CodeDocument * test = dynamic_cast<CodeDocument*>(this);
		if(test)
		{
			kdWarning()<<" loadChildBlocks : unable to initialize any child blocks in doc: "<<test->getFileName()<<" "<<this<<endl;
		} else {
			HierarchicalCodeBlock * hb = dynamic_cast<HierarchicalCodeBlock*>(this);
			if(hb)
				kdWarning()<<" loadChildBlocks : unable to initialize any child blocks in Hblock: "<<hb->getTag()<<" "<<this<<endl;
			else
				kdDebug()<<" loadChildBlocks : unable to initialize any child blocks in UNKNOWN OBJ:"<<this<<endl;
		}
	}

}

void CPPHeaderCodeDocument::resetTextBlocks()
{

	// all special pointers need to be zero'd out.
	classDeclCodeBlock = 0;
	publicBlock = 0;
	protectedBlock = 0;
	privateBlock = 0;
	namespaceBlock = 0;
	pubConstructorBlock = 0;
	protConstructorBlock = 0;
	privConstructorBlock = 0;
	pubOperationsBlock = 0;
       	privOperationsBlock = 0;
	protOperationsBlock = 0;

	// now do the traditional release of child text blocks
	ClassifierCodeDocument::resetTextBlocks();

}

// Initialize this cpp classifier code document
void CPPHeaderCodeDocument::init ( ) {

	setFileExtension(".h");

	initCodeClassFields(); // we have to call here as .newCodeClassField is pure virtual in parent class 

	// needed? I doubt it, but it feels good to do it.
	classDeclCodeBlock = 0;
	publicBlock = 0;
	protectedBlock = 0;
	privateBlock = 0;
	namespaceBlock = 0;
	pubConstructorBlock = 0;
	protConstructorBlock = 0;
	privConstructorBlock = 0;
	pubOperationsBlock = 0;
	privOperationsBlock = 0;
	protOperationsBlock = 0;

	// this will call updateContent() as well as other things that sync our document.
	synchronize();
}

// IF the classifier object is modified, this will get called.
// Possible mods include changing the filename and package
// the classifier has.
void CPPHeaderCodeDocument::syncNamesToParent( )
{

	setFileName(getParentGenerator()->cleanName(getParentClassifier()->getName().lower()));
	setPackage(getParentGenerator()->cleanName(getParentClassifier()->getPackage().lower()));
}

/**
 * @param       op
 */
// in the vannilla version, we just tack all operations on the end
// of the document
bool CPPHeaderCodeDocument::addCodeOperation (CodeOperation * op ) {

	Uml::Scope scope = op->getParentOperation()->getScope();
	if(!op->getParentOperation()->isConstructorOperation())
	{
		switch (scope) {
			default:
			case Uml::Public:
				return pubOperationsBlock->addTextBlock(op);
				break;
			case Uml::Protected:
				return protOperationsBlock->addTextBlock(op);
				break;
			case Uml::Private:
				return privOperationsBlock->addTextBlock(op);
				break;
		}
	} else {
		switch (scope) {
			default:
			case Uml::Public:
				return pubConstructorBlock->addTextBlock(op);
				break;
			case Uml::Protected:
				return protConstructorBlock->addTextBlock(op);
				break;
			case Uml::Private:
				return privConstructorBlock->addTextBlock(op);
				break;
		}
	}
}

/**
 * create a new CodeAccesorMethod object belonging to this CodeDocument.
 * @return      CodeAccessorMethod
 */
CodeAccessorMethod * CPPHeaderCodeDocument::newCodeAccessorMethod( CodeClassField *cf, CodeAccessorMethod::AccessorType type ) {
	return new CPPHeaderCodeAccessorMethod((CPPCodeClassField*)cf, type);
}


CodeClassField * CPPHeaderCodeDocument::newCodeClassField ( UMLAttribute * at) {
	return new CPPCodeClassField(this,at);
}

CodeClassField * CPPHeaderCodeDocument::newCodeClassField ( UMLRole * role) {
	return new CPPCodeClassField(this,role);
}

/**
 * create a new CodeBlockWithComments object belonging to this CodeDocument.
 * @return      CodeBlockWithComments
 */
CodeComment * CPPHeaderCodeDocument::newCodeComment ( ) {
	return new CPPCodeDocumentation(this);
}

/**
 * create a new CodeOperation object belonging to this CodeDocument.
 * @return      CodeOperation
 */
CodeOperation * CPPHeaderCodeDocument::newCodeOperation( UMLOperation * op) {
	return new CPPHeaderCodeOperation(this, op);
}

CodeClassFieldDeclarationBlock * CPPHeaderCodeDocument::newDeclarationCodeBlock (CodeClassField * cf ) {
	return new CPPHeaderCodeClassFieldDeclarationBlock((CPPCodeClassField*)cf);
}

/**
 * Save the XMI representation of this object
 * @return      bool    status of save
 */
/*
void CPPHeaderCodeDocument::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
	QDomElement docElement = doc.createElement( "" );

	setAttributesOnNode(doc, docElement);

	root.appendChild( docElement );
}
*/

// This method will cause the class to rebuild its text representation.
// based on the parent classifier object.
// For any situation in which this is called, we are either building the code
// document up, or replacing/regenerating the existing auto-generated parts. As
// such, we will want to insert everything we resonablely will want
// during creation. We can set various parts of the document (esp. the
// comments) to appear or not, as needed.
void CPPHeaderCodeDocument::updateContent( ) 
{

      	// Gather info on the various fields and parent objects of this class...
	UMLClassifier * c = getParentClassifier();
	CodeGenerator * g = getParentGenerator();
	// CPPCodeGenerator * gen = dynamic_cast<CPPCodeGenerator*>(g);
	CPPCodeGenerator * gen = (CPPCodeGenerator*)g;
	CPPCodeGenerationPolicy * policy = (CPPCodeGenerationPolicy*)getPolicy();

	// first, set the global flag on whether or not to show classfield info 
	QPtrList<CodeClassField> * cfList = getCodeClassFieldList();
	for(CodeClassField * field = cfList->first(); field; field = cfList->next())
		field->setWriteOutMethods(gen->getAutoGenerateAccessors());

      	// attribute-based ClassFields
      	// we do it this way to have the static fields sorted out from regular ones
	QPtrList<CodeClassField> staticPublicAttribClassFields = getSpecificClassFields (CodeClassField::Attribute, true, Uml::Public );
	QPtrList<CodeClassField> publicAttribClassFields = getSpecificClassFields (CodeClassField::Attribute, false, Uml::Public );
	QPtrList<CodeClassField> staticProtectedAttribClassFields = getSpecificClassFields (CodeClassField::Attribute, true, Uml::Protected );
	QPtrList<CodeClassField> protectedAttribClassFields = getSpecificClassFields (CodeClassField::Attribute, false, Uml::Protected );
	QPtrList<CodeClassField> staticPrivateAttribClassFields = getSpecificClassFields (CodeClassField::Attribute, true, Uml::Private );
	QPtrList<CodeClassField> privateAttribClassFields = getSpecificClassFields (CodeClassField::Attribute, false, Uml::Private);

      	// association-based ClassFields 
      	// dont care if they are static or not..all are lumped together
	QPtrList<CodeClassField> publicPlainAssocClassFields = getSpecificClassFields ( CodeClassField::PlainAssociation , Uml::Public);
	QPtrList<CodeClassField> publicAggregationClassFields = getSpecificClassFields ( CodeClassField::Aggregation, Uml::Public);
	QPtrList<CodeClassField> publicCompositionClassFields = getSpecificClassFields ( CodeClassField::Composition, Uml::Public );

	QPtrList<CodeClassField> protPlainAssocClassFields = getSpecificClassFields ( CodeClassField::PlainAssociation , Uml::Protected);
	QPtrList<CodeClassField> protAggregationClassFields = getSpecificClassFields ( CodeClassField::Aggregation, Uml::Protected);
	QPtrList<CodeClassField> protCompositionClassFields = getSpecificClassFields ( CodeClassField::Composition, Uml::Protected);

	QPtrList<CodeClassField> privPlainAssocClassFields = getSpecificClassFields ( CodeClassField::PlainAssociation , Uml::Private);
	QPtrList<CodeClassField> privAggregationClassFields = getSpecificClassFields ( CodeClassField::Aggregation, Uml::Private);
	QPtrList<CodeClassField> privCompositionClassFields = getSpecificClassFields ( CodeClassField::Composition, Uml::Private);

	bool hasOperationMethods = c->getOpList().last() ? true : false;
	bool hasNamespace = false;
	bool isEnumeration = false;
	bool isInterface = parentIsInterface();
	bool hasclassFields = hasClassFields();
	bool forcedoc = forceDoc();
	QString endLine = gen->getNewLineEndingChars(); // a shortcut..so we dont have to call this all the time

	UMLClassifierList superclasses = c->findSuperClassConcepts();


	// START GENERATING CODE/TEXT BLOCKS and COMMENTS FOR THE DOCUMENT
	//

	// Write the hash define stuff to prevent multiple parsing/inclusion of header
	QString cppClassName = gen->getCPPClassName(c->getName());
	QString hashDefine = gen->getCPPClassName(c->getName()).upper().simplifyWhiteSpace().replace(QRegExp(" "),  "_");
	QString defText = "#ifndef "+hashDefine + "_H"+ endLine + "#define "+ hashDefine + "_H";
	addOrUpdateTaggedCodeBlockWithComments("hashDefBlock", defText, "", 0, false);

       	// INCLUDE CODEBLOCK 
       	//
       	// Q: Why all utils? Isnt just List and Vector the only classes we are using?
       	// A: doesn't matter at all; its more readable to just include '*' and cpp compilers
       	//    don't slow down or anything. (TZ)
       	QString includeStatement = "";
	bool stringGlobal = policy->stringIncludeIsGlobal();
	QString sStartBrak = stringGlobal ? "<" : "\"";
	QString sEndBrak = stringGlobal ? ">" : "\"";
       	includeStatement.append("#include "+sStartBrak+policy->getStringClassNameInclude()+sEndBrak+endLine);
       	if ( hasObjectVectorClassFields() )
	{
		bool vecGlobal = policy->vectorIncludeIsGlobal();
		QString vStartBrak = vecGlobal ? "<" : "\"";
		QString vEndBrak = vecGlobal ? ">" : "\"";
		QString value ="#include "+vStartBrak+policy->getVectorClassNameInclude()+vEndBrak;
       		includeStatement.append(value+endLine);
	}

   	//only include classes in a different package from this class
       	UMLClassifierList includes;
       	QMap<UMLClassifier *,QString> *packageMap = new QMap<UMLClassifier*,QString>; // so we dont repeat packages

	gen->findObjectsRelated(c,includes);
       	for(UMLClassifier *con = includes.first(); con ; con = includes.next())
       	if (!(packageMap->contains(con)))
	{
       		packageMap->insert(con,con->getPackage());
		if(con != getParentClassifier())
	       		includeStatement.append("#include \""+gen->cleanName(con->getName().lower())+".h\""+endLine);
	}
	// now, add/update the includes codeblock
	CodeBlockWithComments * inclBlock = addOrUpdateTaggedCodeBlockWithComments("includes", includeStatement, QString::null, 0, false);
	if(includeStatement.isEmpty() && inclBlock->getContentType() == CodeBlock::AutoGenerated)
		inclBlock->setWriteOutText(false);
	else
		inclBlock->setWriteOutText(true);

	// Using
	QString usingStatement;
	for(UMLClassifier *classifier = superclasses.first(); classifier ; classifier = superclasses.next()) {
		if(classifier->getPackage()!=c->getPackage() && !classifier->getPackage().isEmpty()) {
			usingStatement.append("using "+gen->cleanName(c->getPackage())+"::"+cleanName(c->getName())+";"+endLine);
		}
	}
	CodeBlockWithComments * usingBlock = addOrUpdateTaggedCodeBlockWithComments("using", usingStatement, "", 0, false);
	if(usingStatement.isEmpty() && usingBlock->getContentType() == CodeBlock::AutoGenerated)
		usingBlock->setWriteOutText(false);
	else
		usingBlock->setWriteOutText(true);

	// namespace
	// This needs special treatment. We cant use "nowriteouttext" for this, as
	// that will prevent the class declaration from being written. Instead, we
	// check if "hasNamspace" is true or not, and then indent the remaining code
	// appropriately as well as set the start/end text of this namespace block.
	if(!c->getPackage().isEmpty() && policy->getPackageIsNamespace())
		hasNamespace = true;
	else
		hasNamespace = false;

	// set start/end text of namespace block
	namespaceBlock = getHierarchicalCodeBlock("namespace", "Namespace", 0);
	if(hasNamespace) {
		UMLPackageList pkgList = c->getPackages();
		QString pkgs;
		UMLPackage *pkg;
		for (pkg = pkgList.first(); pkg != NULL; pkg = pkgList.next()) {
			pkgs += "namespace " + gen->cleanName(pkg->getName()) + " { ";
		}
		namespaceBlock->setStartText(pkgs);
		QString closingBraces;
		for (pkg = pkgList.first(); pkg != NULL; pkg = pkgList.next()) {
			closingBraces += "} ";
		}
		namespaceBlock->setEndText(closingBraces);
		namespaceBlock->getComment()->setWriteOutText(true);
	} else {
		namespaceBlock->setStartText("");
		namespaceBlock->setEndText("");
		namespaceBlock->getComment()->setWriteOutText(false);
	}

	// Enum types for include
	if (!isInterface) {

		QString enumStatement;
		QString indent = policy->getIndentation();
/*
		UMLClass* k = dynamic_cast<UMLClass*>(c);
		if (k->isEnumeration()) {
			enumStatement.append(indent + "enum "+cppClassName+" {"+endLine);

			// populate 
			UMLAttributeList atl = k->getFilteredAttributeList();
			for (UMLAttribute *at=atl.first(); at ; ) {
				enumStatement.append(indent+indent);
				enumStatement.append(gen->cleanName(at->getName()));
				if ((at=atl.next()) != 0)
					enumStatement.append(", "+endLine);
				else
					break;
				enumStatement.append(endLine);
			}
			enumStatement.append(indent+"};");
			isEnumeration = true;
		}
*/
		namespaceBlock->addOrUpdateTaggedCodeBlockWithComments("enums", enumStatement, "", 0, false);
	 }

	// CLASS DECLARATION BLOCK
	//

	// add the class declaration block to the namespace block. 
	CPPHeaderClassDeclarationBlock * myClassDeclCodeBlock = getClassDecl();
	namespaceBlock->addTextBlock(myClassDeclCodeBlock); // note: wont add if already present

	// Is this really true?? hmm..
	if(isEnumeration)
		myClassDeclCodeBlock->setWriteOutText(false); // not written out IF its an enumeration class
	else	
		myClassDeclCodeBlock->setWriteOutText(true);

	//
	// Main Sub-Blocks
	//

	// declare public, protected and private methods, attributes (fields).
	// set the start text ONLY if this is the first time we created the objects.
	bool createdPublicBlock = publicBlock == 0 ? true : false;
	publicBlock = myClassDeclCodeBlock->getHierarchicalCodeBlock("publicBlock","Public stuff",0);
	if (createdPublicBlock)
		publicBlock->setStartText("public:");

	bool createdProtBlock = protectedBlock == 0 ? true : false;
	protectedBlock = myClassDeclCodeBlock->getHierarchicalCodeBlock("protectedBlock","Protected stuff",0);
	if(createdProtBlock)
		protectedBlock->setStartText("protected:");

	bool createdPrivBlock = privateBlock == 0 ? true : false;
	privateBlock = myClassDeclCodeBlock->getHierarchicalCodeBlock("privateBlock","Private stuff",0);
	if(createdPrivBlock)
		privateBlock->setStartText("private:");

	//
	// * CLASS FIELD declaration section
	//

	// setup/get/create the field declaration code block
	// 

	// public fields: Update the comment: we only set comment to appear under the following conditions
	HierarchicalCodeBlock * publicFieldDeclBlock = publicBlock->getHierarchicalCodeBlock("publicFieldsDecl", "Fields", 1);
	CodeComment * pubFcomment = publicFieldDeclBlock->getComment();
	if (!forcedoc && !hasclassFields )
		pubFcomment->setWriteOutText(false);
	else
		pubFcomment->setWriteOutText(true);

	// protected fields: Update the comment: we only set comment to appear under the following conditions
	HierarchicalCodeBlock * protectedFieldDeclBlock = protectedBlock->getHierarchicalCodeBlock("protectedFieldsDecl", "Fields", 1);
	CodeComment * protFcomment = protectedFieldDeclBlock->getComment();
	if (!forcedoc && !hasclassFields )
		protFcomment->setWriteOutText(false);
	else
		protFcomment->setWriteOutText(true);

	// private fields: Update the comment: we only set comment to appear under the following conditions
	HierarchicalCodeBlock * privateFieldDeclBlock = privateBlock->getHierarchicalCodeBlock("privateFieldsDecl", "Fields", 1);
	CodeComment * privFcomment = privateFieldDeclBlock->getComment();
	if (!forcedoc && !hasclassFields )
		privFcomment->setWriteOutText(false);
	else
		privFcomment->setWriteOutText(true);


	// now actually declare the fields within the appropriate HCodeBlock
	//

	// public 
	declareClassFields(staticPublicAttribClassFields, publicFieldDeclBlock);
	declareClassFields(publicAttribClassFields, publicFieldDeclBlock);
	declareClassFields(publicPlainAssocClassFields, publicFieldDeclBlock);
	declareClassFields(publicAggregationClassFields, publicFieldDeclBlock);
	declareClassFields(publicCompositionClassFields, publicFieldDeclBlock);

	// protected
	declareClassFields(staticProtectedAttribClassFields, protectedFieldDeclBlock);
	declareClassFields(protectedAttribClassFields, protectedFieldDeclBlock);
	declareClassFields(protPlainAssocClassFields, protectedFieldDeclBlock);
	declareClassFields(protAggregationClassFields, protectedFieldDeclBlock);
	declareClassFields(protCompositionClassFields, protectedFieldDeclBlock);

	// private
	declareClassFields(staticPrivateAttribClassFields, privateFieldDeclBlock);
	declareClassFields(privateAttribClassFields, privateFieldDeclBlock);
	declareClassFields(privPlainAssocClassFields, privateFieldDeclBlock);
	declareClassFields(privAggregationClassFields, privateFieldDeclBlock);
	declareClassFields(privCompositionClassFields, privateFieldDeclBlock);

	//
	// METHODS section
	//

	// get/create the method codeblock

	// public methods
	HierarchicalCodeBlock * pubMethodsBlock = publicBlock->getHierarchicalCodeBlock("pubMethodsBlock", "", 1);
	CodeComment * pubMethodsComment = pubMethodsBlock->getComment();
	// set conditions for showing this comment
	if (!forcedoc && !hasclassFields && !hasOperationMethods)
		pubMethodsComment->setWriteOutText(false);
	else
		pubMethodsComment->setWriteOutText(true);

       // protected methods
	HierarchicalCodeBlock * protMethodsBlock = protectedBlock->getHierarchicalCodeBlock("protMethodsBlock", "", 1);
	CodeComment * protMethodsComment = protMethodsBlock->getComment();
	// set conditions for showing this comment
	if (!forcedoc && !hasclassFields && !hasOperationMethods)
		protMethodsComment->setWriteOutText(false);
	else
		protMethodsComment->setWriteOutText(true);

       // private methods
	HierarchicalCodeBlock * privMethodsBlock = privateBlock->getHierarchicalCodeBlock("privMethodsBlock", "", 1);
	CodeComment * privMethodsComment = privMethodsBlock->getComment();
	// set conditions for showing this comment
	if (!forcedoc && !hasclassFields && !hasOperationMethods)
		privMethodsComment->setWriteOutText(false);
	else
		privMethodsComment->setWriteOutText(true);


	// METHODS sub-section : constructor methods
	//

	// setup/get/create the constructor codeblocks

	// public
	pubConstructorBlock = pubMethodsBlock->getHierarchicalCodeBlock("constructionMethods", "Constructors", 1);
	// special condiions for showing comment: only when autogenerateding empty constructors
	// Although, we *should* check for other constructor methods too
	CodeComment * pubConstComment = pubConstructorBlock->getComment();
	if (!forcedoc && (isInterface || !gen->getAutoGenerateConstructors()))
		pubConstComment->setWriteOutText(false);
	else
		pubConstComment->setWriteOutText(true);

	// protected
	protConstructorBlock = protMethodsBlock->getHierarchicalCodeBlock("constructionMethods", "Constructors", 1);
       // special condiions for showing comment: only when autogenerateding empty constructors
	// Although, we *should* check for other constructor methods too
	CodeComment * protConstComment = protConstructorBlock->getComment();
	if (!forcedoc && (isInterface || !gen->getAutoGenerateConstructors()))
		protConstComment->setWriteOutText(false);
	else
		protConstComment->setWriteOutText(true);

	// private
	privConstructorBlock = privMethodsBlock->getHierarchicalCodeBlock("constructionMethods", "Constructors", 1);
       // special condiions for showing comment: only when autogenerateding empty constructors
	// Although, we *should* check for other constructor methods too
	CodeComment * privConstComment = privConstructorBlock->getComment();
	if (!forcedoc && (isInterface || !gen->getAutoGenerateConstructors()))
		privConstComment->setWriteOutText(false);
	else
		privConstComment->setWriteOutText(true);

	// add/get the empty constructor. I guess since there is no 
	// meta-data to state what the scope of this method is, we will make it
	// "public" as a default. This might present problems if the user wants
	// to move the block into the "private" or "protected" blocks.
	QString CPPClassName = getCPPClassName(c->getName()); 
	QString emptyConstStatement = CPPClassName+" ( ) { }";

	// search for this first in the entire document. IF not present, put
	// it in the public constructor method block
	TextBlock * emptyConstTb = findTextBlockByTag("emptyconstructor", true);
	CodeBlockWithComments * emptyConstBlock = dynamic_cast<CodeBlockWithComments*>(emptyConstTb);
	if(!emptyConstBlock)
		emptyConstBlock = pubConstructorBlock->addOrUpdateTaggedCodeBlockWithComments("emptyconstructor", emptyConstStatement, "Empty Constructor", 1, false);

	// Now, as an additional condition we only show the empty constructor block 
	// IF it was desired to be shown
	if(!isInterface && gen->getAutoGenerateConstructors())
		emptyConstBlock->setWriteOutText(true);
	else
		emptyConstBlock->setWriteOutText(false);


	// METHODS subsection : ACCESSOR METHODS 
	//

	// get/create the accessor codeblock

	// public
	HierarchicalCodeBlock * pubAccessorBlock = pubMethodsBlock->getHierarchicalCodeBlock("accessorMethods", "Accessor Methods", 1);
	// set conditions for showing section comment
	CodeComment * pubAccessComment = pubAccessorBlock->getComment();
	if (!forcedoc && !hasclassFields)
		pubAccessComment->setWriteOutText(false);
	else
		pubAccessComment->setWriteOutText(true);

	// protected
	HierarchicalCodeBlock * protAccessorBlock = protMethodsBlock->getHierarchicalCodeBlock("accessorMethods", "Accessor Methods", 1);
	// set conditions for showing section comment
	CodeComment * protAccessComment = protAccessorBlock->getComment();
	if (!forcedoc && !hasclassFields)
		protAccessComment->setWriteOutText(false);
	else
		protAccessComment->setWriteOutText(true);

	// private
	HierarchicalCodeBlock * privAccessorBlock = privMethodsBlock->getHierarchicalCodeBlock("accessorMethods", "Accessor Methods", 1);
	// set conditions for showing section comment
	CodeComment * privAccessComment = privAccessorBlock->getComment();
	if (!forcedoc && !hasclassFields)
		privAccessComment->setWriteOutText(false);
	else
		privAccessComment->setWriteOutText(true);

	// now, 2 sub-sub sections in accessor block
	// add/update accessor methods for attributes
	HierarchicalCodeBlock * pubStaticAccessors = pubAccessorBlock->getHierarchicalCodeBlock("pubStaticAccessorMethods", "", 1);
	HierarchicalCodeBlock * pubRegularAccessors = pubAccessorBlock->getHierarchicalCodeBlock("pubRegularAccessorMethods", "", 1);
	pubStaticAccessors->getComment()->setWriteOutText(false); // never write block comment
	pubRegularAccessors->getComment()->setWriteOutText(false); // never write block comment

	HierarchicalCodeBlock * protStaticAccessors = protAccessorBlock->getHierarchicalCodeBlock("protStaticAccessorMethods", "", 1);
	HierarchicalCodeBlock * protRegularAccessors = protAccessorBlock->getHierarchicalCodeBlock("protRegularAccessorMethods", "", 1);
	protStaticAccessors->getComment()->setWriteOutText(false); // never write block comment
	protRegularAccessors->getComment()->setWriteOutText(false); // never write block comment

	HierarchicalCodeBlock * privStaticAccessors = privAccessorBlock->getHierarchicalCodeBlock("privStaticAccessorMethods", "", 1);
	HierarchicalCodeBlock * privRegularAccessors = privAccessorBlock->getHierarchicalCodeBlock("privRegularAccessorMethods", "", 1);
	privStaticAccessors->getComment()->setWriteOutText(false); // never write block comment
	privRegularAccessors->getComment()->setWriteOutText(false); // never write block comment

	// now add in accessors as appropriate

	// public stuff
	pubStaticAccessors->addCodeClassFieldMethods(staticPublicAttribClassFields); 
	pubRegularAccessors->addCodeClassFieldMethods(publicAttribClassFields);
	pubRegularAccessors->addCodeClassFieldMethods(publicPlainAssocClassFields);
	pubRegularAccessors->addCodeClassFieldMethods(publicAggregationClassFields);
	pubRegularAccessors->addCodeClassFieldMethods(publicCompositionClassFields);

	// protected stuff
	protStaticAccessors->addCodeClassFieldMethods(staticProtectedAttribClassFields);
	protRegularAccessors->addCodeClassFieldMethods(protectedAttribClassFields);
	protRegularAccessors->addCodeClassFieldMethods(protPlainAssocClassFields);
	protRegularAccessors->addCodeClassFieldMethods(protAggregationClassFields);
	protRegularAccessors->addCodeClassFieldMethods(protCompositionClassFields);

	// private stuff
	privStaticAccessors->addCodeClassFieldMethods(staticPrivateAttribClassFields);
	privRegularAccessors->addCodeClassFieldMethods(privateAttribClassFields);
	privRegularAccessors->addCodeClassFieldMethods(privPlainAssocClassFields);
	privRegularAccessors->addCodeClassFieldMethods(privAggregationClassFields);
	privRegularAccessors->addCodeClassFieldMethods(privCompositionClassFields);


	// METHODS subsection : Operation methods (e.g. methods derive from operations but which arent constructors)
	//

	// setup/get/create the operations codeblock

	// public
	pubOperationsBlock = pubMethodsBlock->getHierarchicalCodeBlock("operationMethods", "Operations", 1);
	// set conditions for showing section comment
	CodeComment * pubOcomment = pubOperationsBlock->getComment();
	if (!forcedoc && !hasOperationMethods )
		pubOcomment->setWriteOutText(false);
	else
		pubOcomment->setWriteOutText(true);

	//protected
	protOperationsBlock = protMethodsBlock->getHierarchicalCodeBlock("operationMethods", "Operations", 1);
	// set conditions for showing section comment
	CodeComment * protOcomment = protOperationsBlock->getComment();
	if (!forcedoc && !hasOperationMethods )
		protOcomment->setWriteOutText(false);
	else
		protOcomment->setWriteOutText(true);

	//private
	privOperationsBlock = privMethodsBlock->getHierarchicalCodeBlock("operationMethods", "Operations", 1);
	// set conditions for showing section comment
	CodeComment * privOcomment = privOperationsBlock->getComment();
	if (!forcedoc && !hasOperationMethods )
		privOcomment->setWriteOutText(false);
	else
		privOcomment->setWriteOutText(true);

	// Operations
	//
	// nothing to do here.. "updateOperations" in parent class puts things
	// in the right place using the "addCodeOperation" method we defined in this class

	// FINISH up with hash def block close
	QString defTextEnd = "#endif //"+hashDefine + "_H";
	addOrUpdateTaggedCodeBlockWithComments("hashDefBlockEnd", defTextEnd, "", 0, false);

}


#include "cppheadercodedocument.moc"
