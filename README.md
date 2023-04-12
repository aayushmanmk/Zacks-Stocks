Zacks Stocks
---


A program to get information from https://www.zacks.com about the 503 stocks and store them into .dat files written in batch:


```
#!/bin/bash

# to over come the single quote problem  this gets me the correct line

# $ awk -F" " '{ printf("{x:\x27%s\x27, y:%s}\n",$1, $2) }' newfile

stockslist=`cat snp.txt`
#stockslist=`cat stocks.txt`


for stock in $stockslist
do
echo $stock
/usr/bin/curl --output $stock.dat https://www.zacks.com/stock/quote/$stock?q=$stock

done

```


A program in c to get the parts of the information (Name of the stock, Date, Price, Rank) :

```
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


```

A program in batch to do the same for 503 files that are given in SNP.txt:

```
#!/bin/bash
datafiles=`ls *.dat`
for file in $datafiles
do
	./readfilegets $file
done

```



This information can be put into an sql database easily by adding the "INSERT INTO TABLE_NAME VALUES();" command and can be used for easy selectiion of data.



When put into an sql database, the following code can be used to check the stock data changes in the rank:

```
#!/bin/bash
dateno=$(sqlite3 test.db "select count(distinct Date) from ZACKS_STOCKS;")
stocklist=`cat snp.txt`
for stock in $stocklist
do
ranks=$(sqlite3 test.db "select rank from ZACKS_STOCKS WHERE SYMBOL=\"$stock\";")

IFS=$'\n' read -d '' -ra lines_array <<< "$ranks"

all_lines_match=true

# Compare lines
for (( i=1; i<${#lines_array[@]}; i++ ))
do
  if [ "${lines_array[i]}" != "${lines_array[0]}" ]; then
    all_lines_match=false
    break
  fi
done

distinct_lines=($(printf '%s\n' "${lines_array[@]}" | sort -u))

# Print all lines if they do not match
if ! $all_lines_match; then

array_length=${#distinct_lines[@]}

# Loop through each element in the array
for ((i=0; i<$array_length; i++))
do
  # If it is the first element, start the sentence with $
  if [ $i -eq 0 ]
  then
    sentence="$stock went from"
  else
    sentence+=" ${distinct_lines[$(($i-1))]} to ${distinct_lines[$i]} to"
  fi
done

newsentence="${sentence::-3}"

# Print the final sentence
echo $newsentence

  sqlite3 test.db "select * from ZACKS_STOCKS WHERE SYMBOL=\"$stock\" ORDER BY DATE;"
  echo " "
  stockcount=$((stockcount+1))


fi

done

echo "Different stocks found: " $stockcount

```

all files neccessary are given.
