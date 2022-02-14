
#include <windows.h>
#include <iostream>
#include <string>
#include <ostream>
#include <fstream> 
#include <locale.h>
#include <io.h>

using namespace std;

struct ifiles
{
	string word;
	int i;
	string files;
	ifiles* next = NULL;
};
wstring katalog;
WIN32_FIND_DATA FindFileData;
HANDLE hf;
wstring z = L"*";
ifiles imass[100];
wstring  path(L"files\\");
LPCWSTR sw;
int Search(string key)
{
	int i = 0;
	while (imass[i].word != "") {
		if (imass[i].word == key) return i;
		i++;
	}
	return -1;
}

void ind() {
	string s, s1, s2, s3, ss;
	int i = 0;
	wstring  sfiles;
	string files;
	katalog = wstring(path) + wstring(z);
	sw = katalog.c_str();

	ifstream file;
	hf = FindFirstFile(sw, &FindFileData);
	do
	{

		wcout << FindFileData.cFileName << endl;
		sfiles = wstring(path) + wstring(FindFileData.cFileName);
		string files(sfiles.begin(), sfiles.end());
		file.open(files);
		if (file.is_open()) {

			for (file >> s; !file.eof(); file >> s) {

				int pos = -1;
				pos = Search(s);
				if (pos != -1) {
					if (imass[pos].next != NULL) {
						ifiles* tmp = imass[pos].next;
						while (tmp->next != NULL)tmp = tmp->next;
						ifiles* temp = new ifiles;
						temp->i = file.tellg();
						temp->files = files;
						temp->word = s;
						tmp->next = temp;
					}
					else
					{
						ifiles* temp = new ifiles;
						temp->i = file.tellg();
						temp->files = files;
						temp->word = s;
						imass[pos].next = temp;
					}
				}
				else
				{
					imass[i].word = s;
					imass[i].i = file.tellg();
					imass[i].files = files;  i++;
				}
			}
			file.close();
		}
	} while (FindNextFile(hf, &FindFileData) != 0);
	FindClose(hf);


}
void write() {

	ofstream ofile("index.txt", ios::out | ios::trunc);
	int i = 0;
	while (imass[i].word != "") {
		if (imass[i].next == NULL) {
			string s(imass[i].files.begin(), imass[i].files.end());
			ofile << "---- ";
			ofile << "* " << imass[i].word << " ** " << imass[i].i << " *** " << s << " ++++" << endl;
		}
		else
		{
			string s(imass[i].files.begin(), imass[i].files.end());

			ofile << "---- ";
			ofile << "* " << imass[i].word << " ** " << imass[i].i << " *** " << s;
			ifiles* tmp = imass[i].next;

			string ss(tmp->files.begin(), tmp->files.end());

			ofile << " * " << tmp->word << " ** " << tmp->i << " *** " << ss;

			if (tmp->next == NULL) ofile << " ++++" << endl;

			while (tmp->next != NULL) {

				string s(tmp->files.begin(), tmp->files.end());
				ofile << " * " << tmp->word << " ** " << tmp->i << " *** " << s;
				tmp = tmp->next;
				if (tmp->next == NULL) ofile << " ++++" << endl;
			}
		}
		i++;
	}ofile.close();

}
void read(string s1) {
	ifstream ifile;
	bool found = false;
	string s, s2, s3, ss;
	int i = 0, j = 0;
	ifile.open("index.txt");
		if (ifile.is_open()) {
			cout << "Искомое значение: " << s1 << endl;
			for (ifile >> s; !ifile.eof(); ifile >> s) {
				if (s == s1) {
				
					s3 = s1;
					found = true;
				}
				
				else if (found == true && s == "**") {
					s2 = s;
				}
				else if (found == true && s2 == "**") {

					j = stoi(s);
					s2 = "";
				}
				else if (found == true && s == "***") {
					s2 = s;
				}
				else if (found == true && s2 == "***") {
					ifstream readfile;
					readfile.open(s);
					cout << "Файл: " << s << endl;
					
					if (readfile.is_open()) {
						cout << s3 << " " ;
						readfile.seekg(j);
						for (readfile >> s; !readfile.eof(); readfile >> s) {

							cout << s <<" ";
					}
						cout << endl;
						found = false; s2 = s3 = ""; j = 0;
				}
				}
			}
		}

}

void clear() {
	int i = 0;
	while (imass[i].word != "") {
		imass[i].word = "";
		imass[i].i = 0;
		imass[i].next = NULL;
		imass[i].files = "";
		i++;
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");
	bool found = false;
	string s, s1 , s2, s3, ss;
	int i = 0, j = 0, choice, f = 0;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

		do
		{
			printf("\n1.Вывести текущий каталог файлов"
				"\n2.Изменить каталог и проиндексировать "
				"\n3.Поиск по каталогу "
				"\n4.Повторно проиндексировать файлы "
				"\n5.Завершить"
				"\n\n Пожалуйста, выберите нужный вариант: ");

			scanf_s("%d", &choice);
			switch (choice)
			{
			case 1:
				cout << "Текущий каталог с файлами: " ;
				wcout << path << endl;
				cout << "Файлы в каталоге: "  << endl;
				katalog = wstring(path) + wstring(z);
				sw = katalog.c_str();
				hf = FindFirstFile(sw, &FindFileData);
				do
				{
					wcout << FindFileData.cFileName << endl;
				} while (FindNextFile(hf, &FindFileData) != 0);
				break;
			case 2:
				cout << "Текущий каталог с файлами: ";
				wcout <<  path << endl;
				cout << "Введи новый каталог, в формате ""C:\\katalog\\"": ";
				wcin >> path;
				cout << "Текущий каталог с файлами: ";
				wcout  << path << endl;
				katalog = wstring(path) + wstring(z);
				 sw = katalog.c_str();
				hf = FindFirstFile(sw, &FindFileData);
				cout << "Файлы в каталоге: " << endl;
				clear();
				ind();
				write();

		break;
			case 3:
		cout << "Введите слово для поиска: ";
		cin >> s1;

		read(s1);
	
		break;
			case 4:
				clear();
				ind();
				write();
				break;
			case 5:
				f = 1;
				break;
			default:
				printf("Недействительный ввод\n");
	}
} while (f != 1);

	system("pause"); 
}
	
