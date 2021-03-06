//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// 0.0.9
// Alexey Potehin <gnuplanet@gmail.com>, http://www.gnuplanet.ru/doc/cv
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <list>
#include <unistd.h>
#include <sys/mman.h>
#include "submodule/libcore.cpp/libcore.hpp"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// global vars
namespace global
{
	bool flag_kill_backup = false;
	bool flag_sync        = false;
	bool flag_debug       = false;
	bool flag_comment     = false;
	bool flag_mcbug       = false;
	bool flag_unix        = false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// find string
bool strfind(const char *p_data, size_t offset, size_t size, const char *p_token)
{
	if ((p_data == NULL) || (p_token == NULL) || (size == 0) || (offset >= size)) return false;


	p_data += offset;
	const char *p_data_end = p_data + size;


	for (;;)
	{
		char ch = *p_token;
		if (ch == 0) break;

		if (p_data == p_data_end) return false;

		if (ch != *p_data) return false;

		p_data++;
		p_token++;
	}


	return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// convert space to tab
int tabfix(FILE *fh, const char *p, size_t size)
{
	size_t offset = 0;
	bool flag_head_line = true;


	while (offset < size)
	{
		if (global::flag_comment == true)
		{
// skip '//' in head line
			if (strfind(p, offset, size, "//") == true)
			{
				fprintf(fh, "%c", '/');
				fprintf(fh, "%c", '/');
				offset += 2;
				continue;
			}
		}


		if ((flag_head_line == true) && (global::flag_mcbug == true))
		{

//<><-->comment
 //><-->comment
  //<--><-->comment
   //<-><-->comment

// convert '<-->' to '\t' (for mcedit)
			if (strfind(p, offset, size, "<-->") == true)
			{
				fprintf(fh, "%c", '\t');
				offset += 4;
				continue;
			}

// convert '<->' to '\t' (for mcedit)
			if (strfind(p, offset, size, "<->") == true)
			{
				fprintf(fh, "%c", '\t');
				offset += 3;
				continue;
			}

// convert '<>' to '\t' (for mcedit)
			if (strfind(p, offset, size, "<>") == true)
			{
				fprintf(fh, "%c", '\t');
				offset += 2;
				continue;
			}

// convert '>' to '\t' (for mcedit)
			if (strfind(p, offset, size, ">") == true)
			{
				fprintf(fh, "%c", '\t');
				offset += 1;
				continue;
			}
		}



		if ((p[offset] != ' ') && (p[offset] != '\t'))
		{
			flag_head_line = false;
		}


		if (flag_head_line == true)
		{
// convert '    ' to '\t'
			if (strfind(p, offset, size, "    ") == true)
			{
				fprintf(fh, "%c", '\t');
				offset += 4;
				continue;
			}
		}


		if (p[offset] == '\n')
		{
			flag_head_line = true;
		}


		if ((p[offset] == '\r') && (global::flag_unix == true))
		{
			offset++;
			continue;
		}


		fprintf(fh, "%c", p[offset]);
		offset++;
	}


	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// для файла имя которого переданно преобразуем пробелы в таблуляции
int do_file(const char *filename)
{
	int rc; // error code
	size_t size; // file size
	char tmp[256]; // buffer for readlink


// get file size
	struct stat my_stat;
	rc = lstat(filename, &my_stat);
	if (rc == -1)
	{
		printf("ERROR[stat()]: %s\n", strerror(errno));
		return -1;
	}
	size = my_stat.st_size;


// read link
	if (S_ISLNK(my_stat.st_mode) != false)
	{
		rc = readlink(filename, tmp, sizeof(tmp));
		if (rc == -1)
		{
			printf("ERROR[readlink()]: %s\n", strerror(errno));
			return -1;
		}

		if (rc == sizeof(tmp))
		{
			printf("ERROR[readlink()]: buffer too small\n");
			return -1;
		}


		tmp[rc] = 0;
		filename = tmp;


		rc = stat(filename, &my_stat);
		if (rc == -1)
		{
			printf("ERROR[stat()]: %s\n", strerror(errno));
			return -1;
		}
		size = my_stat.st_size;
	}


// check file type
	if (S_ISREG(my_stat.st_mode) == false)
	{
		printf("ERROR[stat()]: this is not regular file\n");
		return -1;
	}


// create backup filename
	char *backup_filename = libcore::concat_str(filename, ".bak");
	if (backup_filename == NULL)
	{
		printf("ERROR[concat_str()]: %s\n", strerror(errno));
		return -1;
	}


// create backup original file
	rc = rename(filename, backup_filename);
	if (rc == -1)
	{
		free(backup_filename);
		printf("ERROR[rename()]: %s \"%s\"\n", strerror(errno), filename);
		return -1;
	}


// open file
	rc = open(backup_filename, O_RDONLY);
	if (rc == -1)
	{
		free(backup_filename);
		printf("ERROR[open()]: %s\n", strerror(errno));
		return -1;
	}
	int fd = rc;


// map file to memory
	void *pmmap = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (pmmap == MAP_FAILED)
	{
		close(fd);
		free(backup_filename);
		printf("ERROR[mmap()]: %s\n", strerror(errno));
		return -1;
	}


// open new file
	FILE *fh = fopen(filename, "a+");
	if (fh == NULL)
	{
		close(fd);
		free(backup_filename);
		printf("ERROR[fopen()]: %s\n", strerror(errno));
		return -1;
	}


// convert space to tab
	rc = tabfix(fh, (const char *)pmmap, size);
	if (rc == -1)
	{
		fclose(fh);
		munmap(pmmap, size);
		close(fd);
		free(backup_filename);
		return -1;
	}


// flush file
	if (global::flag_sync == true)
	{
		rc = fflush(fh);
		if (rc != 0)
		{
			fclose(fh);
			munmap(pmmap, size);
			close(fd);
			free(backup_filename);
			printf("ERROR[fflush()]: %s\n", strerror(errno));
			return -1;
		}
	}


// close file
	rc = fclose(fh);
	if (rc != 0)
	{
		munmap(pmmap, size);
		close(fd);
		free(backup_filename);
		printf("ERROR[fclose()]: %s\n", strerror(errno));
		return -1;
	}


// unmap file from memory
	rc = munmap(pmmap, size);
	if (rc == -1)
	{
		close(fd);
		free(backup_filename);
		printf("ERROR[munmap()]: %s\n", strerror(errno));
		return -1;
	}


// close file
	rc = close(fd);
	if (rc == -1)
	{
		free(backup_filename);
		printf("ERROR[close()]: %s\n", strerror(errno));
		return -1;
	}


	if (global::flag_kill_backup == true)
	{
// kill backup
		rc = unlink(backup_filename);
		if (rc == -1)
		{
			free(backup_filename);
			printf("ERROR[unlink()]: %s\n", strerror(errno));
			return -1;
		}
	}


// free backup filename
	free(backup_filename);


// set old permission bits for new file
	rc = chmod(filename, my_stat.st_mode);
	if (rc == -1)
	{
		printf("ERROR[chmod()]: %s\n", strerror(errno));
		return -1;
	}


	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// в потоке который подан нам на stdinput преобразуем пробелы в таблуляции
int do_stdinput()
{
	int rc;
	int c;


	if (global::flag_debug != false)
	{
		printf("read from stdin...\n");
	}


	c = getchar();
	if (c == EOF)
	{
		printf("\nERROR: stdin is close\n");
		return -1;
	}


	size_t size   = 0;
	size_t delta  = 4096;
	size_t offset = 0;


	void *p_original = NULL;
	void *p_original_old;
	char *p = (char *)p_original;


	for (;;)
	{
		if (offset >= size)
		{
			size += delta;
			p_original_old = p_original;
			p_original = realloc(p_original, size);
			if (p_original == NULL)
			{
				free(p_original_old);
				printf("ERROR[realloc()]: %s\n", strerror(errno));
				return -1;
			}
			p = (char *)p_original + offset;
			continue;
		}

		*p = c; p++; offset++;

		c = getchar();
		if (c == EOF) break;
	}


// convert space to tab
	rc = tabfix(stdout, (const char *)p_original, offset);
	if (rc == -1)
	{
		free(p_original);
		return -1;
	}


// flush stdout
	if (global::flag_sync == true)
	{
		fflush(stdout);
	}


// free memory
	free(p_original);


	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// view help
void help()
{
	printf("%s    %s\n", PROG_FULL_NAME, PROG_URL);
	printf("example: %s [-kb, --] [source_file]\n", PROG_NAME);
	printf("\n");

	printf("Convert spaces to tabs in FILE(s), or standard input, to file(s)\n");
	printf("\n");
	printf("  -h, -help, --help                this message\n");
	printf("  -s, --flag_sync=true|false       sync write\n");
	printf("  -c, --flag_comment=true|false    convert head comment\n");
	printf("  -m, --flag_mcbug=true|false      convert mcbug\n");
	printf("  -kb                              kill backup file\n");
	printf("  -u, --flag_unix=true|false       skip 0x0D byte\n");
	printf("With no FILE, or when FILE is --, read standard input.\n");
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// split string use token to std::list
size_t tokenize2list(const std::string &s, const std::string &token, std::list<std::string> &out, bool exclude_tail)
{
	out.clear();
	size_t size = 0;
	size_t index1 = 0;

	while (index1 < s.size())
	{
		size_t index2 = s.find(token, index1);
		if (index2 == size_t(-1))
		{
			if (exclude_tail != false) return size;

			index2 = s.size();
		}

		std::string tmp = s.substr(index1, index2 - index1);
		if (tmp.empty() == false)
		{
			out.push_back(tmp);
			size++;
		}
		index1 = index2 + token.size();
	}

	return size;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// get all program arguments
int get_args(int argc, char **argv, std::list<std::string> &args, const char *name)
{
	size_t count = 0;
	args.clear();


	char *p = getenv(name);
	if (p != NULL)
	{
		std::list<std::string> out;
		size_t size = tokenize2list(p, " ", out, false);
		for (size_t i=0; i < size; i++)
		{
			args.push_back(out.front());
			out.pop_front();
			count++;
		}
	}


	for (int i=1; i < argc; i++)
	{
		args.push_back(argv[i]);
		count++;
	}


	return count;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// general function
int main(int argc, char *argv[])
{
	int rc = 0;
	bool flag_file = false;


	std::list<std::string> args;
	int args_size = get_args(argc, argv, args, "TABFIX");


	for (int i=0; i < args_size; i++)
	{
		std::string key = args.front();
		args.pop_front();

		std::string tmpl;
		std::string value;


		if
		(
			(key == "-h")    ||
			(key == "-help") ||
			(key == "--help")
		)
		{
			help();
			return 1;
		}


		if (key == "-kb")
		{
			global::flag_kill_backup = true;
			continue;
		}


		tmpl = "--flag_sync=";
		if ((key.size() >= tmpl.size()) && (key.substr(0, tmpl.size()) == tmpl))
		{
			value = key.substr(tmpl.size(), key.size() - 1);
			libcore::str2bool(global::flag_sync, false, value);
			continue;
		}

		if (key == "-s")
		{
			global::flag_sync = true;
			continue;
		}


		tmpl = "--flag_debug=";
		if ((key.size() >= tmpl.size()) && (key.substr(0, tmpl.size()) == tmpl))
		{
			value = key.substr(tmpl.size(), key.size() - 1);
			libcore::str2bool(global::flag_debug, false, value);
			continue;
		}

		if (key == "-d")
		{
			global::flag_debug = true;
			continue;
		}


		tmpl = "--flag_comment=";
		if ((key.size() >= tmpl.size()) && (key.substr(0, tmpl.size()) == tmpl))
		{
			value = key.substr(tmpl.size(), key.size() - 1);
			libcore::str2bool(global::flag_comment, false, value);
			continue;
		}

		if (key == "-c")
		{
			global::flag_comment = true;
			continue;
		}


		tmpl = "--flag_mcbug=";
		if ((key.size() >= tmpl.size()) && (key.substr(0, tmpl.size()) == tmpl))
		{
			value = key.substr(tmpl.size(), key.size() - 1);
			libcore::str2bool(global::flag_mcbug, false, value);
			if (global::flag_mcbug == true) global::flag_comment = true;
			continue;
		}

		if (key == "-m")
		{
			global::flag_comment = true;
			global::flag_mcbug   = true;
			continue;
		}


		tmpl = "--flag_unix=";
		if ((key.size() >= tmpl.size()) && (key.substr(0, tmpl.size()) == tmpl))
		{
			value = key.substr(tmpl.size(), key.size() - 1);
			libcore::str2bool(global::flag_unix, false, value);
			continue;
		}

		if (key == "-u")
		{
			global::flag_unix = true;
			continue;
		}


		if (key == "--")
		{
			break;
		}


		flag_file = true;
		rc = do_file(key.c_str());
		if (rc != 0) break;
	}


	if (flag_file == false)
	{
		rc = do_stdinput();
	}


	if (rc == 0)
	{
//		if (flag_file == true) printf("Ok.\n");
		return 0;
	}


	return 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
