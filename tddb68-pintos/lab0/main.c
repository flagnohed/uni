#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
	char *name;
	struct list_elem elem;
};
typedef struct student student;
typedef struct list_elem list_elem;

void insert (struct list *student_list) {
	char *s_name = malloc(sizeof(20));
	printf("Enter student name: ");
	scanf("%s", s_name);
	printf("Name: %s \n", s_name);
	student *s = malloc(sizeof(student));
	s->name = s_name;

	list_push_back(student_list,&s->elem);

}

void delete (struct list *student_list) {
	char *s_name = malloc(sizeof(20));
	printf("Enter student name: ");
	scanf("%s", s_name);
	printf("Name: %s \n", s_name);

	list_elem *e;
	for (e = list_begin (student_list); e != list_end (student_list); e = list_next (e))
		{
			student *s = list_entry (e, struct student, elem);
			if(strcmp(s->name,s_name) == 0){
				list_remove(e);
				free(s->name);
				free(s);
			}
		}
		free(s_name);
}

void list (struct list *student_list) {
	list_elem *e;
	for (e = list_begin (student_list); e != list_end (student_list); e = list_next (e))
		{
			student *s = list_entry (e, struct student, elem);
			printf("Name: %s\n", s->name);
		}
}

void quit (struct list *student_list) {
	while (!list_empty (student_list))
		{
			list_elem *e = list_pop_front (student_list);
			student *s = list_entry(e,struct student,elem);
			free(s->name);
			free(s);
			free(e);
		}
		exit(0);
}

int main() {
	struct list student_list;
	list_init (&student_list);
	int opt;

	do {
		printf("Menu:\n");
		printf("1 - Insert student\n");
		printf("2 - Delete student\n");
		printf("3 - List students\n");
		printf("4 - Exit\n");
		scanf("%d", &opt);
		switch (opt) {
			case 1:
				{
					insert(&student_list);
					break;
				}
			case 2:
				{
					delete(&student_list);
					break;
				}
			case 3:
				{
					list(&student_list);
					break;
				}

			case 4:
				{
					quit(&student_list);
					break;
				}
			default:
				{
					printf("Quit? (1/0):\n");
					scanf("%d", &opt);
					if (opt)
						quit(&student_list);
					break;
				}
		}
	} while(1);

	return 0;
}
