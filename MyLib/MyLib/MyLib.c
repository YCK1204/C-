#include "MyLib.h"

DLLEXPORT char* Trim(char* str) {
	if (str == NULL)
		return NULL;

	size_t start = 0;
	size_t end = strlen(str) - 1;
	while (str[start] && isspace(str[start]))
		start++;
	while (end > start && isspace(str[end]))
		end--;
	return Substr(str, start, end - start + 1);
}

DLLEXPORT char* _Substr(char* str, size_t start) {
	if (str == NULL)
		return NULL;

	size_t len = strlen(str) - start;
	char* ret = (char*)malloc(sizeof(char) * (len + 1));
	for (size_t i = start; str[i]; i++)
		ret[i - start] = str[i];
	ret[len] = 0;
	return ret;
}

DLLEXPORT char* Substr(char* str, size_t start, size_t count) {
	if (str == NULL)
		return NULL;
	size_t len = strlen(str);

	if (len < start || len - start < count)
		return NULL;
	char* ret = (char*)malloc(sizeof(char) * (count + 1));
	for (size_t i = 0; i < count; i++) {
		ret[i] = str[i + start];
	}
	ret[count] = 0;
	return ret;
}

DLLEXPORT char* Getline(int fd) {
	static char* remainWords = NULL;
	char buf[BUFFER_SIZE + 1];

	while (true) {
		int n = _read(fd, buf, BUFFER_SIZE);
		if (n <= 0) {
			fprintf(stderr, "Error: Invalid fd access");
			return NULL;
		}
		buf[n] = 0;
		size_t size = 0;
		if (remainWords == NULL) {
			remainWords = _strdup(buf);
		}
		else {
			size = _msize(remainWords);
			remainWords = (char*)realloc(remainWords, sizeof(char) * (size + n));
			char* tmp = StrJoin(remainWords, buf);
			ReplaceString(&remainWords, tmp);
			size--;
		}

		for (size_t i = size; remainWords[i]; i++) {
			if (remainWords[i] == '\n') {
				remainWords[i] = 0;
				char* ret = _Substr(remainWords, 0);
				if (remainWords[i + 1] == 0) {
					free(remainWords);
					remainWords = NULL;
				}
				else {
					ReplaceString(remainWords, &remainWords[i + 1]);
				}
				return ret;
			}
		}
	}
	return NULL;
}

DLLEXPORT char** Split(char* str, int delim, bool trim) {
	if (str == NULL || delim == 0)
		return NULL;

	int cnt = 0;
	int i = 0;
	while (str[i]) {
		while (str[i] && str[i] == delim)
			i++;
		if (str[i] && str[i] != delim) {
			cnt++;
			while (str[i] && str[i] != delim)
				i++;
		}
	}

	char** ret = (char**)malloc(sizeof(char*) * (cnt + 1));
	i = 0;
	int j = 0;
	while (str[j]) {
		while (str[j] && str[j] == delim)
			j++;
		int start = j;
		while (str[j] && str[j] != delim)
			j++;
		char* tmp = Substr(str, start, j - start);
		if (trim) {
			ret[i] = Trim(tmp);
			free(tmp);
		}
		else {
			ret[i] = Trim(tmp);
		}
		i++;
	}
	ret[cnt] = NULL;
	return ret;
}

DLLEXPORT char* StrJoin(char* s1, char* s2) {
	if (s1 == NULL || s2 == NULL)
		return NULL;

	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	char* ret = (char*)malloc(sizeof(char) * (len1 + len2 + 1));
	for (int i = 0; s1[i]; i++) { ret[i] = s1[i]; }
	for (int i = 0; s2[i]; i++) { ret[i + len1] = s2[i]; }
	ret[len1 + len2] = 0;
	return ret;
}

DLLEXPORT int StrFindByChar(char* str, char target, size_t pos) {
	if (str == NULL)
		return -1;
	for (int i = pos; str[i]; i++)
	{
		if (str[i] == target)
			return i;
	}
	return -1;
}

DLLEXPORT int StrFindByString(char* str, char* target, size_t pos) {
	if (str == NULL)
		return -1;
	for (int i = pos; str[i]; i++)
	{
		for (int j = 0;; j++) {
			if (target[j] == 0)
				return i;
			if (str[i + j] != target[j])
				break;
		}
	}
	return -1;
}

DLLEXPORT void ReplaceString(char** oldString, char* newString) {
	if (*oldString == NULL || newString == NULL)
		return;
	char* tmp = _strdup(newString);
	if (_msize(*oldString) > 0)
		free(*oldString);
	if (_msize(newString) > 0)
		free(newString);
	*oldString = tmp;
}

DLLEXPORT char* IntToAscii(int n) {
	if (n == INT_MIN)
		return _strdup("-2147483648");

	int len = 1;
	int tmp = n;
	bool negative = (n < 0);
	n = abs(n);

	len += negative;
	while ((tmp /= 10)) len++;
	char* ret = (char*)malloc(sizeof(char) * (len + 1));
	for (int i = len - 1; i >= 0 + negative; i--) {
		ret[i] = n % 10 + '0';
		n /= 10;
	}
	ret[len] = 0;
	if (negative)
		ret[0] = '-';
	return ret;
}

DLLEXPORT char* CharToString(char c) {
	char* ret = (char*)calloc(2, sizeof(char));
	ret[0] = c;
	return ret;
}

DLLEXPORT void _InsertString(char** original, size_t pos, char c) {
	if (original == NULL || *original == NULL)
		return NULL;
	char* oldString = *original;
	size_t len = strlen(oldString);
	if (pos > len)
		return NULL;

	char* ret = (char*)calloc(len + 2, sizeof(char));
	ret = MyStrncpy(ret, oldString, 0, pos);
	ret[pos] = c;
	size_t i = pos + 1;
	for (; oldString[i - 1]; i++)
		ret[i] = oldString[i - 1];
	if (_msize(oldString) > 0)
		free(oldString);
	*original = ret;
}

DLLEXPORT void InsertString(char** original, size_t pos, char* str) {
	if (original == NULL || *original == NULL || str == NULL)
		return NULL;
	char* oldString = *original;
	size_t len1 = strlen(oldString);
	size_t len2 = strlen(str);

	char* ret = (char*)calloc(len1 + len2 + 1, sizeof(char));
	ret = MyStrncpy(ret, oldString, 0, pos);
	ret = MyStrcpy(ret, str, pos);
	size_t i = pos + 1;
	for (; oldString[i - 1]; i++)
		ret[i] = oldString[i - 1];
	if (_msize(oldString) > 0)
		free(oldString);
	*original = ret;
}

DLLEXPORT char* MyStrcpy(char* dest, char* src, size_t pos) {
	if (src == NULL)
		return dest;
	return MyStrncpy(dest, src, pos, strlen(src));
}

DLLEXPORT char* MyStrncpy(char* dest, char* src, size_t pos, size_t n) {
	if (dest == NULL)
		return NULL;
	if (src == NULL || n == 0)
		return dest;
	size_t len = strlen(dest);
	if (strlen(src) < n)
		return dest;
	size_t i = 0;
	for (; i < n; i++)
		dest[i + pos] = src[i];
	dest[i + pos] = 0;
	return dest;
}