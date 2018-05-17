void break_here(void)
{
  return;
}

int main(void)
{
  while (1)
    break_here();

  return 0;
}
