

int isGreaterThan(const String& other) const  // 1 means greater // 2 means less // 3 means equal
{
	int i = 0;
	while (data[i] && other.data[i])
	{
		if (data[i] > other.data[i]) return 1;
		if (data[i] < other.data[i]) return 2;
		i++;
	}
	// If one string is a prefix of the other, the longer string is greater
	if (data[i] && !other.data[i]) return 1;
	if (!data[i] && other.data[i]) return 2;
	return 3; // Strings are equal
}