int a = 10;
int b = 20;
int c;
int d;

// CSE Test: c and d should share the same calculation
c = a + b;
d = a + b;

int x = 0;
int y = 5;
int z = 10;

// LICM Test: y + z should be moved outside the loop
while (x < 10) {
    a = y + z;
    x = x + 1;
}
