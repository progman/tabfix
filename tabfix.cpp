//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// 0.0.2
// Alexey Potehin http://www.gnuplanet.ru/doc/cv
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// convert space to tab
int tabfix(const char *filename, const char* p, size_t size)
{
	int rc; // error code


// open new file
	FILE *fh = fopen(filename, "a+");
	if (fh == NULL)
	{
		printf ("ERROR[fopen()]: %s\n", strerror(errno));
		return -1;
	}


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


// flush file
	rc = fflush(fh);
	if (rc != 0)
	{
		printf ("ERROR[fflush()]: %s\n", strerror(errno));
		return -1;
	}


// close file
	rc = fclose(fh);
	if (rc != 0)
	{
		printf ("ERROR[fclose()]: %s\n", strerror(errno));
		return -1;
	}


	return 0;
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
int do_it(const char *filename)
{
	int rc; // error code


// create backup filename
	char* new_filename = concat_str(filename, ".bak");
	if (new_filename == NULL)
	{
		printf ("ERROR[concat_str()]: %s\n", strerror(errno));
		return -1;
	}


// create backup original file
	rc = rename(filename, new_filename);
	if (rc == -1)
	{
		printf ("ERROR[rename()]: %s\n", strerror(errno));
		free(new_filename);
		return -1;
	}


// open file
	rc = open(new_filename, O_RDONLY);
	if (rc == -1)
	{
		printf ("ERROR[open()]: %s\n", strerror(errno));
		free(new_filename);
		return -1;
	}
	int fd = rc;


// free backup filename
	free(new_filename);


// get file size
	struct stat my_stat;
	rc = fstat(fd, &my_stat);
	if (rc == -1)
	{
		printf ("ERROR[fstat()]: %s\n", strerror(errno));
		close(fd);
		return -1;
	}
	size_t size = my_stat.st_size;


// map file to memory
	void *pmmap = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (pmmap == MAP_FAILED)
	{
		printf ("ERROR[mmap()]: %s\n", strerror(errno));
		close(fd);
		return -1;
	}


// convert space to tab
	rc = tabfix(filename, (const char*)pmmap, size);
	if (rc == -1)
	{
		munmap(pmmap, size);
		close(fd);
		return -1;
	}


// unmap file from memory
	rc = munmap(pmmap, size);
	if (rc == -1)
	{
		printf ("ERROR[munmap()]: %s\n", strerror(errno));
		close(fd);
		return -1;
	}


// close file
	rc = close(fd);
	if (rc == -1)
	{
		printf ("ERROR[close()]: %s\n", strerror(errno));
		return -1;
	}


	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// general function
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf ("%s\t(%s)\n", PROG_FULL_NAME, PROG_URL);
		printf ("example: %s source_file\n", PROG_NAME);
		return 1;
	}


	if (do_it(argv[1]) == -1)
	{
		return 1;
	}


	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
