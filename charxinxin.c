#include <stdio.h>
#include <string.h>

int main()
{
	char aaa[10][40] = {"sdhf  fhsad jsd hf",
					"asdhfsd asj kjsdkl ",
					"asdf sad sdaj ",
					"poshnn asd we ",
					"piwenjkd wqe ",
					"weui jksd hsdj jk",
					"wer sd ",
					"qwer sduf hsdj ",
					"jksdasjd",
					"ruiewy uew "
	};

	int i = 0;
	while(*(aaa+i) && i < 10)
	{
		printf("%s\n",*(aaa+i));
		i++;
	}
}
