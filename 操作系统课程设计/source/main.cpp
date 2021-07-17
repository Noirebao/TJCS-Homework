#define _CRT_SECURE_NO_WORNINGS
#include "Kernel.h"
#include "Utility.h"

int main()
{
	Kernel* k = Kernel::getInstance();
	k->initialize();
	cout << "-----------------------------------------------------------------"	<< endl;
	cout << "����˵��:"															<< endl;
	cout << "init                                   : ��ʼ���ļ�ϵͳ"			<< endl;
	cout << "fformat                                : ��ʽ���ļ���"				<< endl;
	cout << "cd <dirname>                           : ������һ��Ŀ¼"			<< endl;
	cout << "ls                                     : �г���ǰĿ¼�������ļ�"	<< endl;
	cout << "mkdir <dirname>                        : �����µ�Ŀ¼"				<< endl;
	cout << "fcreate <filename>                     : �����ļ�"					<< endl;
	cout << "fdelete <filename>                     : ɾ���ļ�"					<< endl;
	cout << "fopen <filename>                       : ���ļ�"					<< endl;
	cout << "fclose <filename>                      : �ر��ļ�"					<< endl;
	cout << "fread <filename> <nbytes>              : ���ļ�"					<< endl;
	cout << "fwrite <filename> <nbytes> <string>    : д�ļ�"					<< endl;
	cout << "fseek <filename> <offset> <ptrname>    : �ƶ��ļ�ָ��"				<< endl;
	cout << "fmount <file1> <file2>                 : �����ļ�1���ļ�2��"		<< endl;
	cout << "exit                                   : �˳��ļ�ϵͳ"				<< endl;
	cout << "-----------------------------------------------------------------"	<< endl;
	cout << endl;

	while(1)
	{
		cout << "[Unix@FileSystem " << k->curdir << " ] $";
		char input[100];
		cin.getline(input, 100);
		vector<char*> result = Utility::parseCmd(input);
		if (result.size() > 0)
		{
			if (strcmp(result[0], "cd") == 0)
			{
				if (result.size() > 1)
				{
					k->cd(result[1]);
					if (k->error == Kernel::NOTDIR)
						cout << result[1] << ": Not a directory" << endl;
					else if (k->error == Kernel::NOENT)
						cout << result[1] << ": No such file or directory" << endl;
				}
				else;
			}
			else if (strcmp(result[0], "fformat") == 0)
			{
				k->format();
				k->initialize();
			}
			else if (strcmp(result[0], "mkdir") == 0)
			{
				if (result.size() > 1)
				{
					k->mkdir(result[1]);
					if (k->error == Kernel::NOENT)
						cout << "No such a file or directory" << endl;
					if (k->error == Kernel::ISDIR)
						cout << result[1] << ": This is a directory" << endl;
				}
				else
					cout << "Operand missed" << endl;

			}
			else if (strcmp(result[0], "ls") == 0)
			{
				k->ls();
			}
			else if (strcmp(result[0], "fopen") == 0)
			{
				int fd;
				if (result.size() > 1)
				{
					fd = k->open(result[1], 511);
					if (k->error == Kernel::NO_ERROR)
						cout << "fd = " << fd << endl;
					else if (k->error == Kernel::ISDIR)
						cout << result[1] << ": This is a directory" << endl;
					else if (k->error == Kernel::NOENT)
						cout << result[1] << ": No such a file or directory" << endl;
				}
				else
					cout << "Operand missed" << endl;

			}
			else if (strcmp(result[0], "fcreate") == 0)
			{
				int fd;
				if (result.size() > 1)
				{
					fd = k->create(result[1], 511);

					if (k->error == Kernel::NOENT)
						cout << "No such a file or directory" << endl;
					if (k->error == Kernel::ISDIR)
						cout << result[1] << ": This is a directory" << endl;
					if (k->error == Kernel::NO_ERROR)
						cout << "fd = " << fd << endl;
				}
				else
					cout << "Operand missed" << endl;
			}
			else if (strcmp(result[0], "fclose") == 0)
			{
				if (result.size() > 1)
					k->close(atoi(result[1]));
				else
					cout << "Operand missed" << endl;
			}
			else if (strcmp(result[0], "fread") == 0)
			{
				int actual;
				if (result.size() > 2) {
					char* buf;
					buf = new char[atoi(result[2])];
					buf[0] = '\0';
					actual = k->fread(atoi(result[1]), buf, atoi(result[2]));
					if (actual == -1)
					{
						if (k->error == Kernel::BADF)
							cout << atoi(result[1]) << ":This fd is wrong" << endl;
					}
					else
					{
						if (actual > 0)
						{
							for (int i = 0; i < actual; i++)
								//cout << buf[i];  
								//cout << buf[i] << endl;
								printf("%c", buf[i]);
							cout << endl;
						}
						cout << "Actually read " << actual << " bytes" << endl;
					}
					delete buf;
				}
				else
					cout << "Operand missed" << endl;
			}
			else if (strcmp(result[0], "fwrite") == 0)
			{
				int actual;
				if (result.size() > 3)
				{
					if (atoi(result[2]) > strlen(result[3]))
					{
						cout << "Read nbytes can not be larger than the length of the string" << endl;
						continue;
					}
					actual = k->fwrite(atoi(result[1]), result[3], atoi(result[2]));
					if (actual == -1)
					{
						if (k->error == Kernel::BADF)
							cout << atoi(result[1]) << ": This fd is wrong" << endl;
					}
					else
					{
						cout << "Actually write " << actual << " bytes" << endl;
					}
				}
				else
				{
					cout << "Operand missed" << endl;
				}
			}
			else if (strcmp(result[0], "fseek") == 0)
			{
				if (result.size() > 3)
				{
					if (atoi(result[3]) >= 0 && atoi(result[3]) <= 5)
					{
						k->fseek(atoi(result[1]), atoi(result[2]), atoi(result[3]));
						if (k->error == Kernel::BADF)
							cout << atoi(result[1]) << ": this fd is wrong" << endl;
					}
					else
						cout << result[3] << ": this ptrname is wrong" << endl;

				}
				else
				{
					cout << "Operand missed" << endl;
				}
			}
			else if (strcmp(result[0], "fdelete") == 0)
			{
				if (result.size() > 1)
				{
					k->fdelete(result[1]);
					if (k->error == Kernel::NOENT)
						cout << result[1] << ": No such a file or directory" << endl;
				}
				else
				{
					cout << "Operand missed" << endl;
				}
			}
			else if (strcmp(result[0], "fmount") == 0)
			{
				if (result.size() > 2)
				{
					k->fmount(result[1], result[2]);
					if (k->error == Kernel::NOENT)
						cout << result[2] << ": No such a file or directory" << endl;
					else if (k->error == Kernel::NOOUTENT)
						cout << result[1] << ": No such a file or directory" << endl;
					else if (k->error == Kernel::ISDIR)
						cout << result[2] << ": This is a directory" << endl;
				}
				else
				{
					cout << "Operand missed" << endl;
				}
			}
			else if (strcmp(result[0], "exit") == 0)
			{
				k->clear();
				break;
			}
			else if (strcmp(result[0], "init") == 0)
			{
				//fformat
				k->format();
				k->initialize();
				//mkdir
				char name1[64] = { 0 };
				char name2[64] = { 0 };
				strcpy_s(name1, "bin");
				k->mkdir(name1);
				strcpy_s(name1, "etc");
				k->mkdir(name1);
				strcpy_s(name1, "home");
				k->mkdir(name1);
				strcpy_s(name1, "dev");
				k->mkdir(name1);
				//cd
				strcpy_s(name1, "/home");
				k->cd(name1);
				//mkdir
				strcpy_s(name1, "texts");
				k->mkdir(name1);
				strcpy_s(name1, "reports");
				k->mkdir(name1);
				strcpy_s(name1, "photos");
				k->mkdir(name1);
				//put Report.pdf
				strcpy_s(name1, "/home/texts");
				k->cd(name1);
				strcpy_s(name1, "Report");
				k->create(name1, 511);
				strcpy_s(name1, "Report.pdf");
				strcpy_s(name2, "Report");
				k->fmount(name1, name2);
				k->close(atoi(name2));
				//put ReadMe.txt
				strcpy_s(name1, "/home/reports");
				k->cd(name1);
				strcpy_s(name1, "ReadMe");
				k->create(name1, 511);
				strcpy_s(name1, "ReadMe.txt");
				strcpy_s(name2, "ReadMe");
				k->fmount(name1, name2);
				k->close(atoi(name2));
				//put Photo.png
				strcpy_s(name1, "/home/photos");
				k->cd(name1);
				strcpy_s(name1, "Photo");
				k->create(name1, 511);
				strcpy_s(name1, "Photo.png");
				strcpy_s(name2, "Photo");
				k->fmount(name1, name2);
				k->close(atoi(name2));
				//return to root
				strcpy_s(name1, "/");
				k->cd(name1);
			}
			else {
				cout << "command \'" << result[0] << "\' not found" << endl;
			}
		}
	}
	return 0;
}