// Ref manual page 376
__attribute__((section (".isr_vector"))) const int irq_table[107];

int main()
{
  char *c = "Toto is happy c:";
  return 0;
}
