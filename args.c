#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void print_help() {
	printf("Date Difference Calculator\n"
			"\n"
			"Usage:\n"
  			"date-calc -1 YYYYMMDD -2 YYYYMMDD\n"
			"\n"
			"Options:\n"
			"  -1, --date1=DATE    First date (format: YYYYMMDD)\n"
			"  -2, --date2=DATE    Second date (format: YYYYMMDD)\n"
			"  -h, --help          Show this help message\n"
			"\n"
			"Examples:\n"
			"  date-calc -1 20250115 -2 20260907\n"
			"  date-calc --date1=20250115 --date2=20260907\n"
			"\n"
			"Date format:\n"
			"  YYYYMMDD     (8 digits: year, month, day)\n"
			"  Example: 20250115 = January 15, 2025\n"
			"\n"
			"Output:\n"
			"  Years, months, weeks, days difference\n"
			"  Total days difference\n");
	exit(0);
}

typedef struct {
	int date1;
	int date2;
} dates;

typedef struct {
	char* long_arg;
	char* short_arg;
	int long_leigth;
} args;

dates parse(int argc, char *argv[]) {
	dates result = {0};
	bool date1_set = false;
	bool date2_set = false;
	args arg[] = {
		{"--help", "-h", 6},
		{"--date1=", "-1", 8},
		{"--date2=", "-2", 8}
	};
	if (argc == 1)
		print_help();
	for (int i=1; i<argc; i++){
		int arg_type = -1;
		if (strncmp(argv[i], arg[1].long_arg, arg[1].long_leigth) == 0)
			arg_type = 1;
		else if (strcmp(argv[i], arg[1].short_arg) == 0)
			arg_type = 2;
		else if (strncmp(argv[i], arg[2].long_arg, arg[2].long_leigth) == 0)
			arg_type = 3;
		else if (strcmp(argv[i], arg[2].short_arg) == 0)
			arg_type = 4;
		else if (strcmp(argv[i], arg[0].long_arg) == 0 ||
				strcmp(argv[i], arg[0].short_arg) == 0)
			arg_type = 0;

		switch(arg_type) {
			
			case 0:{
				print_help();
				break;
			}
			case 1: {
				result.date1 = strtoll((argv[i] + arg[1].long_leigth), NULL, 10);
				date1_set = true;
				break;
			}
			case 2: {
				if (i + 1 >= argc) {
					fprintf(stderr, "Error: -1 requires an argument\n");
					exit(1);
				}else {
					result.date1 = strtoll(argv[i+1], NULL, 10);
					date1_set = true;
					i++;
					break;
				}
			}
			case 3: {
				result.date2 = strtoll((argv[i] + arg[2].long_leigth), NULL, 10);
				date2_set = true;
				break;
			}
			case 4: {
				if (i + 1 >= argc) {
					fprintf(stderr, "Error: -2 requires an argument\n");
					exit(2);
				}else {
					result.date2 = strtoll(argv[i+1], NULL, 10);
					date2_set = true;
					i++;
					break;
				}
			}
			default: {
				fprintf(stderr, "unkown parametr: %s\n", argv[i]);
				exit(3);
			}
		}
	}
	if (date1_set && date2_set)
		return result;
	else {
		fprintf(stderr, "Error: Both -1/--date1 and -2/--date2 are required\n");
        exit(4);
	}
}
