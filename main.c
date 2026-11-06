#define _GNU_SOURCE

/*
 * Source - https://stackoverflow.com/a/8438663
 * Posted by Lloyd Macrohon, modified by community. See post 'Timeline' for change history
 * Retrieved 2026-11-05, License - CC BY-SA 3.0
 *
 * https://stackoverflow.com/a/8438663
 *
 */

#include <sys/types.h>
#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_file(char *file, int is_last, int curr_itter, int fmt_arr[]) {

	char *branch;
	if (is_last == 1) {
		branch = "└";
		fmt_arr[curr_itter] = 0;
	} else {
		branch = "├";
		fmt_arr[curr_itter] = 1;
	}

	for (int i = 0; i < curr_itter; i++) {
		const char *c = (fmt_arr[i] == 0) ? " " : "│";
		printf("%s   ", c);
	}

	printf("%s── %s\n", branch, file);
}

void ls_dir(const char *dir, int indent, int arr[]) {

	DIR *dirp;
	if ((dirp = opendir(dir)) == NULL) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	struct dirent **namelist;
	int i,n;

	n = scandir(dir, &namelist, 0, alphasort);
	if (n < 0)
		perror("scandir");
	else {
		for (i = 0; i < n; i++) {

			if (namelist[i]->d_type == DT_DIR) {
				if ((strcmp(namelist[i]->d_name, ".") == 0) ||
					strcmp(namelist[i]->d_name, "..") == 0)
				{
					free(namelist[i]);
					continue;
				}
				print_file(namelist[i]->d_name, n-i, indent, arr);

				char path[4096];
				snprintf(path, sizeof(path), "%s/%s", dir, namelist[i]->d_name);
				ls_dir(path, indent + 1, arr);

			} else
				print_file(namelist[i]->d_name, n-i, indent, arr);

			free(namelist[i]);
		}
	}
	free(namelist);
	closedir(dirp);
}

int main(int argc, char *argv[]) {

	const char *cwd = (argc == 1) ? "." : argv[--argc];
	int stream_arr[20] = {0};
	stream_arr[0] = 1;
	
	printf("%s\n", cwd);
	ls_dir(cwd, 0, stream_arr);

	return 0;
}
