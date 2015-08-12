#ifndef LOGINCODE_H
#define LOGINCODE_H

namespace myLoginCode
{

static char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//static char base64[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/";
static int pos(char c)
{
  char *p;
  for(p = base64; *p; p++)
    if(*p == c)
      return p - base64;
  return -1;
}

int base64_decode(const char *str, char **data)
{
  const char *s, *p;
  unsigned char *q;
  int c;
  int x;
  int done = 0;
  int len;
  s = (const char *)malloc(strlen(str));
  q = (unsigned char *)s;
  for(p=str; *p && !done; p+=4){
	  x = pos(p[0]);
	  if(x >= 0)
		  c = x;
	  else{
		  done = 3;
		  break;
	  }
	  c*=64;
	  
	  x = pos(p[1]);
	  if(x >= 0)
		  c += x;
	  else
		  return -1;
	  c*=64;
	  
	  if(p[2] == '=')
		  done++;
	  else{
		  x = pos(p[2]);
		  if(x >= 0)
			  c += x;
		  else
			  return -1;
	  }
	  c*=64;
	  
	  if(p[3] == '=')
		  done++;
	  else{
		  if(done)
			  return -1;
		  x = pos(p[3]);
		  if(x >= 0)
			  c += x;
		  else
			  return -1;
	  }
	  if(done < 3)
		  *q++=(c&0x00ff0000)>>16;
	  
	  if(done < 2)
		  *q++=(c&0x0000ff00)>>8;
	  if(done < 1)
		  *q++=(c&0x000000ff)>>0;
  }

  len = q - (unsigned char*)(s);

  *data = (char*)realloc((void *)s, len);

  return len;
}

char* MyLoginDecode(char *str)
{
	int		i, len;
	char	*data = NULL;
	len = base64_decode(str, &data);
	
	for (i = 0; i < len; i++)
	{
		data[i] -= 0x86;
		data[i] ^= 0x19;

	}
	return data;
}

static char _base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int base64_encode(const void *data, int size, char **str)
{
	char *s, *p;
	int i;
	int c;
	const unsigned char *q;

	p = s = (char*)malloc(size*4/3+4);
	if (p == NULL)
		return -1;
	q = (const unsigned char*)data;
	i=0;
	for(i = 0; i < size;){
		c=q[i++];
		c*=256;
		if(i < size)
			c+=q[i];
		i++;
		c*=256;
		if(i < size)
			c+=q[i];
		i++;
		p[0]=_base64[(c&0x00fc0000) >> 18];
		p[1]=_base64[(c&0x0003f000) >> 12];
		p[2]=_base64[(c&0x00000fc0) >> 6];
		p[3]=_base64[(c&0x0000003f) >> 0];
		if(i > size)
			p[3]='=';
		if(i > size+1)
			p[2]='=';
		p+=4;
	}
	*p=0;
	*str = s;
	return strlen(s);
}

char* MyLoginEncode(char *str)
{
	int		i, len;
	char	*p;
	char	*s, *data;
	len = strlen(str) + 1;
	s = (char *)malloc(len);
	memcpy(s, str, len);
	for (i = 0; i < len; i++)
	{
		s[i] ^= 0x19;
		s[i] += 0x86;
	}
	base64_encode(s, len, &data);
	free(s);
	return data;
}

}

#endif