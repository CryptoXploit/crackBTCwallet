typedef union	union_INT256	{
	char lineal[32];
	uint64_t number64[4];
	uint32_t number32[8];
	uint16_t number16[16];
}INT256;

typedef struct str_list	{
	int n;
	char **data;
	int *lengths;
}List;

typedef struct str_tokenizer	{
	int current;
	int n;
	char *copydata;
	char **tokens;
}Tokenizer;

char *ltrim(char *str, const char *seps);
char *rtrim(char *str, const char *seps);
char *trim(char *str, const char *seps);
int indexOf(char *s,const char **array,int length_array);

int hexchr2bin(char hex, char *out);
int hexs2bin(char *hex, unsigned char *out);
char *tohex(char *ptr,int length);

int hasMoreTokens(Tokenizer *t);
char *nextToken(Tokenizer *t);

char *ltrim(char *str, const char *seps)	{
	size_t totrim;
	if (seps == NULL) {
		seps = "\t\n\v\f\r ";
	}
	totrim = strspn(str, seps);
	if (totrim > 0) {
		size_t len = strlen(str);
		if (totrim == len) {
			str[0] = '\0';
		}
		else {
			memmove(str, str + totrim, len + 1 - totrim);
		}
	}
	return str;
}

char *rtrim(char *str, const char *seps)	{
	int i;
	if (seps == NULL) {
		seps = "\t\n\v\f\r ";
	}
	i = strlen(str) - 1;
	while (i >= 0 && strchr(seps, str[i]) != NULL) {
		str[i] = '\0';
		i--;
	}
	return str;
}

char *trim(char *str, const char *seps)	{
	return ltrim(rtrim(str, seps), seps);
}

int indexOf(char *s,const char **array,int length_array)	{
	int index = -1,i,continuar = 1;
	for(i = 0; i <length_array && continuar; i++)	{
		if(strcmp(s,array[i]) == 0)	{
			index = i;
			continuar = 0;
		}
	}
	return index;
}

char *nextToken(Tokenizer *t)	{
	if(t->current < t->n)	{
		t->current++;
		return t->tokens[t->current-1];
	}
	else {
		return  NULL;
	}
}
int hasMoreTokens(Tokenizer *t)	{
	return (t->current < t->n);
}

void stringtokenizer(char *data,Tokenizer *t)	{
	//printf("stringtokenizer\n");
	char *token;
	int len;
	t->tokens = NULL;
	t->n = 0;
	t->current = 0;
	t->copydata = NULL;
	len = strlen(data);
	t->copydata = (char *) malloc(len+1);
	if(t->copydata == NULL)	{
		printf("Out of memory\n");
		exit(0);
	}
	memcpy(t->copydata,data,len+1);
	trim(t->copydata,"\t\n\r ");
	token = strtok(t->copydata," ");
	while(token != NULL)	{
		t->tokens = realloc(t->tokens,sizeof(char*)*(t->n+1));
		if(t->tokens == NULL)	{
			printf("Out of memory\n");
			exit(0);
		}
		t->tokens[t->n] = token;
		t->n++;
		token = strtok(NULL," ");
	}
	//printf("stringtokenizer\n");
}

void freetokenizer(Tokenizer *t)	{
	if(t->n > 0)	{
		memset(t->tokens,0,sizeof(char*)*t->n );
		free(t->tokens);
	}
	if(t->copydata != NULL)	{
		free(t->copydata);
	}
	memset(t,0,sizeof(struct str_tokenizer));
}


/*
	Aux function to get the hexvalues of the data
*/

char *tohex(char *ptr,int length){
  char *buffer;
  int offset = 0;
  unsigned char c;
  buffer = (char *) malloc((length * 2)+1);
  for (int i = 0; i <length; i++) {
    c = ptr[i];
	sprintf((char*) (buffer + offset),"%.2x",c);
	offset+=2;
  }
  buffer[length*2] = 0;
  return buffer;
}

int hexs2bin(char *hex, unsigned char *out)	{
	int len;
	char   b1;
	char   b2;
	int i;

	if (hex == NULL || *hex == '\0' || out == NULL)
		return 0;

	len = strlen(hex);
	if (len % 2 != 0)
		return 0;
	len /= 2;

	memset(out, 'A', len);
	for (i=0; i<len; i++) {
		if (!hexchr2bin(hex[i*2], &b1) || !hexchr2bin(hex[i*2+1], &b2)) {
			return 0;
		}
		out[i] = (b1 << 4) | b2;
	}
	return len;
}

int hexchr2bin(const char hex, char *out)	{
	if (out == NULL)
		return 0;

	if (hex >= '0' && hex <= '9') {
		*out = hex - '0';
	} else if (hex >= 'A' && hex <= 'F') {
		*out = hex - 'A' + 10;
	} else if (hex >= 'a' && hex <= 'f') {
		*out = hex - 'a' + 10;
	} else {
		return 0;
	}

	return 1;
}

void addItemList(char *data, List *l)	{
	/*
	printf("addItemList");
	printf("item : %i\n",l->n);
	printf("l->data : %p\n",l->data);
	*/
	l->data = realloc(l->data,sizeof(char*) * (l->n +1));
	//printf("l->data : %p\n",l->data);

	//printf("data : %p\n",data);
	l->data[l->n] = data;
	//printf("l->data[%i] : %p: %s\n",l->n,l->data[l->n],l->data[l->n]);
	l->n++;
	//printf("addItemList");
}

int isValidHex(char *data)	{
	//printf("isValidHex\n");
	char c;
	int len,i,valid = 1;
	len = strlen(data);
	for(i = 0 ; i <  len && valid;i++ )	{
		c = data[i];
		valid = ( (c >= '0' && c <='9') || (c >= 'A' && c <='F' ) || (c >= 'a' && c <='f' ) );
	}
	//printf("isValidHex\n");
	return valid;
}
