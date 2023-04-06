// C program to implement
// the above approach
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* replaceWord(const char* s, const char* oldW,
				const char* newW)
{
	char* result;
	int i, cnt = 0;
	int newWlen = strlen(newW);
	int oldWlen = strlen(oldW);

	// Counting the number of times old word
	// occur in the string
	for (i = 0; s[i] != '\0'; i++) {
		if (strstr(&s[i], oldW) == &s[i]) {
			cnt++;

			// Jumping to index after the old word.
			i += oldWlen - 1;
		}
	}

	// Making new string of enough length
	result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);

	i = 0;
	while (*s) {
		// compare the substring with the result
		if (strstr(s, oldW) == s) {
			strcpy(&result[i], newW);
			i += newWlen;
			s += oldWlen;
		}
		else
			result[i++] = *s++;
	}

	result[i] = '\0';
	return result;
}
// C program to trim leading whitespaces
 
 
void removeLeading(char *str, char *str1)
{
    int idx = 0, j, k = 0;
 
    // Iterate String until last
    // leading space character
    while (str[idx] == ' ' || str[idx] == '\t' || str[idx] == '\n')
    {
        idx++;
    }
 
    // Run a for loop from index until the original
    // string ends and copy the content of str to str1
    for (j = idx; str[j] != '\0'; j++)
    {
        str1[k] = str[j];
        k++;
    }
 
    // Insert a string terminating character
    // at the end of new string
    str1[k] = '\0';
 
    // Print the string with no whitespaces
	str = str1;
}


// Driver code
int main(int argc,char* argv[])
{
	FILE* ptr;
	char str[300];
	char *ret;
	char *name;
	char *pr;
	char *dt;
	char* result = NULL;
	char* newpricestr = NULL;
	char* date = NULL;
	
	int rankflag;
	rankflag=0;
	int nameflag;
	nameflag=0;
	ptr = fopen(argv[1], "r");
	if (NULL == ptr) {
		printf("file %s can't be opened \n", argv[1]);
		return 1;
	}
	while (fgets(str, 300, ptr) != NULL)
	{
		str[strlen(str)-1]='\0';
		dt = strstr(str,"to_date");
		if(dt!=NULL)
		{
			date = replaceWord(str,"<input type=\"hidden\" class=\"to_date\" value=","");
			date = replaceWord(date,"/>","");
			removeLeading(date,date);

		}
	}
	fclose(ptr);
	ptr = fopen(argv[1], "r");
	if (NULL == ptr) {
		printf("file %s can't be opened \n", argv[1]);
		return 1;
	}
	while (fgets(str, 300, ptr) != NULL)
	{
		ret = strstr(str,"<title>");
		if (ret!=NULL)
		{
			result = replaceWord(str,"- Stock Price Today - Zacks</title>","");
			result = replaceWord(result,"<title>","");
			result[strlen(result)-1]='\0';
			removeLeading(result,result);
			nameflag=1;
		}
		
		pr = strstr(str,"<p class=\"last_price\">$");
		if(pr!=NULL)
		{
			newpricestr = replaceWord(str,"<span> USD</span></p>","");
			newpricestr = replaceWord(newpricestr,"<p class=\"last_price\">$","");
			newpricestr[strlen(newpricestr)-1]='\0';
			removeLeading(newpricestr,newpricestr);
			
		}
		ret = strstr(str,"1-Strong Buy");
		if (ret!=NULL)
		{
   			printf("%s,\"%s\",%s,\"1-Strong Buy\" \n",date,result,newpricestr);
			rankflag=1;
        	}
		else
		{
			ret=strstr(str,"3-Hold");
			if (ret!=NULL)
			{
				printf("%s,\"%s\",%s,\"3-Hold\" \n",date,result,newpricestr);
				rankflag=1;
			}
			else
			{
				ret=strstr(str,"2-Buy");
				if(ret!=NULL)
				{
					printf("%s,\"%s\",%s,\"2-Buy\" \n",date,result,newpricestr);
					rankflag=1;
				}
				else
				{
					ret=strstr(str,"4-Sell");
					if(ret!=NULL)
					{
						printf("%s,\"%s\",%s,\"4-Sell\" \n",date,result,newpricestr);
						rankflag=1;
					}
					else
					{
						ret=strstr(str,"5-Strong Sell");
						if(ret!=NULL)
						{
							printf("%s,\"%s\",%s,\"5-Strong Sell\" \n",date,result,newpricestr);
							rankflag=1;
						}
					}
				}
			}
		}

	}
	

	if(rankflag!=1)
	{
		printf("%s,\"%s\",%s,\"No Rank\" \n",date,result,newpricestr);
	}
	
	fclose(ptr);
	return 0;
}
