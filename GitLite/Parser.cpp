//#include "Parser.h"
//
//Parser::Parser(String path, String Column)
//{
//	file.open(path.getData());
//	colNum = 0;
//	char c;
//	String columnName;
//	while (file.get(c) && c != '\n')
//	{
//		if (c == ',') {
//			if (columnName.compare(Column)) {
//				break;
//			}
//			colNum++;
//		}
//		else {
//			columnName = columnName + c;
//		}
//	}
//}
//
//Parser::~Parser()
//{
//}
