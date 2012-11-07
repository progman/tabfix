//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// 0.0.3
// Alexey Potehin http://www.gnuplanet.ru/doc/cv
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <algorithm>
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// global vars
namespace global
{
	bool flag_kill_backup = false;
	bool flag_debug       = false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// convert space to tab
int tabfix(FILE *fh, const char* p, size_t size)
{
	size_t offset = 0;
	bool flag_head_char = true;
	bool flag_head_line = true;


	while (offset != size)
	{
		if (flag_head_char == true)
		{
			flag_head_char = false;

			if
			(
				(p[offset + 0] == '/') &&
				((offset + 1) != size) && (p[offset + 1] == '/')
			)
			{
				fprintf (fh, "%c", '/');
				fprintf (fh, "%c", '/');
				offset += 2;
				continue;
			}
		}


		if ((p[offset + 0] != ' ') && (p[offset + 0] != '\t'))
		{
			flag_head_line = false;
		}


		if (flag_head_line == true)
		{
			if
			(
				(p[offset + 0] == ' ') &&
				((offset + 1) != size) && (p[offset + 1] == ' ') &&
				((offset + 2) != size) && (p[offset + 2] == ' ') &&
				((offset + 3) != size) && (p[offset + 3] == ' ')
			)
			{
				fprintf (fh, "%c", '\t');
				offset += 4;
				continue;
			}
		}


		if (p[offset + 0] == '\n')
		{
			flag_head_line = true;
			flag_head_char = true;
		}


		fprintf (fh, "%c", p[offset + 0]);
		offset++;
	}


	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// convert string to boolean
bool str2bool(const std::string& str)
{
	std::string tmp = str;

	std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);

	if (tmp == "true")
	{
		return true;
	}

	if (tmp == "t")
	{
		return true;
	}

	if (tmp == "on")
	{
		return true;
	}

	if (tmp == "1")
	{
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// return (concat str1 and str2) or NULL
char* concat_str(const char* str1, const char* str2)
{
	size_t str1_size = strlen(str1);
	size_t str2_size = strlen(str2);

	char* str3 = (char*)malloc(str1_size + str2_size + 1);
	if (str3 == NULL)
	{
		return NULL;
	}

	memcpy(str3, str1, str1_size);
	memcpy(str3 + str1_size, str2, str2_size);
	str3[str1_size + str2_size] = 0;

	return str3;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// для файла имя которого переданно преобразуем пробелы в таблуляции
int do_file(const char *filename)
{
	int rc; // error code


// create backup filename
	char* backup_filename = concat_str(filename, ".bak");
	if (backup_filename == NULL)
	{
		printf ("ERROR[concat_str()]: %s\n", strerror(errno));
		return -1;
	}


// create backup original file
	rc = rename(filename, backup_filename);
	if (rc == -1)
	{
		free(backup_filename);
		printf ("ERROR[rename()]: %s\n", strerror(errno));
		return -1;
	}


// open file
	rc = open(backup_filename, O_RDONLY);
	if (rc == -1)
	{
		free(backup_filename);
		printf ("ERROR[open()]: %s\n", strerror(errno));
		return -1;
	}
	int fd = rc;


// get file size
	struct stat my_stat;
	rc = fstat(fd, &my_stat);
	if (rc == -1)
	{
		close(fd);
		free(backup_filename);
		printf ("ERROR[fstat()]: %s\n", strerror(errno));
		return -1;
	}
	size_t size = my_stat.st_size;


// map file to memory
	void *pmmap = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (pmmap == MAP_FAILED)
	{
		close(fd);
		free(backup_filename);
		printf ("ERROR[mmap()]: %s\n", strerror(errno));
		return -1;
	}


// open new file
	FILE *fh = fopen(filename, "a+");
	if (fh == NULL)
	{
		close(fd);
		free(backup_filename);
		printf ("ERROR[fopen()]: %s\n", strerror(errno));
		return -1;
	}


// convert space to tab
	rc = tabfix(fh, (const char*)pmmap, size);
	if (rc == -1)
	{
		fclose(fh);
		munmap(pmmap, size);
		close(fd);
		free(backup_filename);
		return -1;
	}


// flush file
	rc = fflush(fh);
	if (rc != 0)
	{
		fclose(fh);
		munmap(pmmap, size);
		close(fd);
		free(backup_filename);
		printf ("ERROR[fflush()]: %s\n", strerror(errno));
		return -1;
	}


// close file
	rc = fclose(fh);
	if (rc != 0)
	{
		munmap(pmmap, size);
		close(fd);
		free(backup_filename);
		printf ("ERROR[fclose()]: %s\n", strerror(errno));
		return -1;
	}


// unmap file from memory
	rc = munmap(pmmap, size);
	if (rc == -1)
	{
		close(fd);
		free(backup_filename);
		printf ("ERROR[munmap()]: %s\n", strerror(errno));
		return -1;
	}


// close file
	rc = close(fd);
	if (rc == -1)
	{
		free(backup_filename);
		printf ("ERROR[close()]: %s\n", strerror(errno));
		return -1;
	}


	if (global::flag_kill_backup == true)
	{
// kill backup
		rc = unlink(backup_filename);
		if (rc == -1)
		{
			free(backup_filename);
			printf ("ERROR[unlink()]: %s\n", strerror(errno));
			return -1;
		}
	}


// free backup filename
	free(backup_filename);


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
		printf ("read from stdin...\n");
	}


	c = getchar();
	if (c == EOF)
	{
		printf ("\nERROR: stdin is close\n");
		return -1;
	}


	size_t size   = 4096;
	size_t delta  = 4096;
	size_t offset = 0;


	void *p_original = malloc(size);
	if (p_original == NULL)
	{
		printf ("ERROR[malloc()]: %s\n", strerror(errno));
		return -1;
	}
	char *p = (char*)p_original;


	for(;;)
	{
		if (offset >= size)
		{
			size += delta;
			p_original = realloc(p_original, size);
			if (p_original == NULL)
			{
				printf ("ERROR[realloc()]: %s\n", strerror(errno));
				return -1;
			}
			p = (char*)p_original + offset;
			continue;
		}

		*p = c; p++; offset++;

		c = getchar();
		if (c == EOF) break;
	}


// convert space to tab
	rc = tabfix(stdout, (const char*)p_original, offset);
	if (rc == -1)
	{
		free(p_original);
		return -1;
	}


// flush stdout
	fflush(stdout);


// free memory
	free(p_original);


	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// view help
void help()
{
	printf ("%s\t(%s)\n", PROG_FULL_NAME, PROG_URL);
	printf ("example: %s source_file\n", PROG_NAME);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// general function
int main(int argc, char* argv[])
{
	int rc;
	bool flag_file = false;


	for(int i=1; i < argc; i++)
	{
		std::string key = argv[i];
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


		tmpl = "--flag_debug=";
		if ((key.size() >= tmpl.size()) && (key.substr(0, tmpl.size()) == tmpl))
		{
			value = key.substr(tmpl.size(), key.size() - 1);
			global::flag_debug = str2bool(value);
			continue;
		}


		if (key == "-d")
		{
			global::flag_debug = true;
			continue;
		}


		if
		(
			(key == "-")  ||
			(key == "--")
		)
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
//		if (flag_file == true) printf ("Ok.\n");
		return 0;
	}


	return 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
