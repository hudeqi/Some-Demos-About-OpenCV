#include <algorithm>    
#include <vector>      
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;
int main_nfhnfng() {
	ifstream ifs1("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.31\\nbOCR_2018.1.31\\nbOCR\\Debug\\300hh.txt");//读取Label
	ifstream ifs2("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.31\\nbOCR_2018.1.31\\nbOCR\\Debug\\300cutSize.txt");//读取Result
	ofstream in;
	in.open("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.31\\nbOCR_2018.1.31\\nbOCR\\Debug\\300result.txt", ios::trunc);//切割结果数量写入文件
	string str1,str2;
	int count=0;
	while (ifs1>>str1 && ifs2>>str2)
	{
		//cout << count << " " << str1 << ": " << str1.length() / 2 << endl;
		if (str1 == str2) {
			++count;
			in << "Label:" << str1 << " result:" << str2 << "   T" << "\n";
		}
		else
		{
			in << "Label:" << str1 << " result:" << str2 << "   F" << "\n";
		}
		
	}
	cout << count*1.0 / 300 * 1.0 << endl;
	system("pause");
	/*string str;
	while (ifs1 >> str)
	{
		int size = str.length() / 2;
		in << size << "\n";
	}*/
	in.close();
	return 0;
}