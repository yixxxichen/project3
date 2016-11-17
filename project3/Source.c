#include<windows.h>
#include "mysql.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#pragma comment(lib,"libmysql.lib")

MYSQL *conn;
char username[50], password[50],year[50],quarter[3];

void login();

void student_menu();

void show_course_details(char *c_num) {
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char query[200],year[10],quarter[10];
	memset(query, 0, 200);
	memset(quarter, 0, 10);
	memset(year, 0, 10);
	printf("****************************\n");
	printf("Course number:\t%s\n", c_num);
	//Course name
	sprintf(query, "SELECT UoSName FROM unitofstudy WHERE UoSCode=\"%s\" ", c_num);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Course Title:\t%s\n",row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//Year
	sprintf(query, "SELECT Year FROM transcript WHERE UoSCode=\"%s\" and StudId=\"%s\" ", c_num, username);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Year:\t%s\n", row[0]);
	strcpy(year, row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//Semester
	sprintf(query, "SELECT Semester FROM transcript WHERE UoSCode=\"%s\"and StudId=\"%s\"  ", c_num, username);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Semester:\t%s\n", row[0]);
	strcpy(quarter, row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//enrollment
	sprintf(query, "SELECT Enrollment FROM uosoffering WHERE UoSCode=\"%s\"and Year = \"%s\" and Semester=\"%s\" ", c_num, year, quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Student Enrollment:\t%s\n", row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//maxenrollment
	sprintf(query, "SELECT MaxEnrollment FROM uosoffering WHERE UoSCode=\"%s\"and Year = \"%s\" and Semester=\"%s\" ", c_num, year, quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Student MaxEnrollment:\t%s\n", row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//leturer name
	sprintf(query, "SELECT Name FROM faculty WHERE Id in (select InstructorId from uosoffering where UoSCode=\"%s\"and Year = \"%s\" and Semester=\"%s\") ", c_num, year, quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Lecturer Name:\t%s\n", row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	//Student Grade
	sprintf(query, "SELECT Grade FROM transcript WHERE UoSCode=\"%s\"and StudId=\"%s\"  ", c_num, username);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	row = mysql_fetch_row(res_set);
	printf("Your Grade:\t%s\n", row[0]);
	mysql_free_result(res_set);
	memset(query, 0, 200);
	printf("****************************\n");
}

void display_transcript_operation() {
	printf("Which operation you want to take?\n");
	printf("1.Show course Details.\n");
	printf("2.Back to Student Menu.\n");
	printf("****************************\n");
	printf("Please enter the operation number.\n");
	int op_num=0;
	scanf(" %d", &op_num);
	switch (op_num) {
	case 1: {printf("Please enter the course number to show the details.\n");
		char c_num[20];
		scanf(" %s", &c_num);
		printf("Now show the details of course %s.\n", c_num);
		show_course_details(c_num);
		display_transcript_operation();
		break;
	}
	case 2:printf("Go back to Student Menu.\n"); student_menu(); break;
	default:printf("Operation number is wrong. Please enter the correct number!\n"); display_transcript_operation();
	}
}

void transcript() {
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char query[200];
	printf("****************************\n");
	memset(query, 0, 200);
	sprintf(query, "SELECT UoSCode,Grade FROM transcript where StudId=\"%s\" ", username);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	printf("Course Number\tGrade\n");
	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL)
		{
			printf("%s\t", row[0]);
			printf("%s\n", row[1]);
		}
	}
	printf("****************************\n");
	mysql_free_result(res_set);
	display_transcript_operation();
}


void enroll() {
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char query[200];
	printf("****************************\n");
	sprintf(query, "SELECT UoSCode,UoSName FROM unitofstudy where UoSCode in (select UoSCode from uosoffering where Year=\"%s\" and Semester=\"%s\" )", year, quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	printf("Here is some courses you can enroll in this quarter:\n");
	printf("Course Number\tCourse Title\n");
	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL)
		{
			printf("%d.%s\t", i, row[0]);
			printf("%s\n", row[1]);
		}
	}
	printf("****************************\n");
	char q1[] = "Q1";
	char q2[] = "Q2";
	if (!(strcmp(quarter, q1)))
	{
		strcpy(quarter, q2);
		year[3] = year[3] + 1;
	}
	else quarter[1] = quarter[1] + 1;
	sprintf(query, "SELECT UoSCode,UoSName FROM unitofstudy where UoSCode in (select UoSCode from uosoffering where Year=\"%s\" and Semester=\"%s\" )", year, quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	numrows = (int)mysql_num_rows(res_set);
	printf("Here is some courses you can enroll in next quarter:\n");
	printf("Course Number\tCourse Title\n");
	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL)
		{
			printf("%d.%s\t", i, row[0]);
			printf("%s\n", row[1]);
		}
	}
	printf("****************************\n");
	printf("Which Courses do you want to enroll?\n");
	printf("Please enter the Course Number.\n");
	char c_num[20];
	scanf(" %s", c_num);

	mysql_free_result(res_set);
}

void withdraw() {

}

void personal_details() {

}


void display_menu_operation() {
	printf("****************************\n");
	printf("Which operation you want to take?\n");
	printf("1.Transcript\n");
	printf("2.Enroll\n");
	printf("3.Withdraw\n");
	printf("4.Personal Details\n");
	printf("5.Log out\n");
	printf("****************************\n");
	printf("Please input the operation number\n");
	int op_num;
	scanf(" %d", &op_num);
	switch (op_num) {
	case 1:printf("Transfer to Transcript screen!\n"); transcript(); break;
	case 2:printf("Transfer to Enroll screen!\n"); enroll(); break;
	case 3:printf("Transfer to Withdraw screen!\n"); withdraw(); break;
	case 4:printf("Transfer to Personal Details screen!\n"); personal_details(); break;
	case 5:printf("You are logging out your account.\n"); login(); break;
	default:printf("Operation number is wrong. Please enter the correct number!\n"); display_menu_operation();
	}
}

void student_menu() {
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char query[200];
	struct tm *newtime;
	char tmpbuf[128];
	time_t lt;
	lt = time(NULL);
	memset(query, 0, 200);
	memset(quarter, 0, 3);
	memset(tmpbuf, 0, 128);
	memset(year, 0, 50);
	newtime = localtime(&lt);
	printf("****************************\n");
	strftime(tmpbuf, 128, "Today is %A, day %d of %B in the year %Y.\n", newtime);
	printf(tmpbuf);
	strftime(year, 20, "%Y", newtime);
	int month = newtime->tm_mon + 1;
	switch (month)
	{
	case 1:strcpy(quarter,"Q2"); break;
	case 2:strcpy(quarter,"Q2");  break;
	case 3:strcpy(quarter,"Q2");  break;
	case 4:strcpy(quarter,"Q3");  break;
	case 5:strcpy(quarter,"Q3");  break;
	case 6:strcpy(quarter,"Q3");  break;
	case 7:strcpy(quarter,"Q4");  break;
	case 8:strcpy(quarter,"Q4");  break;
	case 9:strcpy(quarter,"Q1");  break;
	case 10:strcpy(quarter,"Q1");  break;
	case 11:strcpy(quarter,"Q1");  break;
	case 12:strcpy(quarter,"Q1"); break;
	}
	printf("****************************\n");
	printf("You are taking courses as follows in this quarter:\n");
	sprintf(query, "SELECT UoSName FROM unitofstudy WHERE UoSCode in(SELECT UoSCode FROM transcript where StudId =\"%s\" and Year =\"%s\" and Semester =\"%s\");", username,year,quarter);
	mysql_query(conn, query);
	res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL)
		{
			printf("%s\n",row[0]);
		}
	}
	mysql_free_result(res_set);
	display_menu_operation();

}

void login() {
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	mysql_query(conn, "SELECT * FROM student;");
	res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	memset(username, 0, 50);
	memset(password, 0, 50);
	printf("****************************\n");
	printf("Please enter your username!\n");
	scanf(" %[^\n]", &username);
	printf("Please enter your password!\n");
	scanf(" %[^\n]", &password);
	printf("****************************\n");
	int flag = 0;
	for (int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(res_set);
		if (row != NULL)
		{
			if ((!(strcmp(username,row[0])) && !(strcmp(password,row[2]))))
				flag = 1;
		}
	}
	if (flag == 1) {
		printf("Succeed logging! Now transfer to The STUDENT MENU.\n");
		student_menu();
	}
	else printf("The username or password is not correct!\n");
	mysql_free_result(res_set);
}



void main() {
	conn = mysql_init(NULL);
	mysql_real_connect(
		conn, "localhost", "root", "0817", "project3-nudb", 3306, NULL, NULL);
	login();
	mysql_close(conn);
}