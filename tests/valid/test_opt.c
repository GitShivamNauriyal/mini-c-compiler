int x;
int y;
x = 2 + 3 * 4; // Should fold to 14
y = x + 0;     // Should not change for now, but good to test
int z;
z = 10 / 2;    // Should fold to 5
