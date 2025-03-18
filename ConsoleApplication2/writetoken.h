#pragma once
#include"total.h"
#include"axil.h"
#include"analyse.h"

void writetoken()
{
	ofstream output(OUT1);

	int i = 0;
	while (tokenList[i].wd.tok != ENDFILE)
	{
		output << tokenList[i].line
			<< "\t" << std::left << setw(16) << GetString(tokenList[i].wd.tok)
			<< "\t" << tokenList[i].wd.str << endl;

		cout << tokenList[i].line
			<< "\t" << std::left << setw(16) << GetString(tokenList[i].wd.tok)
			<< "\t" << tokenList[i].wd.str << endl;

		i++;
	}
	output << tokenList[i].line
		<< "\t" << std::left << setw(16) << GetString(tokenList[i].wd.tok)
		<< "\t" << tokenList[i].wd.str << endl;

	cout << tokenList[i].line
		<< "\t" << std::left << setw(16) << GetString(tokenList[i].wd.tok)
		<< "\t" << tokenList[i].wd.str << endl;

	output.close();
}
