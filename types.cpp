#include <list>
#include <sstream>

#include "types.hpp"
#include "ast.hpp"

namespace cshanty{

std::string BasicType::getString() const{
	std::string res = "";
	switch(myBaseType){
	case BaseType::INT:
		res += "int";
		break;
	case BaseType::BOOL:
		res += "bool";
		break;
	case BaseType::VOID:
		res += "void";
		break;
	case BaseType::STRING:
		res += "string";
		break;
	}
	return res;
}

const DataType * StringTypeNode::getType() const { 
	return BasicType::STRING(); 
}

const DataType * BoolTypeNode::getType() const { 
	return BasicType::BOOL(); 
}

const DataType * IntTypeNode::getType() const { 
	return BasicType::INT(); 
}

const DataType * VoidTypeNode::getType() const { 
	return BasicType::VOID(); 
}

const DataType * RecordTypeNode::getType() const{ 
	return myType;
}


} //End namespace
