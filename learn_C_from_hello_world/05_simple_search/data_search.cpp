#include<stdio.h>

int main() {
	float v1, v2, v3, v4, tar=1.1;
	FILE *dat;
	int ni=0;
	char rstr[6];
	char ll[30];

	fopen_s(&dat, "test.data", "r");

	while (fgets(ll, 30, dat) != NULL) {
		++ni;
		sscanf_s(ll, "%g %g %g %g %6s", &v1, &v2, &v3, &v4, rstr, 6);
		if (v1 == tar) {
			printf("Line %d,%s\n\n", ni, rstr);
		}
		else {
		}
	}
}