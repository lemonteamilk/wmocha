#include <stdio.h>

int main()
{
	int a, b, d;
	FILE *fp,*fdd;
	int error;
	char c;
	char s[5];
	a = b = d = 0;
	error = fopen_s(&fdd,"rnp.data", "r");
	fscanf_s(fdd, "%d %d %d %c %s", &a, &b, &d, &c, 1, s, 5);
	printf("%d\n%d\n%d\n%c\n%s\n", a, b, d, c, s);
	error = fopen_s(&fp,"testbin.data", "wb+");
	fprintf(fp, "%d;%d;%d;%c;%s\n-FileEnd-", a, b, d, c, s);
	fclose(fp);
}
