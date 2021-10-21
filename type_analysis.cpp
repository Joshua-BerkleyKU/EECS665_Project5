#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include "types.hpp"
#include "name_analysis.hpp"
#include "type_analysis.hpp"

namespace cshanty{

TypeAnalysis * TypeAnalysis::build(NameAnalysis * nameAnalysis){
	//To emphasize that type analysis depends on name analysis
	// being complete, a name analysis must be supplied for 
	// type analysis to be performed.
	TypeAnalysis * typeAnalysis = new TypeAnalysis();
	auto ast = nameAnalysis->ast;	
	typeAnalysis->ast = ast;

	ast->typeAnalysis(typeAnalysis);
	if (typeAnalysis->hasError){
		return nullptr;
	}

	return typeAnalysis;

}

void ProgramNode::typeAnalysis(TypeAnalysis * ta){

	//pass the TypeAnalysis down throughout
	// the entire tree, getting the types for
	// each element in turn and adding them
	// to the ta object's hashMap
	for (auto global : *myGlobals){
		global->typeAnalysis(ta);
	}

	//The type of the program node will never
	// be needed. We can just set it to VOID
	//(Alternatively, we could make our type 
	// be error if the DeclListNode is an error)
	ta->nodeType(this, BasicType::produce(VOID));
}

void FnDeclNode::typeAnalysis(TypeAnalysis * ta){

	//HINT: you might want to change the signature for
	// typeAnalysis on FnBodyNode to take a second
	// argument which is the type of the current 
	// function. This will help you to know at a 
	// return statement whether the return type matches
	// the current function

	//Note: this function may need extra code

	for (auto stmt : *myBody){
		stmt->typeAnalysis(ta);
	}
}

void StmtNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Implement me in the subclass");
}

void AssignStmtNode::typeAnalysis(TypeAnalysis * ta){
	myExp->typeAnalysis(ta);

	//It can be a bit of a pain to write 
	// "const DataType *" everywhere, so here
	// the use of auto is used instead to tell the
	// compiler to figure out what the subType variable
	// should be
	auto subType = ta->nodeType(myExp);

	// As error returns null if subType is NOT an error type
	// otherwise, it returns the subType itself
	if (subType->asError()){
		ta->nodeType(this, subType);
	} else {
		ta->nodeType(this, BasicType::produce(VOID));
	}
}

void ExpNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Override me in the subclass");
}

void AssignExpNode::typeAnalysis(TypeAnalysis * ta){
	//TODO: Note that this function is incomplete. 
	// and needs additional code

	//Do typeAnalysis on the subexpressions
	myDst->typeAnalysis(ta);
	mySrc->typeAnalysis(ta);

	const DataType * tgtType = ta->nodeType(myDst);
	const DataType * srcType = ta->nodeType(mySrc);

	//While incomplete, this gives you one case for 
	// assignment: if the types are exactly the same
	// it is usually ok to do the assignment. One
	// exception is that if both types are function
	// names, it should fail type analysis
	if (tgtType == srcType){
		ta->nodeType(this, tgtType);
		return;
	}
	
	//Some functions are already defined for you to
	// report type errors. Note that these functions
	// also tell the typeAnalysis object that the
	// analysis has failed, meaning that main.cpp
	// will print "Type check failed" at the end
	ta->errAssignOpr(this->pos());


	//Note that reporting an error does not set the
	// type of the current node, so setting the node
	// type must be done
	ta->nodeType(this, ErrorType::produce());
}

void DeclNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Override me in the subclass");
}

void VarDeclNode::typeAnalysis(TypeAnalysis * ta){
	// VarDecls always pass type analysis, since they 
	// are never used in an expression. You may choose
	// to type them void (like this), as discussed in class
	ta->nodeType(this, BasicType::produce(VOID));
}

void IDNode::typeAnalysis(TypeAnalysis * ta){
	// IDs never fail type analysis and always
	// yield the type of their symbol (which
	// depends on their definition)
	ta->nodeType(this, this->getSymbol()->getDataType());
}

void PlusNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void MinusNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);

	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void TimesNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void DivideNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void AndNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isBool())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isBool() || !myExp1->isFnCall())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isBool())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isBool() || !myExp2->isFnCall())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void OrNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isBool())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isBool() || !myExp1->isFnCall())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isBool())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isBool() || !myExp2->isFnCall())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void EqualsNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);

	if (left->asFn() == nullptr)
	{
		if (right->asFn() == nullptr)
		{
			if (left == right)
			{
				ta->nodeType(this, left);
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
		}
		else {
			if (!myExp2->isFnCall())
			{
				ta->errEqOpd(this->pos());
			}
			
			if (left == right->asFn()->getReturnType())
			{
				ta->nodeType(this, left);
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
		}
	}
	else {
		if (right->asFn() == nullptr)
		{
			if (!myExp1->isFnCall())
			{
				ta->errEqOpd(this->pos());
			}
			
			if (left->asFn()->getReturnType() == right)
			{
				ta->nodeType(this, right);
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
		}
		else {
			if (!myExp1->isFnCall() || !myExp2->isFnCall())
			{
				ta->errEqOpd(this->pos());
			}
			
			if (left->asFn()->getReturnType() == right->asFn()->getReturnType())
			{
				ta->nodeType(this, left);
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
			
		}
	}
	ta->nodeType(this, ErrorType::produce());
}

void NotEqualsNode::typeAnalysis(TypeAnalysis * ta) {
	
}

void LessNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void LessEqNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void GreaterNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void GreaterEqNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(this->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void IntLitNode::typeAnalysis(TypeAnalysis * ta){
	// IntLits never fail their type analysis and always
	// yield the type INT
	ta->nodeType(this, BasicType::produce(INT));
}

void StrLitNode::typeAnalysis(TypeAnalysis * ta){
	// IntLits never fail their type analysis and always
	// yield the type INT
	ta->nodeType(this, BasicType::produce(STRING));
}

void TrueNode::typeAnalysis(TypeAnalysis * ta){
	// IntLits never fail their type analysis and always
	// yield the type INT
	ta->nodeType(this, BasicType::produce(BOOL));
}


void FalseNode::typeAnalysis(TypeAnalysis * ta){
	// IntLits never fail their type analysis and always
	// yield the type INT
	ta->nodeType(this, BasicType::produce(BOOL));
}


}
