unsigned int chksum(unsigned char *check,unsigned int size)
{
	unsigned int	i;
	unsigned long	sum = 0;
	unsigned int	*ptr = (unsigned int *)check;

	for (i = 0; i < (size) / 2; i++)
	{
		sum += *ptr++;
	}
	if (size & 0x01)		//表示长度为单数
	{
		sum += ((*ptr)&0xff);
	}
	sum = (sum & 0xffff) + ((sum>>16)&0xffff);//高16位和低16位相加
	if(sum&0xffff0000)
	{//表示有进位
		sum++;
	}
	return ((unsigned int)((sum)&0xffff));
}

unsigned int Swap16U(unsigned int data16u)
{
	unsigned int	a;
	a = data16u >> 8;
	a += ((data16u & 0xFF) << 8);
	return	a;
}

unsigned long Add16U(unsigned int a, unsigned int b)
{
	unsigned long	tmp;
	tmp = (((unsigned long)Swap16U(a)) << 16) + ((unsigned long)Swap16U(b));
	return	tmp;
}
