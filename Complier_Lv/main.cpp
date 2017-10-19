//
// Created by 吕百杨 on 14/10/2017.
//

#include "string"
#include "iostream"
#include "fstream"
#define MAX_LINE 100
#define LINE_LENGTH 100

const char* Keys[31] = {"auto", "double", "int", "struct", "break", "else", "long", "switch",
                        "case", "enum", "register", "typedef", "char", "extern", "return",
                        "union", "const", "float", "short", "unsigned", "continue", "for",
                        "signed", "void", "default", "goto", "sizeof", "do", "if", "while", "static"};
const char Marks[5] ={';', '\'', '\"', '{', '}'};
const char SingleOperators[19] = {'(', ')', '[', ']', '.', '!', '~', '-', '+', '*',
                                  '&', '|', '^', '/', '%', '<', '>', ',', '='};
const char* DoubleOpertors[16] = {
		"<=", ">=", "<<", ">>", "==", "!=", "&&", "||", "+=", "-=", "*=", "/=", "%=",  "->", "++", "--"};
//using char* will be warned, change it to const char*




/*
 *  待改进的地方
 *
 *  1.先查找是否是数字，字母等，平均查找时间比先查找是否是字符应该会少一些
 *  2.使用一个缓存对进行查找
 *  3.没有判断浮点数的形式，只要满足数字与点的结合我全算进浮点数里了
 *  4.没有保存结果的操作
 *  5.目前只能控制在100行，如何对大型程序进行编译是个问题（时间，空间，跟第二点可能有关联）
 *  6.单词和数字的循环，把NFA判断语句加入到while循环的判断语句
 *  7.\t没有处理
 */




using namespace std;
class Complier {
private:
	char* text[MAX_LINE];
	int LINE_NUM;

	bool isLetter(char s) {
		if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z'))
			return true;
		else
			return false;
	}
	bool isNum(char s) {
		if (s >= '0' && s <= '9')
			return true;
		else
			return false;
	}
	bool isOperator(char a) {
		bool flag = false;
		for (int i = 0; i < 19; i++) {
			if (a == SingleOperators[i]) {
				flag = true;
				break;
			}
		}
		return flag;
	}
	bool isDoubleOperator(char* start, char* end) {
		bool flag = false;
		char* e = new char[2];
		e[0]=start[0];
		e[1]=*end;
		for (int i = 0; i < 16; i ++)
			if(strcmp(e, DoubleOpertors[i]) == 0) {
				flag = true;
				break;
			}
		return flag;
	}
	bool isMark(char s) {
		bool flag = false;
		for (int i = 0; i < 5; i++) {
			if (s == Marks[i]) {
				flag = true;
				break;
			}
		}
		return flag;
	}
	bool isKeywords(char* words) {
		bool flag = false;
		for (int i = 0; i < 31; i++)
			if(strcmp(words, Keys[i]) == 0) {
				flag = true;
				break;
			}
		return flag;
	}
	void whatSingleWord(char* token) {
		if(isLetter(*token))
			cout << *token << " is ID" << endl;
		else if (isMark(*token))
			cout << *token << " is Mark" << endl;
		else if (isOperator(*token))
			cout << *token << " is Op" << endl;
		else if (isNum(*token))
			cout << *token << " is Num" << endl;
	}

public:
	Complier();//read file  +变量
	void lex();

};

Complier::Complier() {
	ifstream fin( "/Users/lvbaiyang/Desktop/text.txt" );
	const char* note1 = "//";
	string note1_s(note1);

	for (LINE_NUM = 0; !fin.eof(); LINE_NUM++) {
		text[LINE_NUM] = new char[LINE_LENGTH];
		fin.getline(text[LINE_NUM], LINE_LENGTH);
		if (strlen(text[LINE_NUM]) == 0) {
			LINE_NUM--;//i 表示一共有几行
			continue;
		}
		string k(text[LINE_NUM]);
		if (k.compare(0,2,note1,0,2) == 0) {//删除了//注释
			LINE_NUM--;
			continue;
		}
	}

	for (int i = 0; i < LINE_NUM; i++) {
		cout << text[i] << endl;
	}
}

void Complier::lex() {
	for (int i = 0; i < LINE_NUM; i++) {
		char* token = strtok(text[i], " ");
		char* start;
		char* end;
		while (token != NULL) {//这一行分析完
			//analysis
			if (strlen(token) == 1) {
				whatSingleWord(token);

				// 没有保存的操作
			} else if (strlen(token) > 1) {
				start = token;
				end = start+1;
				while (end == NULL || *end != '\0') {
					if (isOperator(*start)) {
						if (isOperator(*end)) {
							//检查是否是二元字符
							if (isDoubleOperator(start, end)) {
								cout << *start << *end << " is Op" << endl;
							} else {
								cout << *start << " is Op" << endl << *end << " is Op" << endl;
							}
							start = end + 1;
							if (start == NULL || *start == '\0')
								break;
							end = start + 1;
							if (end == NULL || *end == '\0') {
								whatSingleWord(start);
								break;
							}
						} else {
							cout << start[0] << " is Op" << endl;
							//没有保存的操作
							start = end;
							end++;
							if (end == NULL || *end == '\0') {
								whatSingleWord(start);
							}
						}
					} else if (isMark(*start)) {//MARK
						cout << start[0] << " is Mark" << endl;
						start = end;
						if (start == NULL || *start == '\0')
							break;
						end++;
						if (end == NULL || *end == '\0') {
							whatSingleWord(start);
						}
					} else if(isNum(*start)) {//NUM
						while (end != NULL) {
							if (isNum(*end) || *end == '.')
								end++;
							else
								break;
						}
						char* num = new char[end - start];
						for (int i = 0; i < end - start; i++) {
							num[i] = *(start+i);
						}
						cout << num << " is Number" << endl;
						start = end;
						if (start == NULL || *start == '\0')
							break;
						end++;
						if (end == NULL || *end == '\0') {
							whatSingleWord(start);
						}

					} else if(isLetter(*start) || *start == '_') {//ID
						while ((end != NULL && *end != '\0') && (isLetter(*end) || *end == '_' || isNum(*end))) {
							end++;
						}
						char* words = new char[end - start];
						for (int i = 0; i < end - start; i++) {
							words[i] = *(start+i);
						}
						if (isKeywords(words)) {
							cout << words << " is KeyWords" << endl;
						} else {
							cout << words << " is ID" << endl;
						}
						start = end;
						if (start == NULL || *start == '\0')
							break;
						end++;
						if (end == NULL || *end == '\0') {
							whatSingleWord(start);
						}
					} else if (start[0] == '\t') {
						start = end;
						if (start == NULL || *start == '\0')
							break;
						end++;
						if (end == NULL || *end == '\0') {
							whatSingleWord(start);
						}
					} else
						end = NULL;
				}
			}



			token = strtok(NULL, " ");
		}

	}
}

int main() {
	Complier a;
	a.lex();
	cout << endl << endl;
}