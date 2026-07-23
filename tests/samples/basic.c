int	global_data = 42;
int	global_bss;
static int	static_data = 7;
static int	static_bss;

void	foo(void)
{
	global_bss = global_data + static_data + static_bss;
}

int	main(void)
{
	foo();
	return (global_bss);
}
