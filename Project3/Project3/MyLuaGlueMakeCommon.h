
int strpos(char *haystack, char *needle)
{

   if (strlen(haystack) ==0) return -1;

   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack;
   return -1;   // Not found = -1.
}

void mystrcpy(char* dst, int max, int pos, char* src) {
	int count = 0;
	int mypos = pos;
	while (count < max && (src[mypos] != '\0')) {
		dst[count] = src[mypos];
		mypos++;
		count++;
	}
	dst[max-1] = '\0';
}

void mystrcpy2(char *dst, int max, int mpos,  char* src) {
	int count = 0;
	int mypos = mpos;
	while (count < max && mypos > count && (src[count] != '\0')) {
		dst[count] = src[count];
		count++;
	}
	dst[max-1] = '\0';

}